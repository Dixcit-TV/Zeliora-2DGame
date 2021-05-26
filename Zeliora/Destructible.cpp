#include "pch.h"
#include <fstream>
#include "Destructible.h"
#include "utils.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "Attack.h"

Destructible::Destructible(const Rectp& shape)
	: m_pSprite{ nullptr }
	, m_Shape{ shape }
	, m_State{ DestructibleState::set }
{
	const std::string xPathQuery{ "WorldObjects/Destructibles/Destructible" };
	const std::string xmlPath{ "Resources/Data/WorldObjects.xml" };

	std::ifstream iS{};
	iS.open(xmlPath);

	if (!iS.is_open())
	{
		throw std::runtime_error("Couldn't open file " + xmlPath);
	}

	std::vector<std::string> elements{ utils::GetXmlElements(iS, xPathQuery) };
	iS.close();

	if (elements.size() == 0)
		throw std::runtime_error("No destructibles were found!");

	int randIdx{ utils::GetRand(int(elements.size() - 1)) };

	m_pSprite = new Sprite(
			ResourceManager::GetInstance()->GetSpriteSheet(utils::GetXmlAttributeValue("SpritePath", elements[randIdx]))
			, std::stoi(utils::GetXmlAttributeValue("SpriteOffset", elements[randIdx]))
			, std::stoi(utils::GetXmlAttributeValue("Columns", elements[randIdx]))
			, std::stoi(utils::GetXmlAttributeValue("Rows", elements[randIdx]))
			, std::stoi(utils::GetXmlAttributeValue("StartColumn", elements[randIdx]))
			, std::stoi(utils::GetXmlAttributeValue("StartRow", elements[randIdx]))
			, std::stof(utils::GetXmlAttributeValue("Frametime", elements[randIdx]))
		);
}

Destructible::Destructible(Destructible&& other) noexcept
	: m_pSprite{ other.m_pSprite }
	, m_Shape{ other.m_Shape }
	, m_State{ other.m_State }
{
	other.m_pSprite = nullptr;
}

Destructible::~Destructible() 
{
	delete m_pSprite;
};

bool Destructible::IsDestroyed() const
{
	return m_State == DestructibleState::destroyed;
}

const Point2f& Destructible::GetPos() const
{
	return m_Shape.center;
}

bool Destructible::HitCheck(const Attack& attack)
{
	if (m_State == DestructibleState::set && utils::IsOverlapping(m_Shape, attack.GetImpactArea()))
	{
		m_State = DestructibleState::hit;

		return true;
	}

	return false;
}

void Destructible::Update(float elapsedSec, LootManager& lootManager)
{
	if (m_State == DestructibleState::hit)
	{
		if (m_pSprite->IsDone())
		{
			lootManager.TrySpawnLoot(m_Shape.center);
			m_State = DestructibleState::destroyed;
		}
		else
			m_pSprite->NextFrame(elapsedSec, false);
	}
}

void Destructible::Draw() const
{
	if (m_State != DestructibleState::destroyed)
		m_pSprite->Draw(m_Shape);
}