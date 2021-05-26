#include "pch.h"
#include "SpriteTile.h"
#include "SpriteSheet.h"

SpriteTile::SpriteTile(const SpriteSheet* pSpriteSheet, int row, int col)
	: m_pSpriteSheet{ pSpriteSheet }
	, m_Row{ row }
	, m_Col{ col }
{}


void SpriteTile::Draw(const Point2f& centerPos, float size) const
{
	Rectf src{ };
	src.width = m_pSpriteSheet->GetFrameWidth();
	src.height = m_pSpriteSheet->GetFrameHeight();
	src.left = src.width * m_Col;
	src.bottom = src.height * (m_Row + 1);

	Rectf dest{ centerPos.x - src.width / 2, centerPos.y - src.height / 2, src.width, src.height };

	if (size > 0.f)
	{
		dest.left = centerPos.x - size / 2;
		dest.bottom = centerPos.y - size / 2;
		dest.width = size;
		dest.height = size;
	}

	m_pSpriteSheet->Draw(dest, src);
}

float SpriteTile::GetFrameWidth() const
{
	return m_pSpriteSheet->GetFrameWidth();
}

float SpriteTile::GetFrameHeight() const
{
	return m_pSpriteSheet->GetFrameWidth();
}