#ifndef _DLEVEL_H
#define _DLEVEL_H

#include "monster.h"
#include "Framework\console.h"
#include <string>

extern void sendMessage(std::string);
extern double g_dElapsedTime;

enum EFeatureType {FT_STANDARD, FT_MAZE};

class SDungeonFeature
{
	public: 
		char m_cMapChar;
		char m_cMapColor;
		unsigned char m_ucFlags;
		EFeatureType m_eType;
		SDungeonFeature()
		{
			m_cMapChar = ' ';
		};

		virtual void update(){};

		char getMapChar()
		{
			return m_cMapChar;
		}
		
		char getMapColor()
		{
			return m_cMapColor;
		}

		SDungeonFeature(char cMapChar)
		{
			m_cMapChar = cMapChar;
		};

		~SDungeonFeature()
		{
		
		};
		
		virtual bool canBeMovedInto() {return true;};
		virtual bool onMovedInto() {return canBeMovedInto();};
		virtual bool transparent() {return true;};
};

class SDungeonFeatureWall : public SDungeonFeature
{
	public:	
		SDungeonFeatureWall(char cMapChar, char cMapColor)
		{
			m_cMapChar = cMapChar;
			m_cMapColor = cMapColor;
			m_eType = FT_STANDARD;
		};
		bool transparent()
		{
			return false;
		};
		bool canBeMovedInto()
		{
			return false;
		};
};

class SDungeonFeatureFloor : public SDungeonFeature
{
	public:	
		SDungeonFeatureFloor(char cMapChar, char cMapColor)
		{
			m_cMapChar = cMapChar;
			m_cMapColor = cMapColor;
			m_eType = FT_STANDARD;
		};
		bool transparent()
		{
			return true;
		};
		bool canBeMovedInto()
		{
			return true;
		};
};

class SDungeonFeatureDoor : public SDungeonFeature
{
	public: 
		char m_cClosedChar;
		char m_cOpenChar;
		double m_dMessageTimeout = -100.0;
		SDungeonFeatureDoor(char cClosedChar = '+', char cOpenChar = 'X', char cMapColor = 0x0c, bool bUnlocked = false)
		{
			m_cMapChar = cClosedChar;
			m_cMapColor = cMapColor;
			m_ucFlags = (bUnlocked?0x02:0x00);
			m_cClosedChar = cClosedChar;
			m_cOpenChar = cOpenChar;
			m_eType = FT_STANDARD;
		};
		//flags bit 0x02: can be opened by hand, flags bit 0x01: can be walked through
		bool onMovedInto()
		{
			doorOpen();
			return canBeMovedInto();
		};
		bool transparent()
		{
			return(m_ucFlags & 0x01);
		}
		bool doorOpen()
		{
			if(m_ucFlags & 0x02) 
			{
				if(!(m_ucFlags & 0x01))
				{
					m_cMapChar = m_cOpenChar;
					m_ucFlags |= 0x01b;
					sendMessage("You open the door.");
				}
			}
			else if(g_dElapsedTime - m_dMessageTimeout > 2.0)
			{				
				sendMessage("The door is locked.");
				m_dMessageTimeout = g_dElapsedTime;
			}
			return m_ucFlags & 0x02;
		};
		bool canBeMovedInto()
		{
			return(m_ucFlags & 0x01);
		}
		bool doorClose()
		{
			m_cMapChar = m_cClosedChar;
		};
};


class SDungeonFeatureMazeDoor : public SDungeonFeatureDoor
{
	private:
		char m_cFilledChar;
	public:
		// flags: 
		// 0x08 = is a wall
		// 0x10 = is horizontally checking (part of vertical wall)
		SDungeonFeatureMazeDoor(unsigned char cFlags, char cFilled, char cOpen, char cClosed)
		{
			m_eType = FT_MAZE;
			m_ucFlags = cFlags;
			m_cFilledChar = cFilled;
			m_cOpenChar = cOpen;
			m_cClosedChar = cClosed;
			m_cMapColor = 0x07;
		}
		bool canBeMovedInto()
		{
			return !(m_ucFlags & 8) || (m_ucFlags & 1); 
		}
		bool transparent()
		{
			return !(m_ucFlags & 8);
		}
		void update()
		{
			if(m_ucFlags & 8)
			{
				m_cMapChar = (m_ucFlags & 16?'|':'-');
				m_cMapColor = 0x70;
				return;
			}
			else
			{
				m_cMapColor = 0x06;
			}
		}
		bool onMovedInto()
		{
			if(!(m_ucFlags & 8))doorOpen();
			return canBeMovedInto();
		}
		bool doorOpen()
		{
			if(m_ucFlags & 0x02) 
			{
				if(!(m_ucFlags & 0x01))
				{
					m_cMapChar = m_cOpenChar;
					m_ucFlags |= 0x01;
					sendMessage("You open the door.");
				}
			}
			else if(g_dElapsedTime - m_dMessageTimeout > 2.0)
			{				
				sendMessage("The door is locked.");
				m_dMessageTimeout = g_dElapsedTime;
			}
			return m_ucFlags & 0x02;
		};
};


class SVisibilityMap
{
	private:
		unsigned short m_aacTileVisibility[140];
	public:
		SVisibilityMap()
		{
			
			for(int i = 0; i < 140; i++)
			{
				m_aacTileVisibility[i] = 0;
			}
		};
		bool getTileVisibility(COORD c);
		void setTileVisibility(COORD c, bool b);
		void assimilate(SVisibilityMap *target);

};

class SDungeonRoom;

struct SDungeonPassage
{
	SDungeonRoom *m_sRoom1;
	SDungeonRoom *m_sRoom2;
	SDungeonFeature *m_sPassage;
};

class SDungeonRoom
{
	public:
		SVisibilityMap * m_sArea;
		SDungeonPassage * m_asAdjacents[16];
		bool m_bGenRoomVisited = false;
		SDungeonRoom(SVisibilityMap * sArea)
		{
			m_sArea = sArea;
			for(int i = 0; i < 16; i++)
			{
				m_asAdjacents[i] = nullptr;
			}
		}
		void addLink(SDungeonPassage * sPassage)
		{
			for(unsigned char i = 0; i < 16; i++)
			{
				if(m_asAdjacents[i] == nullptr) 
				{
					m_asAdjacents[i] = sPassage;
					break;
				}
			}
		}
};

class SDungeonLevel
{
	private:
		SDungeonFeature* m_aapsDungeonFeatures[80][28];
	public: 
		SEntityList m_sEnemies;
		SVisibilityMap * m_sExplored;
		SDungeonRoom * m_asRooms[200];
		
		SDungeonLevel(std::string sImportFile);
		void generateEntities(int iDungeonDepth);
		SDungeonFeature* getFeatureAt (COORD* k);
		SDungeonFeature* getFeatureAt (int iX, int iY);
		void addRoom(SDungeonRoom *);
		SDungeonRoom * containingRoom(COORD k);

		~SDungeonLevel();
		bool isUnoccupied(COORD c);
		bool hasEnemy(COORD c);
		SEntity * getEnemyAt(COORD c);
		bool canPlayerSeeEnemy(COORD c);
		bool lineOfSight(COORD sA, COORD sB);
		bool lineOfSight(COORD sA, COORD sB, double, double, double, double);
		SVisibilityMap* tilesWithLineOfSight(COORD cFrom);
		void floodFillAdjacent(COORD sFrom, SVisibilityMap * sMap, int range);
		void floodFillRoom (COORD sFrom, SVisibilityMap * sMap);
		void resolveMazes();
};

#endif