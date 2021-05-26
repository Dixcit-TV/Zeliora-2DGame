#pragma once
#include <vector>
#include "Enemy.h"

class Object;
class LootManager;
class Hero;
class Dungeon;

class EnemyManager final
{
public:
	explicit EnemyManager();
	EnemyManager(const EnemyManager& other) = delete;
	EnemyManager& operator =(const EnemyManager& other) = delete;
	EnemyManager(EnemyManager&& other) noexcept = delete;
	EnemyManager& operator =(EnemyManager&& other) noexcept = delete;
	~EnemyManager();

	Enemy* operator [](size_t idx);
	const Enemy* operator [](size_t idx) const;

	std::vector<Enemy*>::iterator begin();
	std::vector<Enemy*>::const_iterator begin() const;
	std::vector<Enemy*>::iterator end();
	std::vector<Enemy*>::const_iterator end() const;

	void SpawnRandomEnemy(const Circlef& spawnRadius, const Rectp& spawnBounds, Spawn* pSpawnObj);
	void Add(Enemy* newCharacter);
	void Update(float elaspedSecond, const Dungeon* pDungeon, Hero* pHero);
	void OnDeathEvent(LootManager& lootManager, Hero* pHero);
	void Draw() const;

private:
	std::vector<Enemy*> m_pEnemies;
};

