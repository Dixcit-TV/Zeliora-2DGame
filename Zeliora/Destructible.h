#pragma once
#include "LootManager.h"

class Sprite;
class Attack;

class Destructible final
{
public:
	explicit Destructible(const Rectp& shape);
	Destructible(const Destructible& other) = delete;
	Destructible& operator =(const Destructible& other) = delete;
	Destructible(Destructible&& other) noexcept;
	Destructible& operator =(Destructible&& other) noexcept = delete;
	~Destructible();

	bool IsDestroyed() const;
	const Point2f& GetPos() const;
	bool HitCheck(const Attack& attack);
	void Update(float elapsedSec, LootManager& lootManager);
	void Draw() const;

private:
	enum class DestructibleState {
		set, hit, destroyed
	};

	Sprite* m_pSprite;
	const Rectp m_Shape;
	DestructibleState m_State;
};

