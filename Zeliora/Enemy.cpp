#include "pch.h"
#include <vector>
#include "Enemy.h"
#include "utils.h"
#include "Sprite.h"
#include "Hero.h"
#include "Dungeon.h"
#include "SpriteTile.h"
#include "ResourceManager.h"
#include "GameSettings.h"

Enemy::Enemy(Sprite* pSprite, const Point2f& startPos, float health, float attackRadius, float attackStrength, float movementSpeed, float seakingRadius, float rangeRadius, Spawn* pSpawn, Attack&& attack, const SpriteTile* pWeaponSpriteTile)
	: Character(pSprite, startPos, health * GameSettings::GetInstance()->GetWorldMultiplier(), movementSpeed, std::move(attack))
	, m_DeathSprite{ new Sprite(ResourceManager::GetInstance()->GetSpriteSheet("Misc/Particules.png"), 0, 7, 6, 0, 5, 0.1f) }
	, m_MovePath{}
	, m_OriginSpawn{ pSpawn }
	, m_SeekingRadius{ seakingRadius }
	, m_RangeRadius{ rangeRadius }
	, m_MoveTimer{ 0.f }
	, m_pWeaponSpriteTile{ pWeaponSpriteTile }
	, m_AttackRadius{ attackRadius }
	, m_AttackStrength{ GetScaledDamage(attackStrength) }
	, m_AttackDelayDuration{ utils::GetRand(1.5f)}
	, m_AttackDelayTimer{ 0.f }
	, m_State{ EnemyState::wait }
	, m_DirectionTimer{}
{}

Enemy::~Enemy()
{
	delete m_pWeaponSpriteTile;
	delete m_DeathSprite;
}

void Enemy::Update(float elapsedSec, const Dungeon* pDungeon, Hero* pHero)
{
	if (m_State == EnemyState::dying)
	{
		m_DeathSprite->NextFrame(elapsedSec, false);
		if (m_DeathSprite->IsDone())
			m_State = EnemyState::dead;
	}
	else
	{
		SeekTarget(pHero);
		Move(elapsedSec, pDungeon, pHero);

		if (!m_Attack.IsInProcess())
		{
			if (m_State == EnemyState::inrange)
			{
				m_AttackDelayTimer += elapsedSec;
				if (m_AttackDelayTimer >= m_AttackDelayDuration)
				{
					KickStartAttack(pHero->GetAabb().center);
				}
			}
		}
		else {
			m_Attack.Update(elapsedSec);

			if (m_Attack.IsInDoneState())
			{
				m_Attack.SetStateFromHitResult(pHero->HitCheck(m_Attack, m_AttackStrength));
			}

			if (!m_Attack.IsInProcess())
			{
				m_AttackDelayDuration = utils::GetRand(1.5f);
				m_AttackDelayTimer = 0.f;
			}
		}
	}
}

void Enemy::Move(float elapsedSec, const Dungeon* pDungeon, Hero* pHero)
{
	float squareLambda{ 25.f };
	float direction{ INFINITY };

	switch (m_State)
	{
	case EnemyState::wait:
		m_MoveTimer += elapsedSec;
		if (m_MoveTimer >= m_MoveDelay)
		{
			float randDistance{ utils::GetRand(m_SeekingRadius, m_RangeRadius) };
			Point2f newDest = pDungeon->GetRandCenterInRadius(Circlef{ m_Shape.center, randDistance }, m_Shape.halfWidth, m_Shape.halfHeight, true);
			m_MovePath = pDungeon->FindPathToDestination(m_Shape.center, newDest);
			m_State = EnemyState::move;
			m_MoveTimer = 0.f;
		}
		break;
	case EnemyState::follow:
		if (m_MovePath.empty())
		{
			m_MovePath = pDungeon->FindPathToDestination(m_Shape.center, pHero->GetAabb().center);
		}

		if ((m_MovePath.top() - m_Shape.center).SquaredLength() < squareLambda)
		{
			m_MovePath.pop();
		}
		else
		{
			direction = std::atan2f(m_MovePath.top().y - m_Shape.center.y, m_MovePath.top().x - m_Shape.center.x);
			float playerDirection{ std::atan2f(pHero->GetAabb().center.y - m_Shape.center.y, pHero->GetAabb().center.x - m_Shape.center.x) };

			if (abs((direction + utils::g_Pi) - (playerDirection + utils::g_Pi)) > utils::g_Pi_4)
				m_MovePath = pDungeon->FindPathToDestination(m_Shape.center, pHero->GetAabb().center);
		}
		break;
	case EnemyState::inrange:
		while (!m_MovePath.empty())
		{
			m_MovePath.pop();
		}

		UpdateDirection(pHero->GetAabb().center);
		break;
	case EnemyState::runaway:
		direction = std::atan2f(pHero->GetAabb().center.y - m_Shape.center.y, pHero->GetAabb().center.x - m_Shape.center.x);
		direction += utils::g_Pi;
		break;
	case EnemyState::move:
		if (m_MovePath.empty())
			m_State = EnemyState::wait;
		else
		{
			if ((m_MovePath.top() - m_Shape.center).SquaredLength() < squareLambda)
			{
				m_MovePath.pop();
			}
			else
			{
				direction = std::atan2f(m_MovePath.top().y - m_Shape.center.y, m_MovePath.top().x - m_Shape.center.x);
			}
		}
		break;
	}

	if (direction != INFINITY)
	{
		ApplyMovement(elapsedSec, direction, pDungeon, pHero);
	}
	else
	{
		m_pSprite->ResetAnimation();
	}
}

void Enemy::ApplyMovement(float elapsedSec, float direction, const Dungeon* pDungeon, Hero* pHero)
{
	m_Velocity.x = m_MovementSpeed * cos(direction);
	m_Velocity.y = m_MovementSpeed * sin(direction);

	m_DirectionTimer += elapsedSec;
	if (m_DirectionTimer >= m_DirectionDelay)
	{
		UpdateDirection(Point2f{ m_Shape.center.x + m_Velocity.x, m_Shape.center.y + m_Velocity.y });

		m_DirectionTimer = 0.f;
	}

	m_Shape.center.x += m_Velocity.x * elapsedSec;
	m_Shape.center.y += m_Velocity.y * elapsedSec;

	m_pSprite->NextFrame(elapsedSec);

	for (const Enemy* pEnemy : pDungeon->GetEnemyManager())
	{
		if (pEnemy != this)
			HandleCollision(pEnemy);
	}

	HandleCollision(pHero);

	pDungeon->Clamp(this, elapsedSec);
}

bool Enemy::HandleCollision(const Character* pOther)
{
	bool hadCollision{ Character::HandleCollision(pOther) };

	if (hadCollision)
	{
		while (!m_MovePath.empty())
		{
			m_MovePath.pop();
		}

		m_State = EnemyState::wait;
		m_Velocity.x = m_Velocity.y = 0.f;
	}

	return hadCollision;
}

void Enemy::Draw() const
{
	if (m_State == EnemyState::dying)
		m_DeathSprite->Draw(m_Shape);
	else
	{
		m_pSprite->Draw(m_Shape, int(m_Direction));

		if (m_Attack.IsInProcess())
			m_Attack.Draw(m_pWeaponSpriteTile, m_Shape.center);
	}
}

Spawn* Enemy::GetOriginSpawn() const
{
	return m_OriginSpawn;
}

bool Enemy::HitCheck(const Attack& attack, float damage)
{
	if (utils::IsOverlapping(m_Shape, attack.GetImpactArea()))
	{
		m_Health -= damage;

		if (m_Health <= 0)
		{
			m_State = EnemyState::dying;
			return false;
		}

		return true;
	}

	return false;
}

bool Enemy::IsDead() const
{
	return m_State == EnemyState::dead;
}