#ifndef _DLEVEL_H
#define _DLEVEL_H

#include "monster.h"
#include "Framework\console.h"
#include <string>

enum ELevelType {LT_ROOMS, LT_MAZE, LT_ROOMS_WITH_MAZE, LT_COUNT};

class SDungeonFeature
{
	public: 
		char m_cMapChar;
		unsigned char m_ucFlags;
		SDungeonFeature()
		{
			m_cMapChar = ' ';
		};

		char getMapChar()
		{
			return m_cMapChar;
		}

		SDungeonFeature(char cMapChar)
		{
			m_cMapChar = cMapChar;
		};

		~SDungeonFeature()
		{
		
		};

		virtual bool onMovedInto() {return false;};
};

class SDungeonFeatureWall : public SDungeonFeature
{
	public:	
		SDungeonFeatureWall(char cMapChar)
		{
			m_cMapChar = cMapChar;
		};
		bool onMovedInto()
		{
			return false;
		};
};

class SDungeonFeatureFloor : public SDungeonFeature
{
	public:	
		SDungeonFeatureFloor(char cMapChar)
		{
			m_cMapChar = cMapChar;
		};
		bool onMovedInto()
		{
			return true;
		};
};

class SDungeonFeatureDoor : public SDungeonFeature
{
	private:
		char m_cClosedChar;
		char m_cOpenChar;
	public: 
		SDungeonFeatureDoor(char cClosedChar, char cOpenChar)
		{
			m_cMapChar = cClosedChar;
			m_ucFlags = 0x02;
			m_cClosedChar = cClosedChar;
			m_cOpenChar = cOpenChar;
		};
		//flags bit 1: can be opened by hand, flags bit 0: can be walked through
		bool onMovedInto()
		{
			if(m_ucFlags & 0x01) return true;
			else return doorOpen();
		};
		bool doorOpen()
		{
			if(m_ucFlags & 0x02) m_cMapChar = m_cOpenChar;
			return m_ucFlags & 0x02;
		};
		bool doorClose()
		{
			m_cMapChar = m_cClosedChar;
		};
};

class SDungeonLevel
{
	private:
		SDungeonFeature* aapsDungeonFeatures[80][28];
	public: 
		SDungeonLevel(std::string sImportFile);
		SDungeonFeature* getFeatureAt (COORD* k)
		{
			return aapsDungeonFeatures[k->X][k->Y];
		};		
		SDungeonFeature* getFeatureAt (int iX, int iY)
		{
			return aapsDungeonFeatures[iX][iY];
		};
		~SDungeonLevel();
};

#endif