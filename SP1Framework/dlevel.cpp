#include <fstream>
#include <string>
#include "dlevel.h"

SDungeonFeature* parseChar(char cInput)
{
	switch(cInput)
	{
	case '.': return new SDungeonFeatureFloor('.');
	case '#': return new SDungeonFeatureFloor('#');
	case '|': return new SDungeonFeatureWall('|');
	case '-': return new SDungeonFeatureWall('-');
	case ' ': return new SDungeonFeatureWall(' ');
	case '+': return new SDungeonFeatureDoor('+', '|');
	case '=': return new SDungeonFeatureDoor('+', '-');
	}
	
}

SDungeonLevel::SDungeonLevel(std::string sImportFile)
{
	std::fstream sStream;
	sStream.open(sImportFile);
	if (!sStream.is_open()) exit(1);   // call system to stop
	std::string sLine;
	for(int i = 0; i < 28 * 80; i++)
	{
		if(i % 80 == 0) std::getline(sStream, sLine);
		aapsDungeonFeatures[i%80][i/80] = parseChar(sLine[i%80]);
	}
}


SDungeonLevel::~SDungeonLevel()
{
	for(int i = 0; i < 28 * 80; i++)
	{
		delete aapsDungeonFeatures[i%80][i/80];
	}
}