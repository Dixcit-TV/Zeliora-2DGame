#include "pch.h"
#include "MainMenuScreen.h"
#include "HeroSelectionScreen.h"
#include "OptionsScreen.h"
#include "GameSettings.h"

MainMenuScreen::MainMenuScreen()
	: MenuScreen()
	, m_pStartImg{ new Texture("Resources/SpriteSheets/Misc/StartImage.png") }
{
	const float yButtonOffset{ 50.f };
	const float buttonsTop{ GameSettings::GetInstance()->GetGameWindow().height - 100.f };
	Point2f pos{ m_ButtonXOffset , buttonsTop };

	m_Buttons.push_back(Button<ButtonPayLoad, MenuScreen>(OnStartNewGameButtonClicked, this, "Start", pos));
	pos.y -= yButtonOffset;
	m_Buttons.push_back(Button<ButtonPayLoad, MenuScreen>(OnOptionsButtonClicked, this, "Options", pos));
	pos.y -= yButtonOffset;
	m_Buttons.push_back(Button<ButtonPayLoad, MenuScreen>(OnCreditsButtonClicked, this, "Credits", pos));
	pos.y -= yButtonOffset;
	m_Buttons.push_back(Button<ButtonPayLoad, MenuScreen>(OnExitButtonClicked, this, "Exit", pos));
}

MainMenuScreen::~MainMenuScreen()
{
	delete m_pStartImg;
}

void MainMenuScreen::Draw() const
{
	MenuScreen::Draw();

	m_pStartImg->Draw(Point2f{ 500.f, 100.f });
}

ButtonPayLoad MainMenuScreen::OnStartNewGameButtonClicked(MenuScreen* pThisObject)
{
	return ButtonPayLoad{ true, MenuState::next, static_cast<void*>(new HeroSelectionScreen()) };
}

ButtonPayLoad MainMenuScreen::OnOptionsButtonClicked(MenuScreen* pThisObject)
{
	return ButtonPayLoad{ true, MenuState::next, static_cast<void*>(new OptionsScreen()) };
}

ButtonPayLoad MainMenuScreen::OnCreditsButtonClicked(MenuScreen* pThisObject)
{
	return ButtonPayLoad{ false, MenuState::next, nullptr };
}

ButtonPayLoad MainMenuScreen::OnExitButtonClicked(MenuScreen* pThisObject)
{
	return ButtonPayLoad{ true, MenuState::back, nullptr };
}