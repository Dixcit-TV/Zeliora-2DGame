#include "pch.h"
#include "MenuScreen.h"
#include <algorithm>

const float MenuScreen::m_ButtonXOffset = 50.f;

MenuScreen::MenuScreen()
	: m_Buttons{}
	, m_ButtonBoxWidth{ -1 }
{}

void MenuScreen::Draw() const
{
	Rectf buttonBox{ 0.f, 0.f, GetButtonBoxWidth(), 720.f };
	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	utils::DrawRect(buttonBox, 2.f);

	for (const Button<ButtonPayLoad, MenuScreen>& button : m_Buttons)
	{
		button.Draw();
	}
}

ButtonPayLoad MenuScreen::ProcessClick(const Point2f& clickPos)
{
	for (Button<ButtonPayLoad, MenuScreen>& button : m_Buttons)
	{
		if (button.IsClicked(clickPos))
			return button();
	}

	return ButtonPayLoad{ false };
}

void MenuScreen::ProcessHover(const Point2f& clickPos)
{
	for (Button<ButtonPayLoad, MenuScreen>& button : m_Buttons)
	{
		if (button.IsFocused(clickPos))
			return;
	}
}

float MenuScreen::GetLongestButtonWidth() const
{
	if (m_Buttons.size() == 0)
		return 0.f;

	auto maxWidthPred = [](const Button<ButtonPayLoad, MenuScreen>& button1, const Button<ButtonPayLoad, MenuScreen>& button2) { return button1.GetWidth() < button2.GetWidth(); };
	return std::max_element(m_Buttons.cbegin(), m_Buttons.cend(), maxWidthPred)->GetWidth();
}

float MenuScreen::GetButtonBoxWidth() const
{
	return GetLongestButtonWidth() + m_ButtonXOffset * 2;
}