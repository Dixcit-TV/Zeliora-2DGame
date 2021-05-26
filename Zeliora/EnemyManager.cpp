#include "pch.h"
#include <cassert>
#include <algorithm>
#include "EnemyManager.h"
#include "LootManager.h"
#include "Enemy.h"
#include "Hero.h"
#include "Spawn.h"
#include "Guard.h"
#include "Witch.h"

EnemyManager::EnemyManager()
	: m_pEnemies{}
{}

EnemyManager::~EnemyManager()
{
	for (Enemy* pEnemy : m_pEnemies)
		delete pEnemy;
}

Enemy* EnemyManager::operator [](size_t idx)
{
	assert(idx < m_pEnemies.size());
	return m_pEnemies[idx];
}

const Enemy* EnemyManager::operator [](size_t idx) const
{
	assert(idx < m_pEnemies.size());
	return m_pEnemies[idx];
}

std::vector<Enemy*>::iterator EnemyManager::begin()
{
	return m_pEnemies.begin();
}

std::vector<Enemy*>::const_iterator EnemyManager::begin() const
{
	return m_pEnemies.cbegin();
}

std::vector<Enemy*>::iterator EnemyManager::end()
{
	return m_pEnemies.end();
}

std::vector<Enemy*>::const_iterator EnemyManager::end() const
{
	return m_pEnemies.cend();
}

void EnemyManager::Add(Enemy* newCharacter)
{
	m_pEnemies.push_back(newCharacter);
}

void EnemyManager::Update(float elaspedSecond, const Dungeon* pDungeon, Hero* pHero)
{
	for (Enemy* pEnemy : m_pEnemies)
	{
		pEnemy->Update(elaspedSecond, pDungeon, pHero);
	}
}

void EnemyManager::OnDeathEvent(LootManager& lootManager, Hero* pHero)
{
	auto onDeathPred = [&lootManager, pHero](Enemy* pEnemy)
	{
		if (pEnemy->IsDead())
		{
			lootManager.TrySpawnLoot(pEnemy->GetAabb().center);
			if (pEnemy->GetOriginSpawn())
				pEnemy->GetOriginSpawn()->KickStartTimer();

			pHero->ExpUp();
			delete pEnemy;

			return true;
		}

		return false;
	};

	m_pEnemies.erase(std::remove_if(m_pEnemies.begin(), m_pEnemies.end(), onDeathPred), m_pEnemies.end());
}

void EnemyManager::Draw() const
{
	for (Enemy* pEnemy : m_pEnemies)
		pEnemy->Draw();
}

void EnemyManager::SpawnRandomEnemy(const Circlef& spawnRadius, const Rectp& spawnBounds, Spawn* pSpawnObj)
{
	Enemy* pNewEnemy{ nullptr };
	int rand{ utils::GetRand(1) }; 
	if (rand)
		pNewEnemy = Guard::Create(Point2f{}, pSpawnObj);
	else
		pNewEnemy = Witch::Create(Point2f{}, pSpawnObj);

	Point2f newPos{ utils::GetPointInBoundedRadius(spawnRadius, spawnBounds, pNewEnemy->GetAabb().halfWidth, pNewEnemy->GetAabb().halfHeight) };
	pNewEnemy->SetPos(newPos);

	for (const Enemy* pEnemy : m_pEnemies)
	{
		pNewEnemy->HandleCollision(pEnemy);
	}

	m_pEnemies.push_back(pNewEnemy);
}