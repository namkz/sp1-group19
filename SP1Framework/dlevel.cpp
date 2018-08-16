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

SEntity * getNewEntity(int iDungeonDepth)
{
	switch(iDungeonDepth)
	{
	case 1:
		switch(abs(rand()) % 1)
		{
		case 0: return new SEntityFlamerTroll;
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
			if(rand() % 500 <= 10 + (iEntitiesRemaining) * 7)
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