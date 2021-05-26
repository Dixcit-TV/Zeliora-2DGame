#include "pch.h"
#include "Equipment.h"
#include "Weapon.h"
#include "Armor.h"

Equipment::Equipment()
	: m_pWeapon{ nullptr }
	, m_pArmorPieces{}
{
	m_pArmorPieces.insert({ Armor::ArmorType::arms, nullptr });
	m_pArmorPieces.insert({ Armor::ArmorType::body, nullptr });
	m_pArmorPieces.insert({ Armor::ArmorType::helmet, nullptr });
}

Equipment::~Equipment()
{
	delete m_pWeapon;

	for (std::pair<const Armor::ArmorType, const Armor*>& armorPair : m_pArmorPieces)
		delete armorPair.second;
}

void Equipment::SetDefault()
{
	m_pWeapon = Weapon::Create(true);
	
	for (std::pair<const Armor::ArmorType, const Armor*>& armorPair : m_pArmorPieces)
		m_pArmorPieces[armorPair.first] = Armor::Create(armorPair.first, true);
}

const Weapon* Equipment::GetWeapon() const
{
	return m_pWeapon;
}

const std::unordered_map<Armor::ArmorType, const Armor*>& Equipment::GetArmorSet() const
{
	return m_pArmorPieces;
}

const Object* Equipment::PlaceOrReplace(const Object* pObject)
{
	const Object* oldObject{ nullptr };
	switch (pObject->GetType())
	{
	case Object::ObjectType::weapon:
		oldObject = m_pWeapon;
		m_pWeapon = static_cast<const Weapon*>(pObject);
		break;
	case Object::ObjectType::armor:
		const Armor* pArmor{ static_cast<const Armor*>(pObject) };
		oldObject = m_pArmorPieces[pArmor->GetArmorType()];
		m_pArmorPieces[pArmor->GetArmorType()] = pArmor;
		break;
	}

	return oldObject;
}