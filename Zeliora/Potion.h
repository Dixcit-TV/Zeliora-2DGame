#pragma once
#include "Object.h"

class Potion final : public Object
{
public:
	enum class PotionType
	{
		health
	};

	explicit Potion(const SpriteTile* pSpriteTile, const std::string& name, int effectAmount, PotionType potionType);
	Potion(const Potion& other) = delete;
	Potion(Potion&& other) noexcept = delete;
	Potion& operator =(const Potion& other) = delete;
	Potion& operator =(Potion&& other) noexcept = delete;
	virtual ~Potion() override;

private:
	int m_EffectAmount;
	PotionType m_PotionType;

	static std::string GetTypeToString(PotionType type);
};

