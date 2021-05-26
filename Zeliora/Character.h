#pragma once
#include "Vector2f.h"
#include "Attack.h"

class Sprite;
class World;
class Dungeon;

class Character
{
public:
	virtual ~Character();

	const Rectp& GetAabb() const;
	const Vector2f& GetVelocity() const;

	virtual bool HitCheck(const Attack& attack, float damage) = 0;
	virtual bool HandleCollision(const Character* pOther);
	virtual void Draw() const = 0;
	virtual bool IsDead() const;

	void SetPos(const Point2f& newPos);

protected:
	explicit Character(Sprite* pSprite, const Point2f& startPos, float health, float movementSpeed, Attack&& attack);

	Sprite* m_pSprite;
	Rectp m_Shape;
	Direction m_Direction;
	Vector2f m_Velocity;
	float m_MovementSpeed;
	float m_Health;
	Attack m_Attack;

	//I had to create a static getter for this because a static const std::string seems to confuse the HeapControl which flags it as a memory leak
	static std::string GetXmlPath() { return "Resources/Data/Characters.xml"; };

	void UpdateDirection(const Point2f& focusPoint);
	float GetScaledDamage(float baseDamage) const;
};