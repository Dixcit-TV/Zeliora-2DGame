#pragma once
#include <vector>
#include "Button.h"

class MenuScreen;

struct ButtonPayLoad
{
	bool clicked;
	MenuState menuState;
	void* pArg;
};

class MenuScreen
{
public:
	virtual ~MenuScreen() {};

	virtual void ProcessHover(const Point2f& clickPos);
	virtual ButtonPayLoad ProcessClick(const Point2f& clickPos);
	virtual void Draw() const;

protected:
	explicit MenuScreen();

	float m_ButtonBoxWidth;
	const static float m_ButtonXOffset;

	std::vector<Button<ButtonPayLoad, MenuScreen>> m_Buttons;
	float GetLongestButtonWidth() const;
	float GetButtonBoxWidth() const;
};
