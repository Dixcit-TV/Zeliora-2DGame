#pragma once
#include <vector>
#include "Loot.h"

class LootManager final
{
public:
	Loot& operator [](size_t idx);
	const Loot& operator [](size_t idx) const;

	void Update();
	void TrySpawnLoot(const Point2f& lootPos);
	void Draw() const;
	size_t Size() const;

private:
	std::vector<Loot> m_ActiveLoot;
};

