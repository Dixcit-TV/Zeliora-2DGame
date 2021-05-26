#include "pch.h"
#include "HeroSelectionScreen.h"
#include "utils.h"
#include "GameSettings.h"

HeroSelectionScreen::HeroSelectionScreen()
	: MenuScreen()
	, m_pHeroes{}
	, m_SelectedHeroIdx{ -1 }
	, m_BotLeft{300.f, 420.f }
{
	m_pHeroes = Hero::GetAllHeroes();

	const float yButtonOffset{ 50.f };
	const float buttonsTop{ GameSettings::GetInstance()->GetGameWindow().height - 100.f };
	Point2f pos{ m_ButtonXOffset , buttonsTop };

	m_Buttons.push_back(Button<ButtonPayLoad, MenuScreen>(OnStartButtonClicked, this, "Start", pos));
	pos.y = yButtonOffset;
	m_Buttons.push_back(Button<ButtonPayLoad, MenuScreen>(OnReturnButtonClicked, this, "Back", pos));
}

HeroSelectionScreen::~HeroSelectionScreen()
{
	for (Hero* pHero : m_pHeroes)
		delete pHero;
}

ButtonPayLoad HeroSelectionScreen::ProcessClick(const Point2f& clickPos)
{
	for (int idx{}; idx < m_pHeroes.size(); ++idx)
	{
		Rectf itemShape{ GetHeroPosInMenu(idx) };
		itemShape.left += m_BotLeft.x;
		itemShape.bottom += m_BotLeft.y;
		if (utils::IsPointInRect(clickPos, itemShape))
		{
			m_SelectedHeroIdx = idx;
		}
	}

	return MenuScreen::ProcessClick(clickPos);
}

void HeroSelectionScreen::Draw() const
{
	MenuScreen::Draw();

	for (int idx{}; idx < m_pHeroes.size(); ++idx)
	{
		Rectf itemShape{ GetHeroPosInMenu(idx) };
		itemShape.left += m_BotLeft.x;
		itemShape.bottom += m_BotLeft.y;
		m_pHeroes[idx]->MenuDraw(Rectp{ Point2f{itemShape.left + itemShape.width / 2, itemShape.bottom + itemShape.height / 2}, itemShape.width / 2, itemShape.height / 2 });

		if (idx == m_SelectedHeroIdx)
		{
			utils::SetColor(Color4f{ .8f, .3f, .3f, 1.f });
			utils::DrawRect(itemShape, 3.f);
		}
	}
}

Rectf HeroSelectionScreen::GetHeroPosInMenu(int idx) const
{
	const float padding{ 30.f };
	const float cellSize{ 96.f };
	const int numCols{ int(m_pHeroes.size()) / 2 };
	const int numRows{ int(m_pHeroes.size()) / numCols };
	const int row{ idx / numCols };
	const int col{ idx % numCols };

	return Rectf{ (cellSize + padding) * col, (cellSize + padding) * row, cellSize, cellSize };
}

ButtonPayLoad HeroSelectionScreen::OnStartButtonClicked(MenuScreen* pThisObject)
{
	HeroSelectionScreen* pCastedThis{ static_cast<HeroSelectionScreen*>(pThisObject) };

	if (pCastedThis->m_SelectedHeroIdx != -1)
	{
		Hero* pSelectedHero{ pCastedThis->m_pHeroes[pCastedThis->m_SelectedHeroIdx] };
		pCastedThis->m_pHeroes[pCastedThis->m_SelectedHeroIdx] = nullptr;
		return ButtonPayLoad{ true, MenuState::startGame, static_cast<void*>(pSelectedHero) };
	}
	else
		return ButtonPayLoad{ false };
}

ButtonPayLoad HeroSelectionScreen::OnReturnButtonClicked(MenuScreen* pThisObject)
{
	return ButtonPayLoad{ true, MenuState::back, nullptr };
}