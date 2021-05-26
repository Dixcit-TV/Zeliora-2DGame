#pragma once
class DungeonArea final
{
public:
	enum class DungeonAreaType {
		mainRoom, subRoom
	};

	explicit DungeonArea(const Point2f& center, float halfWidth, float halfHeight, DungeonAreaType type);

	void Draw() const {};
	const Rectp& GetAabb() const;
	Rectp GetInnerAabb(int wallPixelSize) const;
	DungeonAreaType GetType() const;
	Point2f GetRandPoint(int wallPixelSize) const;

protected:

	Rectp m_Area;
	DungeonAreaType m_Type;

	static const int m_WallSize;
};

