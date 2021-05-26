#pragma once
#include "MenuScreen.h"

class Hero;

class InGameMenuScreen : public MenuScreen
{
public:
	explicit InGameMenuScreen(Hero* pPlayer);
	virtual ~InGameMenuScreen() override {};

	virtual void Draw() const override;

private:
	Hero* m_pPlayer;

	static ButtonPayLoad OnContinueButtonClicked(MenuScreen* pThisObject);
	static ButtonPayLoad OnHeroInventoryClicked(MenuScreen* pThisObject);
	static ButtonPayLoad OnOptionsButtonClicked(MenuScreen* pThisObject);
	static ButtonPayLoad OnExitButtonClicked(MenuScreen* pThisObject);
};

