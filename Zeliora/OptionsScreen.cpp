#include "pch.h"
#include "OptionsScreen.h"
#include "GameSettings.h"

OptionsScreen::OptionsScreen()
	: MenuScreen()
	, m_pMusicVolumeTexture{ nullptr }
	, m_pEffectVolumeTexture{ nullptr }
	, m_pMenuTile{ new Texture("Options", ResourceManager::GetInstance()->GetFont(ResourceManager::FontSize::size_36), ResourceManager::GetInstance()->GetFontColor(ResourceManager::FontCategory::classic)) }
	, m_pMusicTextTexture{ new Texture("Music", ResourceManager::GetInstance()->GetFont(ResourceManager::FontSize::size_36), ResourceManager::GetInstance()->GetFontColor(ResourceManager::FontCategory::classic)) }
	, m_pEffectTextTexture{ new Texture("Effects", ResourceManager::GetInstance()->GetFont(ResourceManager::FontSize::size_36), ResourceManager::GetInstance()->GetFontColor(ResourceManager::FontCategory::classic)) }
{
	UpdateMusicSettingTexture();
	UpdateEffectSettingTexture();

	Window gameWindow{ GameSettings::GetInstance()->GetGameWindow() };
	// Button Panel
	m_Buttons.push_back(Button<ButtonPayLoad, MenuScreen>(OnReturnButtonClicked, this, "Return", Point2f{ m_ButtonXOffset, 50.f }));
	m_Buttons.push_back(Button<ButtonPayLoad, MenuScreen>(OnMusicDownButtonClicked, this, new Texture("Resources/SpriteSheets/Misc/DownArrow.png"), nullptr, Point2f{ 400.f, gameWindow.height - 200.f }));
	m_Buttons.push_back(Button<ButtonPayLoad, MenuScreen>(OnMusicUpButtonClicked, this, new Texture("Resources/SpriteSheets/Misc/UpArrow.png"), nullptr, Point2f{ 550.f, gameWindow.height - 200.f }));
	m_Buttons.push_back(Button<ButtonPayLoad, MenuScreen>(OnEffectDownButtonClicked, this, new Texture("Resources/SpriteSheets/Misc/DownArrow.png"), nullptr, Point2f{ 850.f, gameWindow.height - 200.f }));
	m_Buttons.push_back(Button<ButtonPayLoad, MenuScreen>(OnEffectUpButtonClicked, this, new Texture("Resources/SpriteSheets/Misc/UpArrow.png"), nullptr, Point2f{ 1000.f, gameWindow.height - 200.f }));
}

OptionsScreen::~OptionsScreen()
{
	delete m_pMusicVolumeTexture;
	delete m_pEffectVolumeTexture;
	delete m_pMenuTile;
	delete m_pMusicTextTexture;
	delete m_pEffectTextTexture;
}

void OptionsScreen::Draw() const
{
	MenuScreen::Draw();

	Window gameWindow{ GameSettings::GetInstance()->GetGameWindow() };
	m_pMenuTile->Draw(Point2f{ gameWindow.width / 2, gameWindow.height - 100.f });

	float soundYOffset{ gameWindow.height - 190.f };
	m_pMusicTextTexture->Draw(Point2f{ 300.f, soundYOffset });
	m_pEffectTextTexture->Draw(Point2f{ 710.f, soundYOffset });

	m_pMusicVolumeTexture->Draw(Point2f{ 485.f, soundYOffset });
	m_pEffectVolumeTexture->Draw(Point2f{ 935.f, soundYOffset });
}

void OptionsScreen::UpdateMusicSettingTexture(unsigned int delta)
{
	unsigned int oldVolum{ GameSettings::GetInstance()->GetMusicVolume() };
	GameSettings::GetInstance()->IncDecMusicVolume(delta);

	if (!m_pMusicVolumeTexture || oldVolum != GameSettings::GetInstance()->GetMusicVolume())
	{
		delete m_pMusicVolumeTexture;
		m_pMusicVolumeTexture = new Texture(std::to_string(GameSettings::GetInstance()->GetMusicVolume()), ResourceManager::GetInstance()->GetFont(ResourceManager::FontSize::size_36), ResourceManager::GetInstance()->GetFontColor(ResourceManager::FontCategory::classic));
	}
}

void OptionsScreen::UpdateEffectSettingTexture(unsigned int delta)
{
	unsigned int oldVolum{ GameSettings::GetInstance()->GetEffectVolume() };
	GameSettings::GetInstance()->IncDecEffectsVolume(delta);

	if (!m_pEffectVolumeTexture || oldVolum != GameSettings::GetInstance()->GetEffectVolume())
	{
		delete m_pEffectVolumeTexture;
		m_pEffectVolumeTexture = new Texture(std::to_string(GameSettings::GetInstance()->GetEffectVolume()), ResourceManager::GetInstance()->GetFont(ResourceManager::FontSize::size_36), ResourceManager::GetInstance()->GetFontColor(ResourceManager::FontCategory::classic));
	}
}

ButtonPayLoad OptionsScreen::OnMusicUpButtonClicked(MenuScreen* pThisObject)
{
	static_cast<OptionsScreen*>(pThisObject)->UpdateMusicSettingTexture(2);

	return ButtonPayLoad{ false };
}

ButtonPayLoad OptionsScreen::OnMusicDownButtonClicked(MenuScreen* pThisObject)
{
	static_cast<OptionsScreen*>(pThisObject)->UpdateMusicSettingTexture(-2);

	return ButtonPayLoad{ false };
}

ButtonPayLoad OptionsScreen::OnEffectUpButtonClicked(MenuScreen* pThisObject)
{
	static_cast<OptionsScreen*>(pThisObject)->UpdateEffectSettingTexture(2);

	return ButtonPayLoad{ false };
}

ButtonPayLoad OptionsScreen::OnEffectDownButtonClicked(MenuScreen* pThisObject)
{
	static_cast<OptionsScreen*>(pThisObject)->UpdateEffectSettingTexture(-2);

	return ButtonPayLoad{ false };
}

ButtonPayLoad OptionsScreen::OnReturnButtonClicked(MenuScreen* pThisObject)
{
	return ButtonPayLoad{ true, MenuState::back, nullptr };
}