#pragma once
#include "MenuScreen.h"

class EndMenuScreen : public MenuScreen
{
public:
	explicit EndMenuScreen();
	EndMenuScreen(const EndMenuScreen& other) = delete;
	EndMenuScreen(EndMenuScreen&& other) noexcept = delete;
	EndMenuScreen* operator=(const EndMenuScreen& other) = delete;
	EndMenuScreen* operator=(EndMenuScreen&& other) = delete;
	virtual ~EndMenuScreen() override;

	virtual void Draw() const override;

private:
	Texture* m_pYouDiedTexture;

	static ButtonPayLoad OnReturnButtonClicked(MenuScreen* pThisObject);
};

