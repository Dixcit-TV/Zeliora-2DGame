#include "pch.h"
#include <stdexcept>
#include <fstream>
#include "Guard.h"
#include "Hero.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "SpriteTile.h"
#include "utils.h"
#include "Dungeon.h"

Guard* Guard::Create(const Point2f& startPos, Spawn* pSpawn)
{
	const std::string xPathQuery{ "Characters/Enemies/Guard" };

	std::ifstream iS{};
	iS.open(GetXmlPath());

	if (!iS.is_open())
	{
		throw std::runtime_error("Couldn't open file " + GetXmlPath());
	}

	std::vector<std::string> elements{ utils::GetXmlElements(iS, xPathQuery) };
	iS.close();

	if (elements.size() == 0)
		throw std::runtime_error("Enemy Character Guard does not exist!");

	int randIdx{ utils::GetRand(int(elements.size() - 1)) };

	return new Guard(
		new Sprite(
			ResourceManager::GetInstance()->GetSpriteSheet(utils::GetXmlAttributeValue("SpritePath", elements[randIdx]))
			, std::stoi(utils::GetXmlAttributeValue("SpriteOffset", elements[randIdx]))
			, std::stoi(utils::GetXmlAttributeValue("Columns", elements[randIdx]))
			, std::stoi(utils::GetXmlAttributeValue("Rows", elements[randIdx]))
			, std::stoi(utils::GetXmlAttributeValue("StartColumn", elements[randIdx]))
			, std::stoi(utils::GetXmlAttributeValue("StartRow", elements[randIdx]))
			, std::stof(utils::GetXmlAttributeValue("Frametime", elements[randIdx]))
		)
		, startPos
		, std::stof(utils::GetXmlAttributeValue("Health", elements[randIdx]))
		, std::stof(utils::GetXmlAttributeValue("AttackRadius", elements[randIdx]))
		, std::stof(utils::GetXmlAttributeValue("Strength", elements[randIdx]))
		, std::stof(utils::GetXmlAttributeValue("MovementSpeed", elements[randIdx]))
		, pSpawn
		, Attack{ utils::GetMeleeHitSprite(), 0.2f }
	);
}

Guard::Guard(Sprite* pSprite, const Point2f& startPos, float health, float attackRadius, float attackStrength, float movementSpeed, Spawn* pSpawn, Attack&& attack)
	: Enemy(pSprite, startPos, health, attackRadius, attackStrength, movementSpeed, 350.f, 50.f, pSpawn, std::move(attack), new SpriteTile(ResourceManager::GetInstance()->GetSpriteSheet("Weapons/Weapons.png"), 0, 0))
{}

void Guard::SeekTarget(const Hero* pHero)
{
	float squaredDist{ (pHero->GetAabb().center - m_Shape.center).SquaredLength() };
	const float maxDim{ std::max(m_Shape.halfWidth, m_Shape.halfHeight) };

	if (squaredDist <= ((m_SeekingRadius + maxDim) * (m_SeekingRadius + maxDim)))
	{
		m_State = EnemyState::follow;

		if (squaredDist <= ((m_RangeRadius + maxDim) * (m_RangeRadius + maxDim)))
		{
			m_State = EnemyState::inrange;
		}
	}
	else if (m_State != EnemyState::wait)
	{
		m_State = EnemyState::move;
	}
}

void Guard::KickStartAttack(const Point2f& targetPoint)
{
	float attackDirection{ atan2f(targetPoint.y - m_Shape.center.y, targetPoint.x - m_Shape.center.x) };
	m_Attack.Start(Point2f{ m_Shape.center.x + (m_Shape.halfWidth + m_AttackRadius) * cos(attackDirection), m_Shape.center.y + (m_Shape.halfHeight + m_AttackRadius) * sin(attackDirection) }, m_AttackRadius, attackDirection);
}