#include "pch.h"
#include "Sprite.h"
#include "SpriteSheet.h"

Sprite::Sprite(const SpriteSheet* pSpriteSheet, int startFrameOffset, int numCols, int numRows, int startColumn, int startRow, float frameTime)
	: m_pSpriteSheet{ pSpriteSheet }
	, m_CurrFrame{ startFrameOffset }
	, m_NumColumns{ numCols }
	, m_NumRows{ numRows }
	, m_StartColumn{ startColumn }
	, m_StartRow{ startRow }
	, m_FrameTime{ frameTime }
	, m_ElapsedTime{ 0.f }
	, m_IsDone{ false }
{}

float Sprite::GetWidth() const
{
	return m_pSpriteSheet->GetFrameWidth();
}

float Sprite::GetHeight() const
{
	return m_pSpriteSheet->GetFrameHeight();
}

bool Sprite::IsDone() const
{
	return m_IsDone;
}

void Sprite::NextFrame(float elapsedSec, bool repeat)
{
	if (!repeat && m_CurrFrame == m_NumColumns - 1)
	{
		m_IsDone = true;
		return;
	}

	m_ElapsedTime += elapsedSec;
	if (m_ElapsedTime >= m_FrameTime)
	{
		++m_CurrFrame %= m_NumColumns;
		m_ElapsedTime -= m_FrameTime;
	}
}

void Sprite::Draw(const Rectp& shape, int row) const
{
	Rectf src{ };
	src.width = m_pSpriteSheet->GetFrameWidth();
	src.height = m_pSpriteSheet->GetFrameHeight();
	src.left = src.width * (m_StartColumn + m_CurrFrame);
	src.bottom = src.height * (m_StartRow + row + 1);

	Rectf dest{ shape.center.x - shape.halfWidth, shape.center.y - shape.halfHeight, shape.halfWidth * 2, shape.halfHeight * 2 };

	m_pSpriteSheet->Draw(dest, src);
}

void Sprite::ResetAnimation()
{
	m_CurrFrame = 0;
	m_ElapsedTime = 0.f;
	m_IsDone = false;
}