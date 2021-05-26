#pragma once
#include <vector>
#include "LootManager.h"
#include "EnemyManager.h"
#include "Destructible.h"

class DungeonArea;
class Character;
class Camera;
class Room;

class Dungeon final
{
public:
	explicit Dungeon(std::vector<DungeonArea>&& dungeonRooms, std::vector<TileType>&& dungeonGrid, const Point2f& dungeonBotLeft, int gridNumCols, int gridNumRows);
	Dungeon(const Dungeon& other) = delete;
	Dungeon(Dungeon&& other) noexcept = delete;
	Dungeon& operator =(const Dungeon& other) = delete;
	Dungeon& operator =(Dungeon&& other) noexcept = delete;
	~Dungeon();

	Point2f GetEntrancePos();
	EnemyManager& GetEnemyManager();
	const EnemyManager& GetEnemyManager() const;
	std::vector<Destructible>& GetDestructibles();

	void Update(float elapsedSec, Hero* pPlayer);
	void DrawCave(const Camera* camera) const;
	void DrawContent() const;

	std::stack<Point2f> FindPathToDestination(const Point2f& start, const Point2f& destination) const;
	void Clamp(Character* pCharacter, float elapsedSec) const;
	void Clamp(Camera* pCamera) const;
	int GetIdxFromPos(const Point2f& point) const;
	TileType GetTileFromPos(const Point2f& point, bool useAIGrid = false) const;
	Point2f GetTilePosFromIdx(size_t idx) const;
	Point2f GetTilePosFromIdx(int idx) const;
	Point2f GetRandCenterInRadius(const Circlef& area, float shapeHalfWidth, float shapeHalfHeight, bool useAIGrid = false) const;

private:
	struct PathNode
	{
		int thisId;
		int prevId;
		int numStepsTo;
		int distToGoal;

		static const int epsilon{ 5 };

		bool operator ==(const Dungeon::PathNode& other) const { return thisId == other.thisId; }
		int GetScore() const { return epsilon * distToGoal + numStepsTo; };
	};

	const std::vector<DungeonArea> m_Rooms;
	std::vector<TileType> m_DungeonGrid;
	std::vector<TileType> m_DungeonAINavGrid;

	const int m_GridColumns;
	const int m_GridRows;
	const Point2f m_GridBotLeft;
	const DungeonArea* m_pEntranceRoom;
	static const int m_NumDirections{ 8 };
	const int m_Directions[m_NumDirections];

	EnemyManager m_EnemyManager;
	LootManager m_LootManager;
	std::vector<Spawn*> m_pSpawns;
	std::vector<Destructible> m_Destructibles;
	
	void SelectStartRoom();
	void AddWorldObjects();
	void ComputeAiNavMap();
	void Populate();
	void CleanUpDestructibles();

	static std::string GetSpriteSheet() { return "WorldObjects/WorldTileset.png"; };
};