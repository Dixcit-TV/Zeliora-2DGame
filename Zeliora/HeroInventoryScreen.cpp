#include "pch.h"
#include "HeroInventoryScreen.h"
#include "Hero.h"
#include "Weapon.h"
#include "Armor.h"
#include "Equipment.h"
#include <stdexcept>
#include "GameSettings.h"

HeroInventoryScreen::HeroInventoryScreen(Hero* pPlayer)
	: MenuScreen()
	, m_pTitleTexture{ new Texture("Inventory & Equipment", ResourceManager::GetInstance()->GetFont(ResourceManager::FontSize::size_36), ResourceManager::GetInstance()->GetFontColor(ResourceManager::FontCategory::classic)) }
	, m_pPlayer{ pPlayer }
	, m_CharacterBox{}
	, m_ObjectSlots{}
	, m_EquipmentSlots{}
{
	const float objectsOffset{ 10.f };

	if (!pPlayer)
		throw std::runtime_error("Player could not be accessed in Game Menu.");
	
	// Button Panel
	m_Buttons.push_back(Button<ButtonPayLoad, MenuScreen>(OnReturnButtonClicked, this, "Return", Point2f{ m_ButtonXOffset, 50.f }));

	// Character Panel
	float xPos{ GetButtonBoxWidth() + objectsOffset };
	m_CharacterBox = Rectf { xPos , 0.f, 980.f - xPos, 720.f };

	// Object Panel
	Point2f objectBotleft{ xPos + m_CharacterBox.width + objectsOffset, 650.f };

	const float cellHeight{ 70.f };
	const float cellWidth{ 1280.f - objectBotleft.x };

	const size_t inventorySize{ m_pPlayer->GetInventory().Capacity() };
	m_ObjectSlots.reserve(inventorySize);
	for (size_t idx{}; idx < inventorySize; idx++)
	{
		m_ObjectSlots.push_back(Rectf{ objectBotleft.x, objectBotleft.y, cellWidth, cellHeight });
		objectBotleft.y -= cellHeight + objectsOffset;
	}
}

HeroInventoryScreen::~HeroInventoryScreen()
{
	delete m_pTitleTexture;
}

ButtonPayLoad HeroInventoryScreen::ProcessClick(const Point2f& clickPos)
{
	const size_t numSlots{ m_ObjectSlots.size() };
	const Inventory& playerInventory = m_pPlayer->GetInventory();
	for (size_t idx{}; idx < numSlots; idx++)
	{
		if (idx <= playerInventory.Size() - 1)
		{
			if (utils::IsPointInRect(clickPos, m_ObjectSlots[idx]))
			{
				m_pPlayer->Equip(idx);
			}
		}
	}

	return MenuScreen::ProcessClick(clickPos);
}

void HeroInventoryScreen::Draw() const
{
	float scrollOffset{};
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_UP])
	{
		scrollOffset -= 10.f;
	}

	if (pStates[SDL_SCANCODE_DOWN])
	{
		scrollOffset += 10.f;
	}

	if (scrollOffset != 0)
	{
		if (m_ObjectSlots[0].bottom + m_ObjectSlots[0].height <= 720.f && scrollOffset < 0.f)
		{
		}
		else if (m_ObjectSlots[m_ObjectSlots.size() - 1].bottom >= 0.f && scrollOffset > 0.f)
		{
		}
		else
		{
			std::vector<Rectf>& slots = const_cast<std::vector<Rectf>&>(m_ObjectSlots);
			for (size_t idx{}; idx < m_ObjectSlots.size(); idx++)
			{
				slots[idx].bottom += scrollOffset;
			}
		}
	}

	MenuScreen::Draw();
	DrawEquipment();
	DrawInventory();
}

void HeroInventoryScreen::DrawEquipment() const
{
	const float equipmentOffset{ 20.f };
	const float cellHeight{ m_ObjectSlots[0].height };
	const float cellWidth{ m_ObjectSlots[0].width };

	m_pTitleTexture->Draw(Point2f{ m_CharacterBox.left + m_CharacterBox.width / 2 - m_pTitleTexture->GetWidth() / 2, GameSettings::GetInstance()->GetGameWindow().height - 100.f });

	const std::unordered_map<Armor::ArmorType, const Armor*>& armorPieces{ m_pPlayer->GetEquipment().GetArmorSet() };

	Rectf equipmentRect{ m_CharacterBox.left + m_CharacterBox.width / 2 - cellWidth / 2, GameSettings::GetInstance()->GetGameWindow().height / 2.f + cellHeight, cellWidth, cellHeight };
	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	utils::DrawRect(equipmentRect, 2.f);
	if (armorPieces.find(Armor::ArmorType::helmet)->second)
		armorPieces.find(Armor::ArmorType::helmet)->second->DrawWithDescription(Point2f{ equipmentRect.left, equipmentRect.bottom + equipmentRect.height }, cellHeight);


	equipmentRect = Rectf{ m_CharacterBox.left + m_CharacterBox.width / 2 - cellWidth / 2, equipmentRect.bottom - cellHeight - equipmentOffset , cellWidth, cellHeight };
	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	utils::DrawRect(equipmentRect, 2.f);
	if (armorPieces.find(Armor::ArmorType::body)->second)
		armorPieces.find(Armor::ArmorType::body)->second->DrawWithDescription(Point2f{ equipmentRect.left, equipmentRect.bottom + equipmentRect.height }, cellHeight);


	equipmentRect = Rectf{ m_CharacterBox.left + m_CharacterBox.width / 4 - cellWidth / 2, equipmentRect.bottom - cellHeight - equipmentOffset, cellWidth, cellHeight };
	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	utils::DrawRect(equipmentRect, 2.f);
	if (m_pPlayer->GetEquipment().GetWeapon())
		m_pPlayer->GetEquipment().GetWeapon()->DrawWithDescription(Point2f{ equipmentRect.left, equipmentRect.bottom + equipmentRect.height }, cellHeight);


	equipmentRect = Rectf{ m_CharacterBox.left + (m_CharacterBox.width / 4 * 3) - cellWidth / 2, equipmentRect.bottom, cellWidth, cellHeight };
	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	utils::DrawRect(equipmentRect, 2.f);
	if (armorPieces.find(Armor::ArmorType::arms)->second)
		armorPieces.find(Armor::ArmorType::arms)->second->DrawWithDescription(Point2f{ equipmentRect.left, equipmentRect.bottom + equipmentRect.height }, cellHeight);

	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	utils::DrawRect(m_CharacterBox, 2.f);
}

void HeroInventoryScreen::DrawInventory() const
{
	const size_t numSlots{ m_ObjectSlots.size() };
	const Inventory& playerInventory = m_pPlayer->GetInventory();
	for (size_t idx{}; idx < numSlots; idx++)
	{
		if (playerInventory.Size() > 0 && idx <= playerInventory.Size() - 1)
		{
			playerInventory[idx]->DrawWithDescription(Point2f{ m_ObjectSlots[idx].left, m_ObjectSlots[idx].bottom + m_ObjectSlots[idx].height }, m_ObjectSlots[idx].height);
		}

		utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
		utils::DrawRect(m_ObjectSlots[idx], 2.f);
	}
}

ButtonPayLoad HeroInventoryScreen::OnReturnButtonClicked(MenuScreen* pThisObject)
{
	return ButtonPayLoad{ true, MenuState::back, nullptr };
}