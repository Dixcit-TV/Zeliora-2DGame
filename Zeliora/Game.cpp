#include "pch.h"
#include <iostream>
#include "Game.h"
#include "Weapon.h"
#include "Armor.h"
#include "Potion.h"
#include "utils.h"
#include "Hero.h"
#include "ResourceManager.h"
#include "DungeonGenerator.h"
#include "Dungeon.h"
#include "GameSettings.h"

Game::Game( const Window& window ) 
	: m_MousePos{}
	, m_pPlayer{ nullptr }
	, m_pDungeon{ nullptr }
	, m_GameCamera{ Point2f{ 0.f, 0.f }, window.width / 2, window.height / 2 }
	, m_Hud{ &m_pPlayer }
	, m_GameState{ GameState::inMenu }
	, m_Menus{}
	, m_MouseStates{}
	, m_CurrentMenu{ MenuType::mainMenu }
{
	GameSettings::GetInstance()->SetGameWindow(window);

	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_MouseStates["Moved"] = false;
	m_MouseStates["LeftClick"] = false;

	m_Menus.emplace(MenuType::mainMenu, Menu{ MenuType::mainMenu, &m_GameState, &m_pPlayer });
	m_Menus.emplace(MenuType::inGameMenu, Menu{ MenuType::inGameMenu, &m_GameState, &m_pPlayer });
	m_Menus.emplace(MenuType::endMenu, Menu{ MenuType::endMenu, &m_GameState, &m_pPlayer });

	m_Menus.find(m_CurrentMenu)->second.Load();
}

void Game::Cleanup( )
{
	delete m_pPlayer;
	delete m_pDungeon;
	delete ResourceManager::GetInstance();
	delete GameSettings::GetInstance();
}

void Game::Update( float elapsedSec )
{
	switch (m_GameState)
	{
	case GameState::inMenu:
		MenuUpdate();
		break; 
	case GameState::init:
		LoadGame();
		m_GameState = GameState::inGame;
		break;
	case GameState::inGame:
		InGameUpdate(elapsedSec);
		break;
	case GameState::paused:
		break;
	case GameState::playerDead:
		m_GameState = GameState::inMenu;
		m_CurrentMenu = MenuType::endMenu;
		m_Menus.find(m_CurrentMenu)->second.Load();
		break;
	case GameState::end:
		EndGameUpdate();
		break;
	case GameState::exit:
		SDL_Event sdlevent;
		sdlevent.type = SDL_QUIT;
		SDL_PushEvent(&sdlevent);
		break;
	}

	m_MouseStates["Moved"] = false;
	m_MouseStates["LeftClick"] = false;
}

void Game::Draw( ) const
{
	ClearBackground( );

	switch (m_GameState)
	{
	case GameState::inMenu:
		m_Menus.find(m_CurrentMenu)->second.Draw();
		break;
	case GameState::inGame:
		glPushMatrix();
			m_GameCamera.ViewTransform();
			m_pDungeon->DrawCave(&m_GameCamera);
			m_pPlayer->Draw();
			m_pDungeon->DrawContent();
		glPopMatrix();

		m_Hud.Draw();
		break;
	case GameState::paused:
		break;
	default:
		break;
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	switch ( e.keysym.sym )
	{
	case SDLK_0:
		if (m_GameState == GameState::inGame)
			LoadGame();
		break;
	case SDLK_i:
		PrintInfo();
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	m_MousePos.x = float(e.x);
	m_MousePos.y = float(GameSettings::GetInstance()->GetGameWindow().height - e.y);

	m_MouseStates["Moved"] = true;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		m_MouseStates["LeftClick"] = true;
		break;
	}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::LoadGame()
{
	if (m_pDungeon)
		delete m_pDungeon;

	m_pDungeon = DungeonGenerator::Generate(Point2f{ 6000.f, 6000.f }, 250.f, 50.f, 200);
	Point2f startPoint{ m_pDungeon->GetEntrancePos() };
	m_pPlayer->SetPos(startPoint);
	m_GameCamera.SetPos(startPoint);

	m_Hud.Update();
}

void Game::PrintInfo()
{
	std::cout << "Zeliora - Top down Dungeon crawler-RPG" << std::endl;
	std::cout << "--------------------------------------" << std::endl;
	std::cout << "Current controls: (In Game)" << std::endl;
	std::cout << "In Game:" << std::endl;
	std::cout << "        Movement			: A, W, S, D" << std::endl;
	std::cout << "        Orientation		: Mouse Movements" << std::endl;
	std::cout << "        Attack			: Mouse Left Button" << std::endl;
	std::cout << "        Open InGame Menu	: Esc" << std::endl;
	std::cout << std::endl;
	std::cout << "In Menu:" << std::endl;
	std::cout << "        Menu navigation		: Mouse" << std::endl; 
	std::cout << "        Menu Button trigger	: Nouse Left Button" << std::endl;
	std::cout << "        Inventory scroll up	: Up Arrow" << std::endl;
	std::cout << "        Inventory scroll down	: Down Arrow" << std::endl;
	std::cout << std::endl;
	std::cout << "Generate new Dungeon: 0" << std::endl;
}

void Game::InGameUpdate(float elapsedSec)
{
	ResourceManager::GetInstance()->PlayMusic("DungeonTheme");

	m_Hud.Update();
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	if (pStates[SDL_SCANCODE_ESCAPE])
	{
		m_GameState = GameState::inMenu;
		m_CurrentMenu = MenuType::inGameMenu;
		m_Menus.find(m_CurrentMenu)->second.Load();
	}
	else
	{
		m_pPlayer->Update(elapsedSec, m_GameCamera.ToWorldPos(m_MousePos), m_pDungeon);
		m_pDungeon->Update(elapsedSec, m_pPlayer);
		m_GameCamera.Track(m_pPlayer, m_pDungeon);

		if (m_pPlayer->IsDead())
			m_GameState = GameState::playerDead;
	}
}

void Game::MenuUpdate()
{
	ResourceManager::GetInstance()->PlayMusic("MenuTheme");

	if (m_MouseStates["Moved"])
	{
		m_Menus.find(m_CurrentMenu)->second.HoverCheck(m_MousePos);
	}

	if (m_MouseStates["LeftClick"])
	{
		m_Menus.find(m_CurrentMenu)->second.ClickCheck(m_MousePos);
	}
}

void Game::EndGameUpdate()
{
	m_GameState = GameState::inMenu;
	m_CurrentMenu = MenuType::mainMenu;
	m_Menus.find(m_CurrentMenu)->second.Load();
	delete m_pPlayer;
	m_pPlayer = nullptr;
	delete m_pDungeon;
	m_pDungeon = nullptr;
}