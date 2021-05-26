#pragma once
#include <vector>
#include "Character.h"
#include <queue>
#include <stack>

class World;
class Hero;
class Spawn;
class Dungeon;

class Enemy : public Character
{
public:
	Enemy(const Enemy& other) = delete;
	Enemy(Enemy&& other) noexcept = delete;
	Enemy& operator =(const Enemy& other) = delete;
	Enemy& operator =(Enemy&& other) noexcept = delete;
	virtual ~Enemy() override;

	virtual bool HitCheck(const Attack& attack, float damage) override;
	virtual bool HandleCollision(const Character* pOther) override;
	virtual void Draw() const override;

	virtual bool IsDead() const override;
	Spawn* GetOriginSpawn() const;
	void Update(float elapsedSec, const Dungeon* pDungeon, Hero* pHero);

	enum class EnemyState
	{
		wait, move, follow, inrange, runaway, attack, dying, dead
	};


protected:
	explicit Enemy(Sprite* pSprite, const Point2f& startPos, float health, float attackRadius, float attackStrength, float movementSpeed, float seakingRadius, float rangeRadius, Spawn* pSpawn, Attack&& attack, const SpriteTile* pWeaponSpriteTile);

	Sprite* m_DeathSprite;
	EnemyState m_State;
	std::stack<Point2f> m_MovePath;
	Spawn* m_OriginSpawn;
	const float m_MoveDelay{ 2.f };
	float m_MoveTimer;
	const SpriteTile* m_pWeaponSpriteTile;

	float m_AttackRadius;
	float m_AttackStrength;
	float m_AttackDelayDuration;
	float m_AttackDelayTimer;

	const float m_SeekingRadius;
	const float m_RangeRadius;

	const float m_DirectionDelay{ 0.1f };
	float m_DirectionTimer;

	void Move(float elapsedSec, const Dungeon* pDungeon, Hero* pHero);
	void ApplyMovement(float elapsedSec, float direction, const Dungeon* pDungeon, Hero* pHero);
	virtual void SeekTarget(const Hero* pHero) = 0;
	virtual void KickStartAttack(const Point2f& targetPoint) = 0;
};

