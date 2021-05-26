#pragma once
#include <vector>
#include <map>
#include "Camera.h"
#include "LootManager.h"
#include "Hud.h"
#include "Menu.h"

class Dungeon;
class Spawn;
class Object;
class Sprite;
class SpriteSheet;

class Game final
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw() const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	Point2f m_MousePos;
	Hero* m_pPlayer;
	Dungeon* m_pDungeon;
	Camera m_GameCamera;
	Hud m_Hud;
	GameState m_GameState;
	std::unordered_map<MenuType, Menu> m_Menus;
	MenuType m_CurrentMenu;
	std::map<std::string, bool> m_MouseStates;

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;

	void LoadGame();
	void PrintInfo();
	void InGameUpdate(float elapsedSec);
	void MenuUpdate();
	void EndGameUpdate();
};