#include "pch.h"
#include <fstream>
#include <vector>
#include "Weapon.h"
#include "SpriteTile.h"
#include "utils.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "ResourceManager.h"

Weapon* Weapon::Create(bool useDefault)
{
	const std::string xPathQuery{ "Objects/Weapons/Weapon" };

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

	int idx{};
	if (!useDefault)
		idx = utils::GetRand(int(elements.size() - 1));
	else
		idx = 0;

	return new Weapon(
		new SpriteTile(
			ResourceManager::GetInstance()->GetSpriteSheet(utils::GetXmlAttributeValue("SpritePath", elements[idx]))
			, std::stoi(utils::GetXmlAttributeValue("Row", elements[idx]))
			, std::stoi(utils::GetXmlAttributeValue("Column", elements[idx]))
		)
		, utils::GetXmlAttributeValue("Name", elements[idx])
		, std::stof(utils::GetXmlAttributeValue("Damage", elements[idx]))
		, std::stof(utils::GetXmlAttributeValue("Range", elements[idx]))
	);
}

Weapon::Weapon(const SpriteTile* pSpriteTile, const std::string& name, float damage, float range)
	: Object(pSpriteTile, name, ObjectType::weapon)
	, m_Damage{ damage }
	, m_Range{ range }
{
	m_pObjectTextDescription = new Texture(
		GetDescriptionText()
		, ResourceManager::GetInstance()->GetFont(ResourceManager::FontSize::size_18)
		, ResourceManager::GetInstance()->GetFontColor(ResourceManager::FontCategory::classic)
		, true
	);
}

Weapon::~Weapon() {}

float Weapon::GetDamage() const
{
	return m_Damage;
}

float Weapon::GetRange() const
{
	return m_Range;
}

std::string Weapon::GetDescriptionText() const
{
	std::string description{};
	description += m_Name + ": \n";
	description += "Damage: " + std::to_string(int(m_Damage)) + "\n";
	return description;
}