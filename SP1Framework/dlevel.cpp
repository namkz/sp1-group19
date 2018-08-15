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
	SEntity *sEntity = new SEntityFlamerTroll;
	sEntity->m_cLocation = COORD{49, 5};
	m_sEnemies.addEntity(sEntity);
}


SDungeonLevel::~SDungeonLevel()
{
	for(int i = 0; i < 28 * 80; i++)
	{
		delete m_aapsDungeonFeatures[i%80][i/80];
	}
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