#pragma once
#include <string>

class SpriteSheet;

class Sprite
{
public:
	explicit Sprite(const SpriteSheet* pSpriteSheet, int startFrameOffset, int numCols, int numRows, int startColumn, int startRow, float frameTime);

	float GetWidth() const;
	float GetHeight() const;
	bool IsDone() const;

	void NextFrame(float elapsedSec, bool repeat = true);
	void Draw(const Rectp& shape, int row = 0) const;
	void ResetAnimation();

private:
	const SpriteSheet* m_pSpriteSheet;
	int m_CurrFrame;
	int m_NumColumns;
	int m_NumRows;
	int m_StartColumn;
	int m_StartRow;
	float m_FrameTime;
	float m_ElapsedTime;
	bool m_IsDone;
};