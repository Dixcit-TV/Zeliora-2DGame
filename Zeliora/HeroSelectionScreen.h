#pragma once
#include <vector>
#include "Texture.h"
#include "Hero.h"
#include "MenuScreen.h"

class HeroSelectionScreen : public MenuScreen
{
public:
	explicit HeroSelectionScreen();
	HeroSelectionScreen(const HeroSelectionScreen& other) = delete;
	HeroSelectionScreen(HeroSelectionScreen&& other) noexcept = delete;
	HeroSelectionScreen& operator =(const HeroSelectionScreen& other) = delete;
	HeroSelectionScreen& operator =(HeroSelectionScreen&& other) noexcept = delete;
	virtual ~HeroSelectionScreen() override;

	virtual ButtonPayLoad ProcessClick(const Point2f& clickPos) override;
	virtual void Draw() const override;

private:
	std::vector<Hero*> m_pHeroes;
	int m_SelectedHeroIdx;
	Point2f m_BotLeft;

	Rectf GetHeroPosInMenu(int idx) const;

	static ButtonPayLoad OnStartButtonClicked(MenuScreen* pThisObject);
	static ButtonPayLoad OnReturnButtonClicked(MenuScreen* pThisObject);
};

