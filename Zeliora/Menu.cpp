#include "pch.h"
#include "Menu.h"
#include "utils.h"
#include "MainMenuScreen.h"
#include "InGameMenuScreen.h"
#include "EndMenuScreen.h"
#include "Hero.h"

Menu::Menu(MenuType type, GameState* const pGameState, Hero** pHero)
	: m_pMenuScreens{}
	, m_pGameState{ pGameState }
	, m_pHero{ pHero }
	, m_MenuType{ type }
{}

Menu::Menu(Menu&& other) noexcept
	: m_pMenuScreens { other.m_pMenuScreens }
	, m_pGameState{ other.m_pGameState }
	, m_pHero{ other.m_pHero }
	, m_MenuType{ other.m_MenuType }
{
	while (!other.m_pMenuScreens.empty())
	{
		other.m_pMenuScreens.pop();
	}
}

Menu::~Menu()
{
	Clear();
}

void Menu::Draw() const
{
	m_pMenuScreens.top()->Draw();
}

void Menu::HoverCheck(const Point2f& mousePos)
{
	m_pMenuScreens.top()->ProcessHover(mousePos);
}

void Menu::ClickCheck(const Point2f& clickPos)
{
	ButtonPayLoad payLoad = m_pMenuScreens.top()->ProcessClick(clickPos);

	if (payLoad.clicked)
	{
		switch (payLoad.menuState)
		{
			case MenuState::next:
				m_pMenuScreens.push(reinterpret_cast<MenuScreen*>(payLoad.pArg));
				break;
			case MenuState::back:
				delete m_pMenuScreens.top();
				m_pMenuScreens.pop();

				if (m_pMenuScreens.empty())
				{
					switch (m_MenuType)
					{
					case MenuType::mainMenu:
						*m_pGameState = GameState::exit;
						break;
					case MenuType::inGameMenu:
						*m_pGameState = GameState::inGame;
						break;
					}
				}
				break;
			case MenuState::startGame:
				*m_pHero = reinterpret_cast<Hero*>(payLoad.pArg);
				*m_pGameState = GameState::init;
				Clear();
				break;
			case MenuState::exitGame:
				*m_pGameState = GameState::end;
				break;
		}
	}
}

void Menu::Load()
{
	switch (m_MenuType)
	{
	case MenuType::mainMenu:
		m_pMenuScreens.push(new MainMenuScreen());
		break;
	case MenuType::inGameMenu:
		m_pMenuScreens.push(new InGameMenuScreen(*m_pHero));
		break;
	case MenuType::endMenu:
		m_pMenuScreens.push(new EndMenuScreen());
		break;
	}
}

void Menu::Clear()
{
	while (!m_pMenuScreens.empty())
	{
		delete m_pMenuScreens.top();
		m_pMenuScreens.pop();
	}
}