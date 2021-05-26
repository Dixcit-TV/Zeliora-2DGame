#include "pch.h"
#include "Camera.h"
#include "Hero.h"
#include "Dungeon.h"

const float Camera::m_FreeAreaRatio = 0.33f;

Camera::Camera(const Point2f& center, float halfWidth, float halfHeight)
	: m_Center{ center }
	, m_HalfWidth{ halfWidth }
	, m_HalfHeight{ halfHeight }
{}

void Camera::Track(const Character* pHero, const Dungeon* pDungeon)
{
	const float freeAreaWidth{ m_HalfWidth * m_FreeAreaRatio };
	const float freeAreaHeight{ m_HalfHeight * m_FreeAreaRatio };

	Point2f charMidPoint{ pHero->GetAabb().center };

	if (charMidPoint.x < m_Center.x - freeAreaWidth)
	{
		m_Center.x = charMidPoint.x + freeAreaWidth;
	}
	if (charMidPoint.x > m_Center.x + freeAreaWidth)
	{
		m_Center.x = charMidPoint.x - freeAreaWidth;
	}
	if (charMidPoint.y < m_Center.y - freeAreaHeight)
	{
		m_Center.y = charMidPoint.y + freeAreaHeight;
	}
	if (charMidPoint.y > m_Center.y + freeAreaHeight)
	{
		m_Center.y = charMidPoint.y - freeAreaHeight;
	}

	pDungeon->Clamp(this);
}

void Camera::ViewTransform() const
{
	glTranslatef(-m_Center.x + m_HalfWidth, -m_Center.y + m_HalfHeight, 0.f);
}

Rectp Camera::GetAabb() const
{
	return Rectp{ m_Center, m_HalfWidth, m_HalfHeight };
}

void Camera::SetPos(const Point2f& newPos)
{
	m_Center = newPos;
}

Point2f Camera::ToWorldPos(const Point2f& screenPos)
{
	return Point2f{ screenPos.x + m_Center.x - m_HalfWidth, screenPos.y + m_Center.y - m_HalfHeight };
}