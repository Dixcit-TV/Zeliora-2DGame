#pragma once
#include "MenuScreen.h"
#include <stack>

class Hero;

class Menu final
{
public:
	explicit Menu(MenuType type, GameState* const pGameState, Hero** pHero);
	Menu(const Menu& other) = delete;
	Menu(Menu&& other) noexcept;
	Menu& operator =(const Menu& other) = delete;
	Menu& operator =(Menu&& other) noexcept = delete;
	~Menu();

	void Draw() const;

	void HoverCheck(const Point2f& mousePos);
	void ClickCheck(const Point2f& clickPos);

	void Load();

private:
	std::stack<MenuScreen*> m_pMenuScreens;
	MenuType m_MenuType;
	GameState* const m_pGameState;
	Hero** m_pHero;

	void Clear();
};

