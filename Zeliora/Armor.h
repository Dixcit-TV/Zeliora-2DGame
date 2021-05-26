#pragma once
#include "Object.h"

class Armor final : public Object
{
public:
	enum class ArmorType
	{
		arms, body, helmet, any
	};

	static Armor* Create(ArmorType armorType = ArmorType::any, bool useDefault = false);
	Armor(const Armor& other) = delete;
	Armor(Armor&& other) noexcept = delete;
	Armor& operator =(const Armor& other) = delete;
	Armor& operator =(Armor&& other) noexcept = delete;
	virtual ~Armor() override;

	ArmorType GetArmorType() const;

private:
	explicit Armor(const SpriteTile* pSpriteTile, const std::string& name, int stat, ArmorType armorType);

	int m_Stat;
	const ArmorType m_ArmorType;

	virtual std::string GetDescriptionText() const override;
	static std::string GetTypeToString(ArmorType type);
	static ArmorType StrToArmorType(const std::string& str);
};

