#pragma once
#include "MenuScreen.h"

class Hero;
class Texture;

class HeroInventoryScreen : public MenuScreen
{
public:
	explicit HeroInventoryScreen(Hero* pPlayer);
	HeroInventoryScreen(const HeroInventoryScreen& other) = delete;
	HeroInventoryScreen(HeroInventoryScreen&& other) noexcept = delete;
	HeroInventoryScreen* operator=(const HeroInventoryScreen& other) = delete;
	HeroInventoryScreen* operator=(HeroInventoryScreen&& other) = delete;
	virtual ~HeroInventoryScreen() override;

	virtual ButtonPayLoad ProcessClick(const Point2f& clickPos) override;
	virtual void Draw() const override;

private:
	Texture* m_pTitleTexture;
	Hero* m_pPlayer;
	Rectf m_CharacterBox;
	std::vector<Rectf> m_ObjectSlots;
	std::vector<Rectf> m_EquipmentSlots;

	void DrawEquipment() const;
	void DrawInventory() const;
	static ButtonPayLoad OnReturnButtonClicked(MenuScreen* pThisObject);
};

