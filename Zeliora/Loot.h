#pragma once
#include "Object.h"

class Loot final
{
public:
	explicit Loot(const Object* pObject, const Point2f& centerPos);
	Loot(const Loot& other) = delete;
	Loot& operator =(const Loot& other) = delete;
	Loot(Loot&& other) noexcept;
	Loot& operator =(Loot&& other) noexcept;
	~Loot();

	const Object* ReleaseObject();
	const Point2f& GetPos() const;
	bool IsPickedUp() const;

	void Draw() const;

private:
	const Object* m_pObject;
	Point2f m_CenterPos;
};

