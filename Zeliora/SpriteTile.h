#pragma once

class SpriteSheet;

class SpriteTile
{
public:
	explicit SpriteTile(const SpriteSheet* pSpriteSheet, int row, int col);

	void Draw(const Point2f& centerPos, float size = 0.f) const;
	float GetFrameWidth() const;
	float GetFrameHeight() const;

private:
	const SpriteSheet* m_pSpriteSheet;
	const int m_Row;
	const int m_Col;
};

