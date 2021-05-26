#include "pch.h"
#include <fstream>
#include <vector>
#include "Armor.h"
#include "SpriteTile.h"
#include "Character.h"
#include "utils.h"
#include "ResourceManager.h"
#include "Texture.h"

Armor* Armor::Create(ArmorType armorType, bool useDefault)
{
	const std::string xPathQuery{ "Objects/Armors/Armor" };

	std::ifstream iS{};
	iS.open(GetXmlPath());

	if (!iS.is_open())
	{
		throw std::runtime_error("Couldn't open file " + GetXmlPath());
	}

	std::vector<std::string> elements;

	if (armorType == ArmorType::any)
		elements =  utils::GetXmlElements(iS, xPathQuery);
	else
	{
		elements = utils::GetXmlElements(iS, xPathQuery, "ArmorType", GetTypeToString(armorType));
	}
	iS.close();

	if (elements.size() == 0)
		throw std::runtime_error("Armor not found!");

	int idx{};
	if (!useDefault)
		idx = utils::GetRand(int(elements.size() - 1));
	else
		idx = 0;

	return new Armor(
		new SpriteTile(
			ResourceManager::GetInstance()->GetSpriteSheet(utils::GetXmlAttributeValue("SpritePath", elements[idx]))
			, std::stoi(utils::GetXmlAttributeValue("Row", elements[idx]))
			, std::stoi(utils::GetXmlAttributeValue("Column", elements[idx]))
		)
		, utils::GetXmlAttributeValue("Name", elements[idx])
		, std::stoi(utils::GetXmlAttributeValue("Stat", elements[idx]))
		, StrToArmorType(utils::GetXmlAttributeValue("ArmorType", elements[idx]))
	);
}

Armor::Armor(const SpriteTile* pSpriteTile, const std::string& name, int stat, ArmorType armorType)
	: Object(pSpriteTile, name, ObjectType::armor)
	, m_Stat{ stat }
	, m_ArmorType{ armorType }
{
	m_pObjectTextDescription = new Texture(
		GetDescriptionText()
		, ResourceManager::GetInstance()->GetFont(ResourceManager::FontSize::size_18)
		, ResourceManager::GetInstance()->GetFontColor(ResourceManager::FontCategory::classic)
		, true
	);
}

Armor::~Armor() {}

Armor::ArmorType Armor::GetArmorType() const
{
	return m_ArmorType;
}

std::string Armor::GetDescriptionText() const
{
	std::string description{};
	description += m_Name + ": \n";
	description += "Type: " + GetTypeToString(m_ArmorType) + "\n";
	description += "Defence: " + std::to_string(m_Stat) + "\n";
	return description;
}

std::string Armor::GetTypeToString(ArmorType type)
{
	std::string attrVal{};
	switch (type)
	{
	case ArmorType::arms:
		return "arms";
	case ArmorType::body:
		return "body";
	case ArmorType::helmet:
		return "helmet";
	}

	throw std::runtime_error("Invalid Armor Type");
}

Armor::ArmorType Armor::StrToArmorType(const std::string& str)
{
	if (str == "helmet")
		return ArmorType::helmet;
	else if (str == "body")
		return ArmorType::body;
	else if (str == "arms")
		return ArmorType::arms;
	else
		throw std::runtime_error("Invalid Armor Type");
}