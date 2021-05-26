#include "pch.h"
#include <iostream>
#include <fstream>
#include "Hero.h"
#include "utils.h"
#include "Dungeon.h"
#include "ResourceManager.h"
#include "SpriteTile.h"
#include "Sprite.h"
#include "EnemyManager.h"
#include "LootManager.h"
#include "Weapon.h"
#include "Armor.h"
#include "GameSettings.h"

Hero::Hero(Sprite* pSprite, const Point2f& startPos, float health, float movementSpeed, Attack&& attack)
	: Character(pSprite, startPos, health,  movementSpeed, std::move(attack))
	, m_MaxHealth{ health }
	, m_Level{ 1 }
	, m_Exp{}
	, m_ExpTarget{ 100 }
	, m_Inventory{}
	, m_Equipment{}
{
	m_Equipment.SetDefault();
}

void Hero::Update(float elapsedSec, const Point2f& mousePos, Dungeon* pDungeon)
{
	UpdateDirection(mousePos);
	Move(elapsedSec, pDungeon);

	if (m_Attack.IsInProcess())
	{
		m_Attack.Update(elapsedSec);

		if (m_Attack.IsInDoneState())
		{
			bool hit{ false };
			EnemyManager& worldEnemyMgr{ pDungeon->GetEnemyManager() };
			for (Enemy* pEnemy : worldEnemyMgr)
			{
				if (pEnemy->HitCheck(m_Attack, GetScaledDamage(m_Equipment.GetWeapon()->GetDamage())))
					hit = true;
			}

			std::vector<Destructible>& destructibles{ pDungeon->GetDestructibles() };

			for (Destructible& destructible : destructibles)
			{
				if (destructible.HitCheck(m_Attack))
					hit = true;
			}

			m_Attack.SetStateFromHitResult(hit);
		}
	}
	else
	{
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			float attackDirection{ atan2f(mousePos.y - m_Shape.center.y, mousePos.x - m_Shape.center.x) };
			float range{ m_Equipment.GetWeapon()->GetRange() };
			m_Attack.Start(Point2f{ m_Shape.center.x + (m_Shape.halfWidth + range) * cos(attackDirection), m_Shape.center.y + (m_Shape.halfHeight + range) * sin(attackDirection) }, range, attackDirection);
		}
	}
}

void Hero::Move(float elapsedSec, Dungeon* pDungeon)
{
	m_Velocity.x = m_Velocity.y = 0.f;
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	if (GameSettings::GetInstance()->GetInputResponse("MoveRight"))
	{
		m_Velocity.x = m_MovementSpeed;
	}
	if (GameSettings::GetInstance()->GetInputResponse("MoveLeft"))
	{
		m_Velocity.x = -m_MovementSpeed;
	}
	if (GameSettings::GetInstance()->GetInputResponse("MoveUp"))
	{
		m_Velocity.y = +m_MovementSpeed;
	}
	if (GameSettings::GetInstance()->GetInputResponse("MoveDown"))
	{
		m_Velocity.y = -m_MovementSpeed;
	}

	if (m_Velocity != Vector2f{})
	{
		if (m_Velocity.x != 0.f && m_Velocity.y != 0.f)
		{
			m_Velocity.x *= float(cos(utils::g_Pi_4));
			m_Velocity.y *= float(sin(utils::g_Pi_4));
		}

		m_Shape.center += m_Velocity * elapsedSec;
		m_pSprite->NextFrame(elapsedSec);

		for (const Enemy* pEnemy : pDungeon->GetEnemyManager())
		{
			HandleCollision(pEnemy);
		}

		pDungeon->Clamp(this, elapsedSec);
	}
	else
	{
		m_pSprite->ResetAnimation();
	}
}

void Hero::Draw() const
{
	m_pSprite->Draw(m_Shape, int(m_Direction));

	if (m_Attack.IsInProcess())
	{
		m_Attack.Draw(m_Equipment.GetWeapon()->GetSpriteTile(), m_Shape.center);
	}
}

void Hero::LootPickUpCheck(LootManager& lootManager)
{
	for (size_t idx{}; idx < lootManager.Size(); ++idx)
	{
		if (utils::IsPointInRect(lootManager[idx].GetPos(), m_Shape))
		{
			m_Inventory.AddObject(lootManager[idx].ReleaseObject());
		}
	}
}

void Hero::Equip(size_t inventoryIdx)
{
	const Object* pUnequippedObject = m_Equipment.PlaceOrReplace(m_Inventory[inventoryIdx]);

	if (pUnequippedObject)
		m_Inventory.ReplaceObject(inventoryIdx, pUnequippedObject);
}

void Hero::ExpUp()
{
	const int expPerkill{ int(10 * m_Level * GameSettings::m_GameMultiplierRatio) };

	m_Exp += expPerkill;
	if (m_Exp >= m_ExpTarget)
	{
		++m_Level;
		m_Exp = 0;
		m_ExpTarget = int(m_ExpTarget * (1 + GameSettings::m_GameMultiplierRatio));
		m_Health = m_MaxHealth *= GameSettings::m_GameMultiplierRatio;
		GameSettings::GetInstance()->UpdateWorldMultiplier(m_Level);
	}
}

int Hero::GetLevel() const
{
	return m_Level;
}

float Hero::GetHealthRatio() const
{
	return m_Health <= 0.f ? 0.f : m_Health / m_MaxHealth;
}

float Hero::GetExpToLevelRatio() const
{
	return float(m_Exp) / m_ExpTarget;
}

const Inventory& Hero::GetInventory() const
{
	return m_Inventory;
}

const Equipment& Hero::GetEquipment() const
{
	return m_Equipment;
}

std::vector<Hero*> Hero::GetAllHeroes()
{
	std::vector<Hero*> pHeroes;
	const std::string xPathQuery{ "Characters/Heroes/Hero" };

	std::ifstream iS{};
	iS.open(GetXmlPath());

	if (!iS.is_open())
	{
		throw std::runtime_error("Couldn't open file " + GetXmlPath());
	}

	const std::vector<std::string> heroElements{ utils::GetXmlElements(iS, xPathQuery) };
	iS.close();

	if (heroElements.empty())
		throw std::runtime_error("Hero Characters do not exist!");

	for (const std::string& element : heroElements)
	{
		pHeroes.push_back(new Hero(
			new Sprite(
				ResourceManager::GetInstance()->GetSpriteSheet(utils::GetXmlAttributeValue("SpritePath", element))
				, std::stoi(utils::GetXmlAttributeValue("SpriteOffset", element))
				, std::stoi(utils::GetXmlAttributeValue("Columns", element))
				, std::stoi(utils::GetXmlAttributeValue("Rows", element))
				, std::stoi(utils::GetXmlAttributeValue("StartColumn", element))
				, std::stoi(utils::GetXmlAttributeValue("StartRow", element))
				, std::stof(utils::GetXmlAttributeValue("Frametime", element))
			)
			, Point2f(0.f, 0.f)
			, std::stof(utils::GetXmlAttributeValue("Health", element))
			, std::stof(utils::GetXmlAttributeValue("MovementSpeed", element))
			, Attack{ utils::GetMeleeHitSprite(), 0.2f }
		));
	}

	return pHeroes;
}

void Hero::MenuDraw(const Rectp& shape) const
{
	m_pSprite->Draw(shape);
}

bool Hero::HitCheck(const Attack& attack, float damage)
{
	if (utils::IsOverlapping(m_Shape, attack.GetImpactArea()))
	{
		m_Health -= damage;

		return true;
	}

	return false;
}