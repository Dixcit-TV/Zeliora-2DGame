#pragma once
#include "Enemy.h"

class SpriteTile;

class Witch : public Enemy
{
public:
	static Witch* Create(const Point2f& startPos, Spawn* pSpawn);
	Witch(const Witch& other) = delete;
	Witch(Witch&& other) noexcept = delete;
	Witch& operator =(const Witch& other) = delete;
	Witch& operator =(Witch&& other) noexcept = delete;
	virtual ~Witch() override {};

	virtual void SeekTarget(const Hero* pHero) override;

private:
	explicit Witch(Sprite* pSprite, const Point2f& startPos, float health, float attackRadius, float attackStrength, float movementSpeed, ElementalType elemType, Spawn* pSpawn, Attack&& attack);

	ElementalType m_ElementalType;
	const float m_RunFromRadius;

	virtual void KickStartAttack(const Point2f& targetPoint) override;
};

