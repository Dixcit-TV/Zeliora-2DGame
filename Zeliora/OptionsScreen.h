#pragma once
#include "MenuScreen.h"
#include "SpriteTile.h"

class OptionsScreen : public MenuScreen
{
public:
	explicit OptionsScreen();
	OptionsScreen(const OptionsScreen& other) = delete;
	OptionsScreen(OptionsScreen&& other) noexcept = delete;
	OptionsScreen* operator=(const OptionsScreen& other) = delete;
	OptionsScreen* operator=(OptionsScreen&& other) = delete;
	virtual ~OptionsScreen() override;

	virtual void Draw() const override;

private:
	Texture* m_pMusicVolumeTexture;
	Texture* m_pEffectVolumeTexture;
	Texture* m_pMenuTile;
	Texture* m_pMusicTextTexture;
	Texture* m_pEffectTextTexture;

	void UpdateMusicSettingTexture(unsigned int delta = 0);
	void UpdateEffectSettingTexture(unsigned int delta = 0);

	static ButtonPayLoad OnMusicUpButtonClicked(MenuScreen* pThisObject);
	static ButtonPayLoad OnMusicDownButtonClicked(MenuScreen* pThisObject);
	static ButtonPayLoad OnEffectUpButtonClicked(MenuScreen* pThisObject);
	static ButtonPayLoad OnEffectDownButtonClicked(MenuScreen* pThisObject);
	static ButtonPayLoad OnReturnButtonClicked(MenuScreen* pThisObject);
};