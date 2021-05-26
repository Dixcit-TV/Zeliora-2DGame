#include "pch.h"
#include "EndMenuScreen.h"
#include "GameSettings.h"

EndMenuScreen::EndMenuScreen()
	: MenuScreen()
	, m_pYouDiedTexture{ new Texture("You Died !",  ResourceManager::GetInstance()->GetFont(ResourceManager::FontSize::size_72), ResourceManager::GetInstance()->GetFontColor(ResourceManager::FontCategory::classic)) }
{
	m_Buttons.push_back(Button<ButtonPayLoad, MenuScreen>(OnReturnButtonClicked, this, "Exit To Menu", Point2f{ m_ButtonXOffset, 50.f }));
}

EndMenuScreen::~EndMenuScreen()
{
	delete m_pYouDiedTexture;
}

void EndMenuScreen::Draw() const
{
	MenuScreen::Draw();

	Window window{ GameSettings::GetInstance()->GetGameWindow() };
	m_pYouDiedTexture->Draw(Point2f{ window.width / 2, window.height / 2 });
}

ButtonPayLoad EndMenuScreen::OnReturnButtonClicked(MenuScreen* pThisObject)
{
	return ButtonPayLoad{ true, MenuState::exitGame, nullptr };
}