#pragma once
#include "Object.h"

class Weapon final : public Object
{
public:
	static Weapon* Create(bool useDefault = false);
	Weapon(const Weapon& other) = delete;
	Weapon(Weapon&& other) noexcept = delete;
	Weapon& operator =(const Weapon& other) = delete;
	Weapon& operator =(Weapon&& other) noexcept = delete;
	virtual ~Weapon() override;

	float GetDamage() const;
	float GetRange() const;

private:
	explicit Weapon(const SpriteTile* pSpriteTile, const std::string& name, float damage, float range);

	float m_Damage;
	float m_Range;

	virtual std::string GetDescriptionText() const override;
};

