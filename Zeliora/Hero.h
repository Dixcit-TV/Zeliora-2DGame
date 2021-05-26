#pragma once
#include "Character.h"
#include "Inventory.h"
#include "Equipment.h"
#include "Attack.h"

class Dungeon;
class EnemyManager;
class LootManager;
class Object;

class Hero : public Character
{
public:
	static std::vector<Hero*> GetAllHeroes();

	Hero(const Hero& other) = delete;
	Hero(Hero&& other) noexcept = delete;
	Hero& operator =(const Hero& other) = delete;
	Hero& operator =(Hero&& other) noexcept = delete;
	virtual ~Hero() override {};

	int GetLevel() const;
	float GetHealthRatio() const;
	float GetExpToLevelRatio() const;
	const Inventory& GetInventory() const;
	const Equipment& GetEquipment() const;

	void Update(float elapsedSec, const Point2f& mousePos, Dungeon* pDungeon);
	virtual bool HitCheck(const Attack& attack, float damage) override;
	virtual void Draw() const override;
	void LootPickUpCheck(LootManager& lootManager);

	void Equip(size_t inventoryIdx);
	void ExpUp();

	void MenuDraw(const Rectp& shape) const;

private:
	explicit Hero(Sprite* pSprite, const Point2f& startPos, float health, float movementSpeed, Attack&& attack);
	
	float m_MaxHealth;
	int m_Level;
	int m_Exp;
	int m_ExpTarget;
	Inventory m_Inventory;
	Equipment m_Equipment;

	void Move(float elapsedSec, Dungeon* pDungeon);
};

