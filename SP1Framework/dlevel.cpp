#include <fstream>
#include <string>
#include "dlevel.h"

SDungeonFeature* parseChar(char cInput)
{
	switch(cInput)
	{
	case '.': return new SDungeonFeatureFloor('.', 0x07);
	case '#': return new SDungeonFeatureFloor('#', 0x07);
	case '|': return new SDungeonFeatureWall('|', 0x70);
	case '-': return new SDungeonFeatureWall('-', 0x70);
	case '\'': return new SDungeonFeatureWall(' ', 0x07);
	case '+': return new SDungeonFeatureDoor('+', '|', 0x06, true);
	case '=': return new SDungeonFeatureDoor('+', '-', 0x06, true);
	case '!': return new SDungeonFeatureDoor('+', '|', 0x06, false);
	case '1': return new SDungeonFeatureDoor('+', '-', 0x06, false);
	}
	
}

SDungeonLevel::SDungeonLevel(std::string sImportFile)
{
	std::fstream sStream;
	sStream.open(sImportFile);
	if (!sStream.is_open()) exit(1);   // call system to stop, file error!
	std::string sLine;
	for(int i = 0; i < 28 * 80; i++)
	{
		if(i % 80 == 0) std::getline(sStream, sLine);
		m_aapsDungeonFeatures[i%80][i/80] = parseChar(sLine[i%80]);
	}
	generateEntities(1);
}

SDungeonFeature* SDungeonLevel::getFeatureAt(COORD* k)
{
	return m_aapsDungeonFeatures[k->X][k->Y];
};		
SDungeonFeature* SDungeonLevel::getFeatureAt (int iX, int iY)
{
	return m_aapsDungeonFeatures[iX][iY];
};
//Random Spawn Generator
SEntity * getNewEntity(int iDungeonDepth)
{
	switch(iDungeonDepth)
	{
	case 1://level 1
		switch(abs(rand()) % 7) // randomizing between 8 Mobs starting for 0 element
		{
		case 0: return new SEntityGreenSlime;
		case 1: return new SEntityGoblin;
		case 2: return new SEntityPossessedStick;
		case 3: return new SEntityTinyRat;
		case 4: return new SEntityGlowingMushroom;
		case 5: return new SEntityCommonBoar;
		case 6: return new SEntityBigMosquito;
		}
	case 2: // level 2
		switch (abs(rand() % 10)) 
		{
		case 0: return new SEntityBlueSlime;
		case 1: return new SEntityGooglyEyes;
		case 2: return new SEntityBouncyBall;
		case 3: return new SEntityCrazyRabbit;
		case 4: return new SEntityLostSoul;
		case 5: return new SEntityFireSalamander;
		case 6: return new SEntityWarningSign;
		case 7: return new SEntityLargeRat;
		case 8: return new SEntityPuppy;
		case 9: return new SEntityBeast;
		}
	case 3: // level 3
	{
		switch (abs(rand() % 2))
		{
		case 0: return new SEntityRedSlime;
		case 1: return new SEntityDerangedWolf;
		}
	}
	}
}

void SDungeonLevel::generateEntities(int iDungeonDepth)
{
	int iEntitiesRemaining = iDungeonDepth + 3;
	for(int i = 0; i < 80 * 28; i++)
	{
		if(m_aapsDungeonFeatures[i%80][i/80]->canBeMovedInto()) 
		{
			if(rand() % 2500 <= 10 + (iEntitiesRemaining) * 7)
			{
				SEntity *sEntity = getNewEntity(iDungeonDepth);
				sEntity->m_cLocation.X = i%80;
				sEntity->m_cLocation.Y = i/80;
				m_sEnemies.addEntity(sEntity);
			}
		}
	}
}

SDungeonLevel::~SDungeonLevel()
{
	for(int i = 0; i < 28 * 80; i++)
	{
		delete m_aapsDungeonFeatures[i%80][i/80];
	}
}

bool SDungeonLevel::isUnoccupied(COORD c)
{
	return !hasEnemy(c) && getFeatureAt(&c)->canBeMovedInto();
}

bool SDungeonLevel::hasEnemy(COORD c)
{
	for(SEntity *sEntity : m_sEnemies)
	{
		if(sEntity == nullptr) continue;
		if(sEntity->m_cLocation.X == c.X && sEntity->m_cLocation.Y == c.Y) return true;
	}
	return false;
}

bool SDungeonLevel::lineOfSight(COORD sA, COORD sB)
{
	double dAX = sA.X + 0.5, dAY = sA.Y + 0.5, dBX = sB.X + 0.5, dBY = sB.Y + 0.5;
	double dDeltaX = dBX - dAX, dDeltaY = dBY - dAY;
	double dInterpolateX = dAX, dInterpolateY = dAY;
	int distance = floor(sqrt((dBX-dAX)*(dBX-dAX)+(dBY+dAY)*(dBY+dAY)));
	for(short s = 0; s < distance * 2; s++)
	{
		dInterpolateX += dDeltaX / (distance * 2);
		dInterpolateY += dDeltaY / (distance * 2);
		if(!(getFeatureAt(int(dInterpolateX), int(dInterpolateY))->transparent()))
		{
			return false;
		}
	}
	return true;
}