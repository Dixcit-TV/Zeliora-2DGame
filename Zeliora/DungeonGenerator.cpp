#include "pch.h"
#include <random>
#include "Dungeon.h"
#include "DungeonGenerator.h"
#include "DungeonArea.h"
#include "Vector2f.h"
#include "utils.h"

const int DungeonGenerator::m_TileSize = 32;
const int DungeonGenerator::m_MinRoomSize = 12;
const int DungeonGenerator::m_MaxRoomSize = 28;
const int DungeonGenerator::m_CorridorTileSize = m_MinRoomSize / 2;

Dungeon* DungeonGenerator::Generate(const Point2f& center, float radiusX, float radiusY, int numStartingCells)
{
	std::vector<Rectp> cells{ GenerateCells(numStartingCells, Ellipsef{ center, radiusX, radiusY } ) };
	SeparateRooms(cells);
	std::vector<DungeonArea> pdungeonRooms{ SelectMainRooms(cells) };
	const std::vector<Edge> graph{ ComputeMST(pdungeonRooms) };
	std::vector<HallwayAabb> hallways{ ComputeHallways(pdungeonRooms, graph) };
	FecthHallwayRooms(cells, pdungeonRooms, hallways);

	std::vector<TileType> dungeonGrid;
	Point2f dungeonBotLeft;
	int gridNumCols, gridNumRows;
	CarveMap(pdungeonRooms, hallways, dungeonGrid, dungeonBotLeft, gridNumCols, gridNumRows);

	return new Dungeon(std::move(pdungeonRooms), std::move(dungeonGrid), dungeonBotLeft, gridNumCols, gridNumRows);
}

#pragma region Generation Functions

Point2f DungeonGenerator::GetRandPointInEllipse(const Ellipsef& bounds)
{
	float randAngle = 2 * utils::g_Pi * utils::GetRand(1.f);
	float rand2 = utils::GetRand(1.f) + utils::GetRand(1.f);
	float radRatio = rand2 > 1 ? 2 - rand2 : rand2;

	return Point2f(float(utils::RoundToClosest(bounds.center.x + bounds.radiusX * radRatio * cosf(randAngle), m_TileSize))
		, float(utils::RoundToClosest(bounds.center.y + bounds.radiusY * radRatio * sinf(randAngle), m_TileSize)));
}

std::vector<Rectp> DungeonGenerator::GenerateCells(int numCells, const Ellipsef& bounds)
{
	std::vector<Rectp> cells{};
	cells.reserve(numCells);

	const float minimalRatio{ 0.4f };
	Rectp newRoomRect{ };
	Point2f rectBotLeft{};
	int randX, randY;
	float ratio;

	std::random_device rd{};
	std::mt19937 gen{ rd() };
	std::normal_distribution<float> distribution(0, 8);

	for (int count{}; count < cells.capacity(); count++)
	{
		rectBotLeft = GetRandPointInEllipse(bounds);

		do
		{
			randX = int(abs(distribution(gen)) + m_MinRoomSize);
			randY = int(abs(distribution(gen)) + m_MinRoomSize);

			if (randX > randY)
				ratio = float(randY) / randX;
			else
				ratio = float(randX) / randY;
		} while (randX == randY || ratio < minimalRatio || randX > m_MaxRoomSize || randY > m_MaxRoomSize);

		newRoomRect.halfWidth = m_TileSize * randX / 2.f;
		newRoomRect.halfHeight = m_TileSize * randY / 2.f;
		newRoomRect.center.x = rectBotLeft.x + newRoomRect.halfWidth;
		newRoomRect.center.y = rectBotLeft.y + newRoomRect.halfHeight;

		cells.push_back(newRoomRect);
	}

	return cells;
}

//Separation Steering Mechanism
void DungeonGenerator::SeparateRooms(std::vector<Rectp>& cells)
{
	const size_t numCells{ cells.size() };
	bool hasOverlap{};
	Vector2f force{};
	float xDist{}, yDist{};

	do {
		hasOverlap = false;

		for (size_t i{}; i < numCells; ++i)
		{
			force.x = force.y = 0.f;
			for (size_t j{}; j < numCells; ++j)
			{
				if (i != j && utils::IsOverlapping(cells[i], cells[j]))
				{
					hasOverlap = true;

					xDist = (cells[i].center.x) - (cells[j].center.x);
					yDist = (cells[i].center.y) - (cells[j].center.y);

					if (xDist == 0.f && yDist == 0.f)
					{
						if (cells[i].halfHeight + cells[j].halfHeight > cells[i].halfWidth + cells[j].halfWidth)
							xDist = float(m_TileSize);
						else
							yDist = float(m_TileSize);
					}

					force.x += xDist;
					force.y += yDist;
				}
			}

			Vector2f n{ force.Normalized() * float(m_TileSize) };

			cells[i].center.x += n.x > 0 ? utils::RoundToClosest(n.x, m_TileSize) : utils::RoundToClosest(n.x, -m_TileSize);
			cells[i].center.y += n.y > 0 ? utils::RoundToClosest(n.y, m_TileSize) : utils::RoundToClosest(n.y, -m_TileSize);
		}
	} while (hasOverlap);
}

std::vector<DungeonArea> DungeonGenerator::SelectMainRooms(std::vector<Rectp>& cells)
{
	const int estimatedSize{ 25 };
	std::vector<DungeonArea> dungeonRooms;
	dungeonRooms.reserve(estimatedSize);

	float minAreaMainRooms{ };
	int attempt{ 0 };

	auto mainRoomPred = [&dungeonRooms, &minAreaMainRooms](const Rectp& cell)
	{
		if ((4 * cell.halfWidth * cell.halfHeight) > minAreaMainRooms)
		{
			dungeonRooms.push_back(DungeonArea(cell.center, cell.halfWidth, cell.halfHeight, DungeonArea::DungeonAreaType::mainRoom));
			return true;
		}

		return false;
	};

	//estimated during testing
	const float sizeRatio{ 4.f / 5 };
	do
	{
		minAreaMainRooms = powf(sizeRatio * (m_MaxRoomSize - attempt) * m_TileSize, 2);
		cells.erase(std::remove_if(cells.begin(), cells.end(), mainRoomPred), cells.end());
		++attempt;
	} while (dungeonRooms.size() < 5);

	return dungeonRooms;
}

const std::vector<DungeonGenerator::Edge> DungeonGenerator::ComputeMST(const std::vector<DungeonArea>& dungeonRooms)
{
	const int numRooms{ int(dungeonRooms.size()) };
	std::vector<int> remainingRooms{ };
	remainingRooms.reserve(numRooms);

	for (int idx{}; idx < numRooms; ++idx)
		remainingRooms.push_back(idx);

	std::vector<int> visitedRoomIdxs{ };
	visitedRoomIdxs.reserve(numRooms);
	//------------ Minimal Spanning Tree --------------//
	std::vector<Edge> edges;
	Edge newEdge{};
	visitedRoomIdxs.push_back(remainingRooms.back());
	remainingRooms.pop_back();

	float minDist{}, squaredDist{};
	Point2f currPos{};

	while (remainingRooms.size() > 0)
	{
		newEdge.weight = INFINITY;

		for (int visitedIdx : visitedRoomIdxs)
		{
			currPos = dungeonRooms[visitedIdx].GetAabb().center;

			for (int remainingIdx : remainingRooms)
			{
				squaredDist = (currPos - dungeonRooms[remainingIdx].GetAabb().center).SquaredLength();

				if (squaredDist < newEdge.weight)
				{
					newEdge.room1 = visitedIdx;
					newEdge.room2 = remainingIdx;
					newEdge.weight = squaredDist;
				}
			}
		}

		visitedRoomIdxs.push_back(newEdge.room2);
		remainingRooms.erase(remove(remainingRooms.begin(), remainingRooms.end(), newEdge.room2));
		edges.push_back(newEdge);
	}

	return edges;
}

std::vector<DungeonGenerator::HallwayAabb> DungeonGenerator::ComputeHallways(const std::vector<DungeonArea>& dungeonRooms, const std::vector<Edge>& graph)
{
	std::vector<HallwayAabb> hallwayAabbs{};
	//------------ Compute Hallway Lines --------------//
	int directionRand{};
	float corridorHalfSize{ m_TileSize * m_CorridorTileSize / 2.f };

	for (const Edge& edge : graph)
	{
		const Rectp& r1Shape{ dungeonRooms[edge.room1].GetAabb() };
		const Rectp& r2Shape{ dungeonRooms[edge.room2].GetAabb() };

		Point2f midPoint{ (r2Shape.center.x + r1Shape.center.x) / 2.f, (r2Shape.center.y + r1Shape.center.y) / 2.f };

		if (abs(midPoint.x - r1Shape.center.x) <= r1Shape.halfWidth
			&& abs(midPoint.x - r2Shape.center.x) <= r2Shape.halfWidth)
		{
			hallwayAabbs.push_back({ Rectp{ midPoint, corridorHalfSize, abs(midPoint.y - r1Shape.center.y) }, false });
		}
		else if (abs(midPoint.y - r1Shape.center.y) <= r1Shape.halfHeight
			&& abs(midPoint.y - r2Shape.center.y) <= r2Shape.halfHeight)
		{
			hallwayAabbs.push_back({ Rectp{ midPoint, abs(midPoint.x - r1Shape.center.x), corridorHalfSize }, true });
		}
		else
		{
			Point2f p1{}, p2{};

			float xHalfDiff{ abs((r1Shape.center.x - r2Shape.center.x) / 2) };
			float yHalfDiff{ abs((r1Shape.center.y - r2Shape.center.y) / 2) };

			if (rand() % 2)
			{
				p1.y = r1Shape.center.y;
				p2.x = r2Shape.center.x;
			}
			else
			{
				p1.y = r2Shape.center.y;
				p2.x = r1Shape.center.x;
			}

			p1.x = midPoint.x;
			p2.y = midPoint.y;

			hallwayAabbs.push_back({ Rectp{ p1, xHalfDiff + corridorHalfSize, corridorHalfSize }, true });
			hallwayAabbs.push_back({ Rectp{ p2, corridorHalfSize, yHalfDiff + corridorHalfSize }, false });
		}
	}

	return hallwayAabbs;
}

void DungeonGenerator::FecthHallwayRooms(std::vector<Rectp>& cells, std::vector<DungeonArea>& dungeonRooms, const std::vector<HallwayAabb>& hallways)
{
	const int minOverlapTiles{ m_CorridorTileSize / 2 };
	const HallwayAabb* currentHallway{ nullptr };

	auto cellCheckPred = [&dungeonRooms, &currentHallway, minOverlapTiles](const Rectp& cell)
	{
		if (utils::IsOverlapping(cell, currentHallway->aabb))
		{
			float overlapLeft{ std::max(cell.center.x - cell.halfWidth, currentHallway->aabb.center.x - currentHallway->aabb.halfWidth) };
			float overlapRight{ std::min(cell.center.x + cell.halfWidth, currentHallway->aabb.center.x + currentHallway->aabb.halfWidth) };
			float overlapBot{ std::max(cell.center.y - cell.halfHeight, currentHallway->aabb.center.y - currentHallway->aabb.halfHeight) };
			float overlapTop{ std::min(cell.center.y + cell.halfHeight, currentHallway->aabb.center.y + currentHallway->aabb.halfHeight) };

			if (overlapRight - overlapLeft > minOverlapTiles * m_TileSize
				|| overlapTop - overlapBot > minOverlapTiles * m_TileSize)
			{
				dungeonRooms.push_back(DungeonArea(cell.center, cell.halfWidth, cell.halfHeight, DungeonArea::DungeonAreaType::subRoom));
				return true;
			}
		}

		return false;
	};

	for (const HallwayAabb& hallway : hallways)
	{
		currentHallway = &hallway;
		cells.erase(std::remove_if(cells.begin(), cells.end(), cellCheckPred), cells.end());
	}
}

void DungeonGenerator::CarveMap(const std::vector<DungeonArea>& dungeonRooms, const std::vector<HallwayAabb>& hallways, std::vector<TileType>& dungeonGrid, Point2f& gridBotLeft, int& gridColumns, int& gridRows)
{
	float left{ INFINITY }, right{ -INFINITY }, bot{ INFINITY }, top{ -INFINITY };
	for (const DungeonArea& area : dungeonRooms)
	{
		const Rectp& areaShape{ area.GetAabb() };
		if (left > areaShape.center.x - areaShape.halfWidth)
			left = areaShape.center.x - areaShape.halfWidth;

		if (right < areaShape.center.x + areaShape.halfWidth)
			right = areaShape.center.x + areaShape.halfWidth;

		if (bot > areaShape.center.y - areaShape.halfHeight)
			bot = areaShape.center.y - areaShape.halfHeight;

		if (top < areaShape.center.y + areaShape.halfHeight)
			top = areaShape.center.y + areaShape.halfHeight;
	}

	gridBotLeft = Point2f{ left , bot };
	gridColumns = int(right - left) / m_TileSize;
	gridRows = int(top - bot) / m_TileSize;

	dungeonGrid.resize(size_t(gridColumns) * size_t(gridRows), TileType::none);

	int roomleft{}, roomBot{}, width{}, height{};
	int start{}, end{};
	for (const DungeonArea& area : dungeonRooms)
	{
		const Rectp& areaShape{ area.GetAabb() };
		roomleft = int((areaShape.center.x - areaShape.halfWidth - left) / m_TileSize);
		width = int(areaShape.halfWidth * 2 / m_TileSize);
		roomBot = int((areaShape.center.y - areaShape.halfHeight - bot) / m_TileSize);
		height = int(areaShape.halfHeight * 2 / m_TileSize);
		for (int row{ roomBot }; row < roomBot + height; ++row)
		{
			start = row * gridColumns + roomleft;
			end = start + width;

			if (row == roomBot || row == roomBot + height - 1)
			{
				std::fill(dungeonGrid.begin() + start, dungeonGrid.begin() + end, TileType::wall);
			}
			else
			{
				dungeonGrid[start] = TileType::wall;
				std::fill(dungeonGrid.begin() + start + 1, dungeonGrid.begin() + end - 1, TileType::walkable);
				dungeonGrid[size_t(end) - size_t(1)] = TileType::wall;
			}
		}
	}

	int idx{};
	for (const HallwayAabb& hallway : hallways)
	{
		roomleft = int((hallway.aabb.center.x - hallway.aabb.halfWidth - left) / m_TileSize);
		width = int(hallway.aabb.halfWidth * 2 / m_TileSize);
		roomBot = int((hallway.aabb.center.y - hallway.aabb.halfHeight - bot) / m_TileSize);
		height = int(hallway.aabb.halfHeight * 2 / m_TileSize);

		if ((hallway.isHorizontal && hallway.aabb.center.y - int(hallway.aabb.center.y) < 0.01f)
			|| (!hallway.isHorizontal && hallway.aabb.center.x - int(hallway.aabb.center.x) < 0.01f))
		{
			++height;
			++width;
		}

		for (int row{ roomBot }; row < roomBot + height; ++row)
		{
			for (int col{ roomleft }; col < roomleft + width; ++col)
			{
				idx = row * gridColumns + col;

				if (dungeonGrid[idx] != TileType::walkable)
				{
					if (row == roomBot)
					{
						dungeonGrid[idx] = TileType::wall;

						if (dungeonGrid[size_t(idx) - gridColumns] == TileType::walkable)
							dungeonGrid[idx] = TileType::walkable;
						
					}
					else if (row == roomBot + height - 1)
					{
						dungeonGrid[idx] = TileType::wall;

						if (dungeonGrid[size_t(idx) + gridColumns] == TileType::walkable)
							dungeonGrid[idx] = TileType::walkable;
					}
					else if (col == roomleft)
					{
						dungeonGrid[idx] = TileType::wall;
						
						if (dungeonGrid[size_t(idx) - 1] == TileType::walkable)
							dungeonGrid[idx] = TileType::walkable;
					}
					else if (col == roomleft + width - 1)
					{
						dungeonGrid[idx] = TileType::wall;

						if (dungeonGrid[size_t(idx) + 1] == TileType::walkable)
							dungeonGrid[idx] = TileType::walkable;
					}
					else
					{
						dungeonGrid[idx] = TileType::walkable;
					}
				}
			}
		}
	}
}



#pragma endregion