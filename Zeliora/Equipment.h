#pragma once
#include "enum.h"
#include <unordered_map>
#include "Armor.h"

class Object;
class Weapon;

class Equipment final
{
public:
	explicit Equipment();
	Equipment(const Equipment& other) = delete;
	Equipment(Equipment&& other) noexcept = delete;
	Equipment& operator =(const Equipment& other) = delete;
	Equipment& operator =(Equipment&& other) noexcept = delete;
	~Equipment();

	const Weapon* GetWeapon() const;
	const std::unordered_map<Armor::ArmorType, const Armor*>& GetArmorSet() const;
	void SetDefault();

	const Object* PlaceOrReplace(const Object* pObject);

private:
	const Weapon* m_pWeapon;
	std::unordered_map<Armor::ArmorType, const Armor*> m_pArmorPieces;
};

