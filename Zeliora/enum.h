#pragma once

enum class GameState
{
	inMenu, init, inGame, paused, playerDead, end, exit
};

enum class TileType
{
	none, walkable, wall, worldobject
};

enum class Direction
{
	down, left, right, up
};

enum class ElementalType
{
	fire, ice, wind, earth
};

enum class MenuType
{
	mainMenu, inGameMenu, endMenu
};

enum class MenuState
{
	back, startGame, next, exitGame
};