#pragma once
#include "MenuScreen.h"

class MainMenuScreen : public MenuScreen
{
public:
	explicit MainMenuScreen();
	virtual ~MainMenuScreen() override;

	virtual void Draw() const override;

private:
	Texture* m_pStartImg;

	static ButtonPayLoad OnStartNewGameButtonClicked(MenuScreen* pThisObject);
	static ButtonPayLoad OnOptionsButtonClicked(MenuScreen* pThisObject);
	static ButtonPayLoad OnCreditsButtonClicked(MenuScreen* pThisObject);
	static ButtonPayLoad OnExitButtonClicked(MenuScreen* pThisObject);
};

