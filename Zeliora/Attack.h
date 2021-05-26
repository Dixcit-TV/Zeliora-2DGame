#pragma once

class SpriteTile;
class Sprite;

enum class AttackState
{
	inactive, triggered, done, hit, miss, indelay
};

class Attack final
{
public:
	explicit Attack(Sprite* pImpactSprite, float duration);
	Attack(const Attack& other) = delete;
	Attack(Attack&& other) noexcept;
	Attack& operator =(const Attack& other) = delete;
	Attack& operator =(Attack&& other) noexcept = delete;
	~Attack();
	
	bool IsInProcess() const;
	bool IsInDoneState() const;
	void SetStateFromHitResult(bool hasHit);
	Circlef GetImpactArea() const;

	void Start(const Point2f& impactPoint, float radius, float directionRad);
	void Update(float elapsedSec);
	void Draw(const SpriteTile* pWeapon, const Point2f& clampingPoint) const;

private:
	Point2f m_ImpactPoint;
	float m_Radius;
	float m_Direction;
	AttackState m_State;
	const float m_Duration;
	float m_Timer;
	bool m_HitSoundPlayed;
	const float m_AvailabilityDelay {0.1f};
	float m_AvailabilityDelayTimer;

	Sprite* m_pImpactSprite;
};

