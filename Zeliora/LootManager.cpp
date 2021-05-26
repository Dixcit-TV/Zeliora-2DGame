#include "pch.h"
#include <algorithm>
#include "LootManager.h"
#include "utils.h"
#include <cassert>
#include "Armor.h"
#include "Weapon.h"

Loot& LootManager::operator [](size_t idx)
{
	assert(idx < m_ActiveLoot.size());
	return m_ActiveLoot[idx];
}

const Loot& LootManager::operator [](size_t idx) const
{
	assert(idx < m_ActiveLoot.size());
	return m_ActiveLoot[idx];
}

size_t LootManager::Size() const
{
	return m_ActiveLoot.size();
}

void LootManager::Update()
{
	auto pred = [](const Loot& loot) { return loot.IsPickedUp(); };

	m_ActiveLoot.erase(remove_if(m_ActiveLoot.begin(), m_ActiveLoot.end(), pred), m_ActiveLoot.end());
}

void LootManager::TrySpawnLoot(const Point2f& lootPos)
{
	Object* newObject{ nullptr };
	int dropLoot{ utils::GetRand(3) };

	if (dropLoot == 1)
	{
		const int numTypes{ 2 };
		Object::ObjectType types[]{ Object::ObjectType::armor, Object::ObjectType::weapon };

		int randType{ utils::GetRand(numTypes - 1) };

		switch (types[randType])
		{
		case Object::ObjectType::armor:
			newObject = Armor::Create();
			break;
		case Object::ObjectType::weapon:
			newObject = Weapon::Create();
			break;
		}

		m_ActiveLoot.push_back(Loot(newObject, lootPos));
	}
}

void LootManager::Draw() const
{
	for (const Loot& loot : m_ActiveLoot)
	{
		loot.Draw();
	}
}