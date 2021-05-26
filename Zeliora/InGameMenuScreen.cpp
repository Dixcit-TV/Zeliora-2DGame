#include "pch.h"
#include "InGameMenuScreen.h"
#include "HeroInventoryScreen.h"
#include "OptionsScreen.h"
#include "GameSettings.h"

InGameMenuScreen::InGameMenuScreen(Hero* pPlayer)
	: MenuScreen()
	, m_pPlayer{ pPlayer }
{
	const float yButtonOffset{ 50.f };
	const float buttonsTop{ GameSettings::GetInstance()->GetGameWindow().height - 100.f };
	Point2f pos{ m_ButtonXOffset , buttonsTop };

	m_Buttons.push_back(Button<ButtonPayLoad, MenuScreen>(OnContinueButtonClicked, this, "Continue", pos));
	pos.y -= yButtonOffset;
	m_Buttons.push_back(Button<ButtonPayLoad, MenuScreen>(OnHeroInventoryClicked, this, "Inventory & Equipment", pos));
	pos.y -= yButtonOffset;
	m_Buttons.push_back(Button<ButtonPayLoad, MenuScreen>(OnOptionsButtonClicked, this, "Options", pos));
	pos.y -= yButtonOffset;
	m_Buttons.push_back(Button<ButtonPayLoad, MenuScreen>(OnExitButtonClicked, this, "Exit To Menu", pos));
}

void InGameMenuScreen::Draw() const
{
	MenuScreen::Draw();
}

ButtonPayLoad InGameMenuScreen::OnContinueButtonClicked(MenuScreen* pThisObject)
{
	return ButtonPayLoad{ true, MenuState::back, nullptr };
}

ButtonPayLoad InGameMenuScreen::OnHeroInventoryClicked(MenuScreen* pThisObject)
{
	InGameMenuScreen* castedThis{ static_cast<InGameMenuScreen*>(pThisObject) };
	return ButtonPayLoad{ true, MenuState::next, static_cast<void*>(new HeroInventoryScreen(castedThis->m_pPlayer)) };
}

ButtonPayLoad InGameMenuScreen::OnOptionsButtonClicked(MenuScreen* pThisObject)
{
	return ButtonPayLoad{ true, MenuState::next, static_cast<void*>(new OptionsScreen()) };
}

ButtonPayLoad InGameMenuScreen::OnExitButtonClicked(MenuScreen* pThisObject)
{
	return ButtonPayLoad{ true, MenuState::exitGame, nullptr };
}