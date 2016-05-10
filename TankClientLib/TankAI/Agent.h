#pragma once

struct Vec2
{
	float x, y;
};

struct TacData
{
	bool enemySeen;
	Vec2 prevLocation;
	Vec2 prevEnemyLocation;
	Vec2 estSpeed;
	Vec2 estTurnSpeed;
};