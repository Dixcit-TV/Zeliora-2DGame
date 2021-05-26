#include "pch.h"
#include "DungeonArea.h"
#include "DungeonGenerator.h"
#include "utils.h"

DungeonArea::DungeonArea(const Point2f& center, float halfWidth, float halfHeight, DungeonAreaType type)
	: m_Area{ center, halfWidth, halfHeight }
	, m_Type{ type }
{}

const Rectp& DungeonArea::GetAabb() const
{
	return m_Area;
}

Rectp DungeonArea::GetInnerAabb(int wallPixelSize) const
{
	return Rectp{ m_Area.center, m_Area.halfWidth - wallPixelSize, m_Area.halfHeight - wallPixelSize };
}

DungeonArea::DungeonAreaType DungeonArea::GetType() const
{
	return m_Type;
}

Point2f DungeonArea::GetRandPoint(int wallPixelSize) const
{
	float randX{ utils::GetRand(m_Area.center.x + m_Area.halfWidth - wallPixelSize, m_Area.center.x - m_Area.halfWidth + wallPixelSize) };
	float randY{ utils::GetRand(m_Area.center.y + m_Area.halfHeight - wallPixelSize, m_Area.center.y - m_Area.halfHeight + wallPixelSize) };

	return Point2f{ randX, randY };
}