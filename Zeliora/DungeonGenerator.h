#pragma once
#include <vector>

class DungeonArea;
class Dungeon;

class DungeonGenerator final
{
public:
	static Dungeon* Generate(const Point2f& center, float radiusX, float radiusY, int numStartingCells);

	static const int m_TileSize;

private:
	struct Edge
	{
		int room1;
		int room2;
		float weight;
	};

	struct HallwayAabb
	{
		Rectp aabb;
		bool isHorizontal;
	};

	static const int m_MinRoomSize;
	static const int m_MaxRoomSize;
	static const int m_CorridorTileSize;

	static std::vector<Rectp> GenerateCells(int numCells, const Ellipsef& bounds);
	static Point2f GetRandPointInEllipse(const Ellipsef& bounds);
	static void SeparateRooms(std::vector<Rectp>& cells);
	static std::vector<DungeonArea> SelectMainRooms(std::vector<Rectp>& cells);
	static const std::vector<Edge> ComputeMST(const std::vector<DungeonArea>& pdungeonRooms);
	static std::vector<HallwayAabb> ComputeHallways(const std::vector<DungeonArea>& pdungeonRooms, const std::vector<Edge>& graph);
	static void FecthHallwayRooms(std::vector<Rectp>& cells, std::vector<DungeonArea>& pdungeonRooms, const std::vector<HallwayAabb>& hallways);
	static void CarveMap(const std::vector<DungeonArea>& pdungeonRooms, const std::vector<HallwayAabb>& hallways, std::vector<TileType>& dungeonGrid, Point2f& gridBotLeft, int& gridColumns, int& gridRows);
};

