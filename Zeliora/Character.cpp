#include "pch.h"
#include "Character.h"
#include "Sprite.h"
#include "utils.h"
#include "GameSettings.h"

Character::Character(Sprite* pSprite, const Point2f& startPos, float health, float movementSpeed, Attack&& attack)
	: m_pSprite{ pSprite }
	, m_Shape{ }
	, m_Direction{ Direction::down }
	, m_Velocity{ 0.f, 0.f }
	, m_MovementSpeed{ movementSpeed }
	, m_Health{ health }
	, m_Attack{ std::move(attack) }
{
	m_Shape.center = startPos;
	m_Shape.halfWidth = m_pSprite->GetWidth() / 2;
	m_Shape.halfHeight = m_pSprite->GetHeight() / 2;
}

Character::~Character()
{
	delete m_pSprite;
}

const Rectp& Character::GetAabb() const
{
	return m_Shape;
}

const Vector2f& Character::GetVelocity() const
{
	return m_Velocity;
}

void Character::SetPos(const Point2f& newPos)
{
	m_Shape.center = newPos;
}

bool Character::IsDead() const
{
	return m_Health <= 0;
}

void Character::UpdateDirection(const Point2f& focusPoint)
{
	float direction{ atan2f(focusPoint.y - m_Shape.center.y, focusPoint.x - m_Shape.center.x) };
	if (direction <= utils::g_Pi_4 && direction > -utils::g_Pi_4)
		m_Direction = Direction::right;
	else if (direction > utils::g_Pi_4&& direction <= utils::g_Pi - utils::g_Pi_4)
		m_Direction = Direction::up;
	else if (direction > utils::g_Pi - utils::g_Pi_4 || direction <= -utils::g_Pi + utils::g_Pi_4)
		m_Direction = Direction::left;
	else
		m_Direction = Direction::down;
}

bool Character::HandleCollision(const Character* pOther)
{
	Rectp otherAabb{ pOther->GetAabb() };
	if (utils::IsOverlapping(m_Shape, otherAabb))
	{
		Vector2f cTocVector{ m_Shape.center - otherAabb.center };
		float angle{ float(atan2(cTocVector.y, cTocVector.x)) };

		if (angle <= utils::g_Pi_4 && angle > -utils::g_Pi_4)
			m_Shape.center.x = otherAabb.center.x + otherAabb.halfWidth + m_Shape.halfWidth;
		else if (angle > utils::g_Pi_4&& angle <= utils::g_Pi - utils::g_Pi_4)
			m_Shape.center.y = otherAabb.center.y + otherAabb.halfHeight + m_Shape.halfHeight;
		else if (angle > utils::g_Pi - utils::g_Pi_4 || angle <= -utils::g_Pi + utils::g_Pi_4)
			m_Shape.center.x = otherAabb.center.x - otherAabb.halfWidth - m_Shape.halfWidth;
		else
			m_Shape.center.y = otherAabb.center.y - otherAabb.halfHeight - m_Shape.halfHeight;

		return true;
	}

	return false;
}

float Character::GetScaledDamage(float baseDamage) const
{
	return baseDamage * GameSettings::GetInstance()->GetWorldMultiplier();
}