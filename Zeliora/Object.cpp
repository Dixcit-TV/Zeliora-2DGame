#include "pch.h"
#include "Object.h"
#include "utils.h"
#include "SpriteTile.h"
#include "Texture.h"
#include "ResourceManager.h"

Object::Object(const SpriteTile* pSpriteTile, const std::string& name, ObjectType type)
	: m_pSpriteTile{ pSpriteTile }
	, m_pObjectTextDescription{ nullptr }
	, m_Name{ name }
	, m_Type{ type }
{}

Object::~Object()
{
	delete m_pSpriteTile;
	delete m_pObjectTextDescription;
};

const SpriteTile* Object::GetSpriteTile() const
{
	return m_pSpriteTile;
}

Object::ObjectType Object::GetType() const
{
	return m_Type;
}

void Object::Draw(const Point2f& centerPoint, float size) const
{
	m_pSpriteTile->Draw(centerPoint, size);
}

void Object::DrawWithDescription(const Point2f& topLeft, float size) const
{
	const float offset{ 10.f };
	float width{ size }, height{ size };

	if (size <= 0.f)
	{
		width = m_pSpriteTile->GetFrameWidth();
		height = m_pSpriteTile->GetFrameHeight();
	}

	m_pSpriteTile->Draw(Point2f{ topLeft.x + width / 2, topLeft.y - height / 2 }, size);
	Point2f DescriptionPos{ };
	DescriptionPos.x = topLeft.x + width + offset;
	DescriptionPos.y = topLeft.y - m_pObjectTextDescription->GetHeight();
	m_pObjectTextDescription->Draw(DescriptionPos);
}