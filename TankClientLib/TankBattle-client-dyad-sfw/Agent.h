#pragma once
#include "TankBattleHeaders.h"
#include "TankBattleNet.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Graph.h"

#define DEGREES 57.2958f

class TankAgent
{
public:
	tankNet::TankBattleStateData prevBSD, curBSD;
	tankNet::TankBattleCommand TBC;

	enum cannonState { SEARCH, AIM };
	enum tankState   { ROAM, CHASE };

	cannonState cS;
	tankState tS;

	COR::Vec2 tankXZ;
	COR::Vec2 prevTankXZ;
	COR::Vec2 tankForwardXZ;
	COR::Vec2 cannonForwardXZ;
	COR::Vec2 enemyXZ;

	int timer;

	float curHealth;
	float prevHealth;

	bool turned;

	void cSearch();
	void cTarget();

	void tRoam();
	void tChase();

public:
	tankNet::TankBattleCommand update(const tankNet::TankBattleStateData &state);
};