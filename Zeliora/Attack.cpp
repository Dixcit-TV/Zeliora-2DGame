#include "pch.h"
#include "Attack.h"
#include "SpriteTile.h"
#include "Sprite.h"
#include "Weapon.h"
#include "utils.h"
#include "ResourceManager.h"
#include <iostream>

Attack::Attack(Sprite* pImpactSprite, float duration)
	: m_ImpactPoint{}
	, m_Radius{ 0.f }
	, m_Direction{ 0.f }
	, m_pImpactSprite{ pImpactSprite }
	, m_State{ AttackState::inactive }
	, m_Duration{ duration }
	, m_Timer{ 0.f }
	, m_HitSoundPlayed{ false }
	, m_AvailabilityDelayTimer{ 0.f }
{}

Attack::Attack(Attack&& other) noexcept
	: m_ImpactPoint{ other.m_ImpactPoint }
	, m_Radius{ other.m_Radius }
	, m_Direction{ other.m_Direction }
	, m_pImpactSprite{ other.m_pImpactSprite }
	, m_State{ other.m_State }
	, m_Duration{ other.m_Duration }
	, m_Timer{ other.m_Timer }
	, m_HitSoundPlayed{ other.m_HitSoundPlayed }
	, m_AvailabilityDelayTimer{ other.m_AvailabilityDelayTimer }
{
	other.m_pImpactSprite = nullptr;
}

Attack::~Attack()
{
	delete m_pImpactSprite;
}

bool Attack::IsInProcess() const
{
	return m_State != AttackState::inactive;
}

bool Attack::IsInDoneState() const
{
	return m_State == AttackState::done;
}

void Attack::SetStateFromHitResult(bool hasHit)
{
	if (hasHit)
		m_State = AttackState::hit;
	else
		m_State = AttackState::miss;
}

Circlef Attack::GetImpactArea() const
{
	return Circlef{ m_ImpactPoint, m_Radius };
}

void Attack::Start(const Point2f& impactPoint, float radius, float directionRad)
{
	if (!IsInProcess())
	{
		m_State = AttackState::triggered;
		m_Direction = directionRad * 180 / utils::g_Pi;
		m_ImpactPoint = impactPoint;
		m_Radius = radius;
	}
}

void Attack::Update(float elapsedSec)
{
	switch (m_State)
	{
	case AttackState::triggered: 
		m_Timer += elapsedSec;
		if (m_Timer >= m_Duration)
		{
			m_State = AttackState::done;
		}
		break;
	case AttackState::done:
		break;
	case AttackState::hit:
		m_pImpactSprite->NextFrame(elapsedSec, false);

		if (!m_HitSoundPlayed)
		{
			ResourceManager::GetInstance()->PlaySoundEffect("AttackHit");
			m_HitSoundPlayed = true;
		}

		if (m_pImpactSprite->IsDone())
		{
			m_State = AttackState::indelay;
		}
		break;
	case AttackState::miss:
		m_State = AttackState::indelay;
		break;
	case AttackState::indelay:
		m_AvailabilityDelayTimer += elapsedSec;
		if (m_AvailabilityDelayTimer >= m_AvailabilityDelay)
		{
			m_AvailabilityDelayTimer = 0.f;
			m_State = AttackState::inactive;
			m_Timer = 0.f;
			m_HitSoundPlayed = false;
			m_pImpactSprite->ResetAnimation();
		}
		break;
	}
}

void Attack::Draw(const SpriteTile* pWeaponSpriteTile, const Point2f& clampingPoint) const
{
	switch (m_State)
	{
	case AttackState::triggered:
		{
			float spriteAngleAdjsutement{ 180.f };
			float completeSwingAngle{ 90.f };
			float weaponHalfSize{ 24.f };
			const float swingAngle{ (completeSwingAngle / m_Duration) * m_Timer };
			float rotation{ spriteAngleAdjsutement + m_Direction + swingAngle };

			glPushMatrix();
			glTranslatef(clampingPoint.x, clampingPoint.y, 0.f);
			glRotatef(rotation, 0.f, 0.f, 1.f);
			glTranslatef(-weaponHalfSize, weaponHalfSize, 0.f);

			pWeaponSpriteTile->Draw(Point2f{}, weaponHalfSize * 2);
			glPopMatrix();
		}
		break;
	case AttackState::hit:
		m_pImpactSprite->Draw(Rectp{ m_ImpactPoint, m_Radius, m_Radius });
		break;
	}
}