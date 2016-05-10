#include "Agent.h"
#include <iostream>

void TankAgent::cSearch()
{
	TBC.cannonMove = tankNet::CannonMovementOptions::RIGHT;
	if (curBSD.tacticoolData[0].inSight && curBSD.tacticoolData[0].isAlive)
	{
		cS = cannonState::AIM;
	}
}

void TankAgent::cTarget()
{
	float dotStuff = COR::dot(cannonForwardXZ, (enemyXZ - tankXZ).normal());
	float perpDotStuff = COR::cross(cannonForwardXZ, (enemyXZ - tankXZ).normal());
	TBC.fireWish = false;

	if (dotStuff < .87f)
	{
		TBC.cannonMove = tankNet::CannonMovementOptions::RIGHT;
	}

	else if (dotStuff > .95f && COR::distance(tankXZ, enemyXZ) <= 12)
	{
		TBC.cannonMove = tankNet::CannonMovementOptions::HALT;
		TBC.fireWish = true;
	}
}

void TankAgent::tRoam()
{
	if (timer <= 0)
	{
		if (turned)
		{
			TBC.tankMove = tankNet::TankMovementOptions::FWRD;
			timer = 60;
			turned = false;
		}
		else if (!turned)
		{
			int randChoice = rand() % 2;
			switch (randChoice)
			{
			case 0: TBC.tankMove = tankNet::TankMovementOptions::RIGHT; timer = 20; break;
			case 1: TBC.tankMove = tankNet::TankMovementOptions::LEFT; timer = 20; break;
			}
			turned = true;
		}
	}

	--timer;

	if (curBSD.tacticoolData[0].inSight)
	{
		timer = 0;
		tS = tankState::CHASE;
	}
}

void TankAgent::tChase()
{
	float dotStuff = COR::dot(tankForwardXZ, (enemyXZ - tankXZ).normal());
	

	if (dotStuff < .80f && curHealth == prevHealth)
	{
		TBC.tankMove = tankNet::TankMovementOptions::RIGHT;
	}
	else if (dotStuff > .80f && curHealth == prevHealth)
	{
		TBC.tankMove = tankNet::TankMovementOptions::FWRD;
	}
	else if (curHealth != prevHealth)
	{
		TBC.tankMove = tankNet::TankMovementOptions::BACK;
	}

	if (!curBSD.tacticoolData[0].inSight)
	{
		++timer;
		if (timer > 60)
		{
			timer = 0;
			tS = tankState::ROAM;
		}
	}
}

tankNet::TankBattleCommand TankAgent::update(const tankNet::TankBattleStateData &state)
{
	prevBSD = curBSD;
	curBSD = state;

	tankXZ = { curBSD.position[0], curBSD.position[2] };
	prevTankXZ = { prevBSD.position[0], prevBSD.position[2] };
	tankForwardXZ = { curBSD.forward[0], curBSD.forward[2] };
	cannonForwardXZ = { curBSD.cannonForward[0], curBSD.cannonForward[2] };
	enemyXZ = { curBSD.tacticoolData[0].lastKnownPosition[0], curBSD.tacticoolData[0].lastKnownPosition[2] };

	prevHealth = prevBSD.currentHealth;
	curHealth = curBSD.currentHealth;

	switch (cS)
	{
	case cannonState::SEARCH: cSearch(); break;
	case cannonState::AIM:    cTarget(); break;
	}

	switch (tS)
	{
	case tankState::CHASE: tChase(); break;
	case tankState::ROAM:  tRoam(); break;
	}

	return TBC;
}