#include "pch.h"
#include <iostream>
#include <random>
#include <cassert>
#include <set>
#include <stack>
#include "Dungeon.h"
#include "utils.h"
#include "Camera.h"
#include "DungeonGenerator.h"
#include "Spawn.h"
#include "Texture.h"
#include "Hero.h"
#include "ResourceManager.h"
#include "SpriteSheet.h"

Dungeon::Dungeon(std::vector<DungeonArea>&& dungeonRooms, std::vector<TileType>&& dungeonGrid, const Point2f& dungeonBotLeft, int gridNumCols, int gridNumRows)
	: m_Rooms{ dungeonRooms }
	, m_DungeonGrid{ dungeonGrid }
	, m_DungeonAINavGrid{}
	, m_GridColumns{ gridNumCols }
	, m_GridRows{ gridNumRows }
	, m_GridBotLeft{ dungeonBotLeft }
	, m_Directions{ 1, -1, gridNumCols, -gridNumCols, 1 + gridNumCols, - 1 + gridNumCols, 1 - gridNumCols, - 1 - gridNumCols }
	, m_pEntranceRoom{}
	, m_LootManager{}
	, m_EnemyManager{}
	, m_pSpawns{}
	, m_Destructibles{}
{
	SelectStartRoom();
	AddWorldObjects();
	ComputeAiNavMap();
	Populate();
}

Dungeon::~Dungeon()
{
	for (Spawn* pSpawn : m_pSpawns)
		delete pSpawn;
}

#pragma region Prepfunctions
void Dungeon::SelectStartRoom()
{
	for (const DungeonArea& area : m_Rooms)
	{
		if (area.GetType() == DungeonArea::DungeonAreaType::mainRoom)
		{
			if (!m_pEntranceRoom)
				m_pEntranceRoom = &area;
			else
				m_pEntranceRoom = m_pEntranceRoom->GetAabb().center.x > area.GetAabb().center.x ? &area : m_pEntranceRoom;
		}
	}
}

void Dungeon::AddWorldObjects()
{
	const float worldObjectHalfSize{ DungeonGenerator::m_TileSize / 2.f };
	size_t cornerTileIdx{};

	for (const DungeonArea& area : m_Rooms)
	{
		const Rectp& areaAabb{ area.GetInnerAabb(DungeonGenerator::m_TileSize) };

		//room's botLeftCorner Tile
		cornerTileIdx = GetIdxFromPos(Point2f{ areaAabb.center.x - areaAabb.halfWidth, areaAabb.center.y - areaAabb.halfHeight });
		if (m_DungeonGrid[size_t(cornerTileIdx) - size_t(1)] == TileType::wall && m_DungeonGrid[size_t(cornerTileIdx) - size_t(m_GridColumns)] == TileType::wall)
		{
			m_DungeonGrid[cornerTileIdx] = TileType::worldobject;
			m_Destructibles.push_back(Destructible(Rectp{ GetTilePosFromIdx(cornerTileIdx), worldObjectHalfSize, worldObjectHalfSize }));

			m_DungeonGrid[cornerTileIdx + 1] = TileType::worldobject;
			m_Destructibles.push_back(Destructible(Rectp{ GetTilePosFromIdx(cornerTileIdx + 1), worldObjectHalfSize, worldObjectHalfSize }));

			m_DungeonGrid[cornerTileIdx + m_GridColumns] = TileType::worldobject;
			m_Destructibles.push_back(Destructible(Rectp{ GetTilePosFromIdx(cornerTileIdx + m_GridColumns), worldObjectHalfSize, worldObjectHalfSize }));
		}

		//room's botRightCorner Tile
		cornerTileIdx = size_t(GetIdxFromPos(Point2f{ areaAabb.center.x + areaAabb.halfWidth, areaAabb.center.y - areaAabb.halfHeight })) - 1;
		if (m_DungeonGrid[cornerTileIdx + 1] == TileType::wall && m_DungeonGrid[cornerTileIdx - m_GridColumns] == TileType::wall)
		{
			m_DungeonGrid[cornerTileIdx] = TileType::worldobject;
			m_Destructibles.push_back(Destructible(Rectp{ GetTilePosFromIdx(cornerTileIdx), worldObjectHalfSize, worldObjectHalfSize }));

			m_DungeonGrid[cornerTileIdx - 1] = TileType::worldobject;
			m_Destructibles.push_back(Destructible(Rectp{ GetTilePosFromIdx(cornerTileIdx - 1), worldObjectHalfSize, worldObjectHalfSize }));

			m_DungeonGrid[cornerTileIdx + m_GridColumns] = TileType::worldobject;
			m_Destructibles.push_back(Destructible(Rectp{ GetTilePosFromIdx(cornerTileIdx + m_GridColumns), worldObjectHalfSize, worldObjectHalfSize }));
		}

		//room's topLeftCorner Tile
		cornerTileIdx = size_t(GetIdxFromPos(Point2f{ areaAabb.center.x - areaAabb.halfWidth, areaAabb.center.y + areaAabb.halfHeight })) - m_GridColumns;
		if (m_DungeonGrid[cornerTileIdx - 1] == TileType::wall && m_DungeonGrid[cornerTileIdx + m_GridColumns] == TileType::wall)
		{
			m_DungeonGrid[cornerTileIdx] = TileType::worldobject;
			m_Destructibles.push_back(Destructible(Rectp{ GetTilePosFromIdx(cornerTileIdx), worldObjectHalfSize, worldObjectHalfSize }));

			m_DungeonGrid[cornerTileIdx + 1] = TileType::worldobject;
			m_Destructibles.push_back(Destructible(Rectp{ GetTilePosFromIdx(cornerTileIdx + 1), worldObjectHalfSize, worldObjectHalfSize }));

			m_DungeonGrid[cornerTileIdx - m_GridColumns] = TileType::worldobject;
			m_Destructibles.push_back(Destructible(Rectp{ GetTilePosFromIdx(cornerTileIdx - m_GridColumns), worldObjectHalfSize, worldObjectHalfSize }));
		}

		//room's topRightCorner Tile
		cornerTileIdx = size_t(GetIdxFromPos(Point2f{ areaAabb.center.x + areaAabb.halfWidth, areaAabb.center.y + areaAabb.halfHeight })) - 1 - m_GridColumns;
		if (m_DungeonGrid[cornerTileIdx + 1] == TileType::wall && m_DungeonGrid[cornerTileIdx + m_GridColumns] == TileType::wall)
		{
			m_DungeonGrid[cornerTileIdx] = TileType::worldobject;
			m_Destructibles.push_back(Destructible(Rectp{ GetTilePosFromIdx(cornerTileIdx), worldObjectHalfSize, worldObjectHalfSize }));

			m_DungeonGrid[cornerTileIdx - 1] = TileType::worldobject;
			m_Destructibles.push_back(Destructible(Rectp{ GetTilePosFromIdx(cornerTileIdx - 1), worldObjectHalfSize, worldObjectHalfSize }));

			m_DungeonGrid[cornerTileIdx - m_GridColumns] = TileType::worldobject;
			m_Destructibles.push_back(Destructible(Rectp{ GetTilePosFromIdx(cornerTileIdx - m_GridColumns), worldObjectHalfSize, worldObjectHalfSize }));
		}
	}
}

void Dungeon::ComputeAiNavMap()
{
	const size_t numTiles{ m_DungeonGrid.size() };
	m_DungeonAINavGrid.resize(numTiles, TileType::wall);

	for (int idx{}; idx < numTiles; ++idx)
	{
		if (m_DungeonGrid[idx] == TileType::walkable)
		{
			bool nextToWalls{ false };

			for (int direction : m_Directions)
			{
				if (m_DungeonGrid[size_t(idx) + size_t(direction)] != TileType::walkable)
				{
					nextToWalls = true;
					break;
				}
			}

			if (!nextToWalls)
			{
				m_DungeonAINavGrid[idx] = TileType::walkable;
			}
		}
	}
}

void Dungeon::Populate()
{
	const float spawnTimerDelay{ 60.f };
	int numEnemies{};
	for (const DungeonArea& area : m_Rooms)
	{
		if (&area != m_pEntranceRoom)
		{
			if (area.GetType() == DungeonArea::DungeonAreaType::mainRoom)
			{
				numEnemies = utils::GetRand(4, 2);
			}
			else
			{
				numEnemies = utils::GetRand(3, 1);
			}

			float spawnRadius{ area.GetAabb().halfWidth < area.GetAabb().halfHeight ? area.GetAabb().halfWidth : area.GetAabb().halfHeight };
			Rectp container{ area.GetInnerAabb(DungeonGenerator::m_TileSize) };
			m_pSpawns.push_back(new Spawn(container.center, container, numEnemies, spawnTimerDelay, spawnRadius));
			m_pSpawns.back()->FillSpawn(m_EnemyManager);
		}
	}
}
#pragma endregion

#pragma region Getters
Point2f Dungeon::GetEntrancePos()
{
	return m_pEntranceRoom ? m_pEntranceRoom->GetAabb().center : throw std::runtime_error("Start Room could not be found for the dungeon !");
}

EnemyManager& Dungeon::GetEnemyManager()
{
	return m_EnemyManager;
}

const EnemyManager& Dungeon::GetEnemyManager() const
{
	return m_EnemyManager;
}

std::vector<Destructible>& Dungeon::GetDestructibles()
{
	return m_Destructibles;
}

int Dungeon::GetIdxFromPos(const Point2f& point) const
{
	int col{ int((point.x - m_GridBotLeft.x) / DungeonGenerator::m_TileSize) };
	int row{ int((point.y - m_GridBotLeft.y) / DungeonGenerator::m_TileSize) };

	return row * m_GridColumns + col;
}

TileType Dungeon::GetTileFromPos(const Point2f& point, bool useAIGrid) const
{
	int idx{ GetIdxFromPos(point) };

	if (idx < 0 || idx > m_GridColumns* m_GridRows)
		return TileType::none;

	if (useAIGrid)
		return m_DungeonAINavGrid[GetIdxFromPos(point)];
	else
		return m_DungeonGrid[GetIdxFromPos(point)];
}

Point2f Dungeon::GetTilePosFromIdx(size_t idx) const
{
	assert(idx < m_DungeonGrid.size());

	size_t col{ idx % m_GridColumns };
	size_t row{ idx / m_GridColumns };

	return Point2f{ m_GridBotLeft.x + (col + 0.5f) * DungeonGenerator::m_TileSize, m_GridBotLeft.y + (row + 0.5f) * DungeonGenerator::m_TileSize };
}

Point2f Dungeon::GetTilePosFromIdx(int idx) const
{
	assert(idx >= 0 && idx < m_DungeonGrid.size());

	int col{ idx % m_GridColumns };
	int row{ idx / m_GridColumns };

	return Point2f{ m_GridBotLeft.x + (col + 0.5f) * DungeonGenerator::m_TileSize, m_GridBotLeft.y + (row + 0.5f) * DungeonGenerator::m_TileSize };
}

Point2f Dungeon::GetRandCenterInRadius(const Circlef& area, float shapeHalfWidth, float shapeHalfHeight, bool useAIGrid) const
{
	float randAngle = utils::GetRand(2 * utils::g_Pi);
	float newX{ area.center.x + area.radius * float(cos(randAngle)) };
	float newY{ area.center.y + area.radius * float(sin(randAngle)) };


	if (GetTileFromPos(Point2f{ newX, newY }, useAIGrid) != TileType::walkable)
		return GetRandCenterInRadius(area, shapeHalfWidth, shapeHalfHeight, useAIGrid);

	return Point2f{ newX, newY };
}
#pragma endregion

#pragma region GameFunctions
void Dungeon::Update(float elapsedSec, Hero* pPlayer)
{
	m_EnemyManager.Update(elapsedSec, this, pPlayer);
	m_EnemyManager.OnDeathEvent(m_LootManager, pPlayer);

	for (Spawn* pSpawn : m_pSpawns)
		pSpawn->Update(elapsedSec, m_EnemyManager);

	for (Destructible& destructible : m_Destructibles)
		destructible.Update(elapsedSec, m_LootManager);

	pPlayer->LootPickUpCheck(m_LootManager);
	m_LootManager.Update();

	CleanUpDestructibles();
}

void Dungeon::DrawCave(const Camera* camera) const
{
	const SpriteSheet* tileSet{ ResourceManager::GetInstance()->GetSpriteSheet(GetSpriteSheet()) };
	const int tileSize{ DungeonGenerator::m_TileSize };

	const int extension{ 2 };
	Rectp cameraAabb{ camera->GetAabb() };
	int drawLeftCol{ int(cameraAabb.center.x - cameraAabb.halfWidth - m_GridBotLeft.x) / tileSize - extension };
	int drawBotRow{ int(cameraAabb.center.y - cameraAabb.halfHeight - m_GridBotLeft.y) / tileSize - extension };
	int drawRightCol{ int(cameraAabb.center.x + cameraAabb.halfWidth - m_GridBotLeft.x) / tileSize + extension };
	int drawTopRow{ int(cameraAabb.center.y + cameraAabb.halfHeight - m_GridBotLeft.y) / tileSize + extension };

	if (drawLeftCol < 0)
		drawLeftCol = 0;

	if (drawBotRow < 0)
		drawBotRow = 0;

	if (drawRightCol > m_GridColumns - 1)
		drawRightCol = m_GridColumns - 1;

	if (drawTopRow > m_GridRows - 1)
		drawTopRow = m_GridRows - 1;

	Rectf srcRect{};
	Rectf destRect{};

	int idx{};
	const int wallColumn{ 4 };
	const int pathColumn{ 5 };
	for (int row{ drawBotRow }; row <= drawTopRow; ++row)
	{
		for (int col{ drawLeftCol }; col <= drawRightCol; ++col)
		{
			idx = row * m_GridColumns + col;

			switch (m_DungeonGrid[idx])
			{
			case TileType::none:
				break;
			case TileType::wall:
				srcRect.width = srcRect.height = float(DungeonGenerator::m_TileSize);
				srcRect.left = wallColumn * srcRect.width;
				srcRect.bottom = srcRect.height;

				destRect.width = destRect.height = float(tileSize);
				destRect.left = m_GridBotLeft.x + col * tileSize;
				destRect.bottom = m_GridBotLeft.y + row * tileSize;

				tileSet->Draw(destRect, srcRect);
				break;
			default:
				srcRect.width = srcRect.height = float(DungeonGenerator::m_TileSize);
				srcRect.left = pathColumn * srcRect.width;
				srcRect.bottom = srcRect.height;

				destRect.width = destRect.height = float(tileSize);
				destRect.left = m_GridBotLeft.x + col * tileSize;
				destRect.bottom = m_GridBotLeft.y + row * tileSize;

				tileSet->Draw(destRect, srcRect);
				break;
			}
		}
	}
}

void Dungeon::DrawContent() const
{
	for (const Destructible& destructible : m_Destructibles)
	{
		destructible.Draw();
	}

	m_LootManager.Draw();
	m_EnemyManager.Draw();
}


void Dungeon::Clamp(Character* pCharacter, float elapsedSec) const
{
	const Vector2f& charVelocity{ pCharacter->GetVelocity() };
	Rectp copyAabb{ pCharacter->GetAabb() };

	int bot{}, top{}, left{}, right{};
	bool clampX{ true }, clampY{ true };

	copyAabb.center.x -= charVelocity.x * elapsedSec;
	if (charVelocity.y < 0.f)
	{
		left = GetIdxFromPos(Point2f{ copyAabb.center.x - copyAabb.halfWidth, copyAabb.center.y - copyAabb.halfHeight });
		right = GetIdxFromPos(Point2f{ copyAabb.center.x + copyAabb.halfWidth, copyAabb.center.y - copyAabb.halfHeight });
	}
	else if (charVelocity.y > 0.f)
	{
		left = GetIdxFromPos(Point2f{ copyAabb.center.x - copyAabb.halfWidth, copyAabb.center.y + copyAabb.halfHeight });
		right = GetIdxFromPos(Point2f{ copyAabb.center.x + copyAabb.halfWidth, copyAabb.center.y + copyAabb.halfHeight });
	}

	if (m_DungeonGrid[left] == TileType::walkable && m_DungeonGrid[right] == TileType::walkable)
		clampY = false;

	copyAabb = pCharacter->GetAabb();
	copyAabb.center.y -= charVelocity.y * elapsedSec;

	if (charVelocity.x < 0.f)
	{
		bot = GetIdxFromPos(Point2f{ copyAabb.center.x - copyAabb.halfWidth, copyAabb.center.y - copyAabb.halfHeight });
		top = GetIdxFromPos(Point2f{ copyAabb.center.x - copyAabb.halfWidth, copyAabb.center.y + copyAabb.halfHeight });
	}
	else if (charVelocity.x > 0.f)
	{
		bot = GetIdxFromPos(Point2f{ copyAabb.center.x + copyAabb.halfWidth, copyAabb.center.y - copyAabb.halfHeight });
		top = GetIdxFromPos(Point2f{ copyAabb.center.x + copyAabb.halfWidth, copyAabb.center.y + copyAabb.halfHeight });
	}

	if (m_DungeonGrid[bot] == TileType::walkable && m_DungeonGrid[top] == TileType::walkable)
		clampX = false;

	if (clampY)
		pCharacter->SetPos(Point2f{ pCharacter->GetAabb().center.x, pCharacter->GetAabb().center.y - charVelocity.y * elapsedSec });
	if (clampX)
		pCharacter->SetPos(Point2f{ pCharacter->GetAabb().center.x - charVelocity.x * elapsedSec, pCharacter->GetAabb().center.y });
}

void Dungeon::Clamp(Camera* pCamera) const
{
	Rectp cameraAabb{ pCamera->GetAabb() };
	float gridWidth{ float(m_GridColumns * DungeonGenerator::m_TileSize) };
	float gridHeight{ float(m_GridRows * DungeonGenerator::m_TileSize) };
	bool clamp{ false };

	if (cameraAabb.center.x - cameraAabb.halfWidth < m_GridBotLeft.x)
	{
		cameraAabb.center.x = m_GridBotLeft.x + cameraAabb.halfWidth;
		clamp = true;
	}
	else if (cameraAabb.center.x + cameraAabb.halfWidth > m_GridBotLeft.x + gridWidth)
	{
		cameraAabb.center.x = m_GridBotLeft.x + gridWidth - cameraAabb.halfWidth;
		clamp = true;
	}

	if (cameraAabb.center.y - cameraAabb.halfHeight < m_GridBotLeft.y)
	{
		cameraAabb.center.y = m_GridBotLeft.y + cameraAabb.halfHeight;
		clamp = true;
	}
	else if (cameraAabb.center.y + cameraAabb.halfHeight > m_GridBotLeft.y + gridHeight)
	{
		cameraAabb.center.y = m_GridBotLeft.y + gridHeight - cameraAabb.halfHeight;
		clamp = true;
	}

	if (clamp)
	{
		pCamera->SetPos(cameraAabb.center);
	}
}

void Dungeon::CleanUpDestructibles()
{
	for (Destructible& destructible : m_Destructibles)
	{
		if (destructible.IsDestroyed())
		{
			m_DungeonGrid[GetIdxFromPos(destructible.GetPos())] = TileType::walkable;
		}
	}
}
#pragma endregion

#pragma region PathFinding
std::stack<Point2f> Dungeon::FindPathToDestination(const Point2f& start, const Point2f& destination) const
{
	const int maxSearchCount{ 200 };
	int iterationCount{};

	std::stack<Point2f> path{};
	int destIdx{ GetIdxFromPos(destination) };
	if (destIdx < 0 || destIdx >= m_DungeonAINavGrid.size() || start == destination)
	{
		path.push(start);
		return path;
	}

	Point2f clampedDestination { GetTilePosFromIdx(destIdx) };
	auto cmp = [](const PathNode& node1, const PathNode& node2)
	{
		return node1.GetScore() < node2.GetScore();
	};

	std::set<PathNode, decltype(cmp)> openPaths(cmp);
	std::vector<PathNode> visitedNodes{};

	openPaths.insert(PathNode{ GetIdxFromPos(start), -1, 0, int(abs(clampedDestination.x - start.x) + abs(clampedDestination.y - start.y)) });
	int neighborIdx;
	Point2f neighborPos;
	PathNode currentNode{};
	PathNode pathDestNode{ -1, -1, m_GridColumns * m_GridRows, m_GridColumns * m_GridRows };
	bool pathFound{ false };

	while (!openPaths.empty())
	{
		iterationCount++;
		currentNode = openPaths.extract(openPaths.begin()).value();
		visitedNodes.push_back(currentNode);

		if (currentNode.thisId == destIdx)
		{
			path.push(destination);
			pathDestNode = currentNode;
			pathFound = true;
			break;
		}

		if (currentNode.GetScore() < pathDestNode.GetScore())
			pathDestNode = currentNode;

		if (iterationCount == maxSearchCount)
		{
			pathFound = true;
			break;
		}

		for (int direction : m_Directions)
		{
			neighborIdx = currentNode.thisId + direction;

			if (m_DungeonAINavGrid[neighborIdx] != TileType::walkable)
				continue;

			neighborPos = GetTilePosFromIdx(neighborIdx);
			PathNode newNode{ neighborIdx, currentNode.thisId, currentNode.numStepsTo + 1, int(abs(clampedDestination.x - neighborPos.x) + abs(clampedDestination.y - neighborPos.y)) };

			if (std::find(visitedNodes.begin(), visitedNodes.end(), newNode) != visitedNodes.end())
				continue;

			std::set<PathNode>::const_iterator existingNodeIt{ std::find(openPaths.begin(), openPaths.end(), newNode) };
			if (existingNodeIt == openPaths.end() || existingNodeIt->GetScore() > newNode.GetScore())
			{
				if (existingNodeIt != openPaths.end())
					openPaths.erase(existingNodeIt);

				openPaths.insert(newNode);
			}
		}
	}

	if (pathDestNode.prevId == -1)
	{
		path.push(start);
	}
	else
	{
		PathNode pathCurrentNode{ pathDestNode };
		while (pathCurrentNode.prevId != -1)
		{
			path.push(GetTilePosFromIdx(pathCurrentNode.thisId));
			for (const PathNode& node : visitedNodes)
			{
				if (node.thisId == pathCurrentNode.prevId)
				{
					pathCurrentNode = node;
					break;
				}
			}
		}
	}

	return path;
}
#pragma endregion