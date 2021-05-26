#pragma once
#include "Enemy.h"

class SpriteTile;

class Guard : public Enemy
{
public:
	static Guard* Create(const Point2f& startPos, Spawn* pSpawn);
	Guard(const Guard& other) = delete;
	Guard(Guard&& other) noexcept = delete;
	Guard& operator =(const Guard& other) = delete;
	Guard& operator =(Guard&& other) noexcept = delete;
	virtual ~Guard() override {};

	virtual void SeekTarget(const Hero* pHero) override;

private:
	explicit Guard(Sprite* pSprite, const Point2f& startPos, float health, float attackRadius, float attackStrength, float movementSpeed, Spawn* pSpawn, Attack&& attack);

	virtual void KickStartAttack(const Point2f& targetPoint) override;
};

