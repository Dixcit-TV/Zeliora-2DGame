#include "pch.h"
#include "Potion.h"
#include "Sprite.h"
#include <stdexcept>

Potion::Potion(const SpriteTile* pSpriteTile, const std::string& name, int effectAmount, PotionType potionType)
	: Object(pSpriteTile, name, ObjectType::potion)
	, m_EffectAmount{ effectAmount }
	, m_PotionType{ potionType }
{}

Potion::~Potion() {}

std::string Potion::GetTypeToString(PotionType type)
{
	std::string attrVal{};
	switch (type)
	{
	case PotionType::health:
		return "health";
	}

	throw std::runtime_error("Unexpected potion type!");
}