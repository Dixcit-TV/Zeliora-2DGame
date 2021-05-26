#include "pch.h"
#include "Spawn.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Guard.h"
#include "Witch.h"

Spawn::Spawn(const Point2f& pos, const Rectp& container, int maxEnemies, float spawnDelay, float spawnRadius)
	: m_Pos{ pos }
	, m_Container{ container }
	, m_MaxEnemies{ maxEnemies }
	, m_SpawnRadius{ spawnRadius }
	, m_SpawnDelay{ spawnDelay }
	, m_SpawnTimers{}
{
	m_SpawnTimers.resize(maxEnemies, SpawnTimer{ 0.f, false });
}

void Spawn::FillSpawn(EnemyManager& characterManager)
{
	for (int count{}; count < m_MaxEnemies; ++count)
	{
		characterManager.SpawnRandomEnemy(Circlef{ m_Pos, m_SpawnRadius }, m_Container, this);
	}
}

void Spawn::Update(float elaspedTime, EnemyManager& characterManager)
{
	for (SpawnTimer& timer : m_SpawnTimers)
	{
		if (timer.isActive)
		{
			timer.timer += elaspedTime;
			if (timer.timer >= m_SpawnDelay)
			{
				characterManager.SpawnRandomEnemy(Circlef{ m_Pos, m_SpawnRadius}, m_Container, this);
				timer.timer = 0.f;
				timer.isActive = false;
			}
		}
	}
}

void Spawn::KickStartTimer()
{
	for (SpawnTimer& timer : m_SpawnTimers)
	{
		if (!timer.isActive)
		{
			timer.isActive = true;
			break;
		}
	}
}