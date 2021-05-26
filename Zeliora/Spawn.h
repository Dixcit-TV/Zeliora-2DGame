#pragma once
#include "structs.h"
#include <vector>
#include "utils.h"

class EnemyManager;

class Spawn
{
public:
	Spawn(const Point2f& pos, const Rectp& container, int maxEnemies, float spawnDelay, float spawnRadius);

	void FillSpawn(EnemyManager& characterManager);
	void Update(float elaspedTime, EnemyManager& characterManager);
	void KickStartTimer();

private:
	struct SpawnTimer 
	{
		float timer;
		bool isActive;
	};

	Point2f m_Pos;
	Rectp m_Container;
	int m_MaxEnemies;
	float m_SpawnRadius;
	float m_SpawnDelay;
	std::vector<SpawnTimer> m_SpawnTimers;
};

