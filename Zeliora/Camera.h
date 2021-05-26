#pragma once

class Character;
class Dungeon;

class Camera final
{
public:
	explicit Camera(const Point2f& botLeft, float width, float height);

	void Track(const Character* pHero, const Dungeon* pDungeon);
	void ViewTransform() const;

	Rectp GetAabb() const;
	void SetPos(const Point2f& newPos);
	Point2f ToWorldPos(const Point2f& screenPos); 

private:
	Point2f m_Center{};
	float m_HalfWidth;
	float m_HalfHeight;

	static const float m_FreeAreaRatio;
};

