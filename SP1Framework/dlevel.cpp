#include <fstream>
#include <string>
#include "dlevel.h"
#include "game.h"
#include <list>
SDungeonFeature* parseChar(char cInput)
{
	switch(cInput)
	{
	case '.': return new SDungeonFeatureFloor('.', 0x07);
	case '#': return new SDungeonFeatureFloor('#', 0x07);
	case 'o': return new SDungeonFeatureMazeDoor(0x1E, '|', '-', '+');
	case 'O': return new SDungeonFeatureMazeDoor(0x0E, '-', '|', '+');
	case 'x': return new SDungeonFeatureMaze(0x3E, ' ', '#');
	case 'X': return new SDungeonFeatureMaze(0x2E, ' ', '#');
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
	m_sExplored = new SVisibilityMap;
	m_sEnemies = SEntityList{};
	for(int i = 0; i < 200; i++)
	{
		m_asRooms[i] = nullptr;
	}
	if (!sStream.is_open()) exit(1);   // call system to stop, file error!
	std::string sLine;
	for(int i = 0; i < 28 * 80; i++)
	{
		if(i % 80 == 0) std::getline(sStream, sLine);
		m_aapsDungeonFeatures[i%80][i/80] = parseChar(sLine[i%80]);
		m_sExplored->setTileVisibility(COORD{short(i%80), short(i/80)}, false
		);
	}
	resolveMazes();
	generateEntities(0);
}


unsigned char choose(unsigned short asChoiceWeights[], unsigned short sSize)
{
	unsigned short sTotal = 0;
	for(unsigned short i = 0; i < sSize; i++)
	{
		sTotal += asChoiceWeights[i] + 10;
	}
	unsigned short sRandom = rand() % sTotal;
	unsigned short sIndex;
	for(sIndex = 0; sIndex < sSize && sRandom > 0; sIndex++)
	{
		sRandom -= asChoiceWeights[sIndex];
	}
	return sIndex;
}

void SDungeonLevel::resolveMazes()
{
	SVisibilityMap * sCheckedTiles = new SVisibilityMap();
	for(short i = 0; i < 28*80; i++)
	{
		if(sCheckedTiles->getTileVisibility(COORD{i%80,i/80})) continue;
		if(m_aapsDungeonFeatures[i%80][i/80]->getMapChar() != '.' && m_aapsDungeonFeatures[i%80][i/80]->getMapChar() != '#') continue;
		SVisibilityMap * sRoomTiles = new SVisibilityMap();
		floodFillRoom(COORD{i%80,i/80}, sRoomTiles, m_aapsDungeonFeatures[i % 80][i / 80]->getMapChar());
		SDungeonRoom * sRoom = new SDungeonRoom(sRoomTiles);
		sCheckedTiles->assimilate(sRoomTiles);
		addRoom(sRoom);
	}
	for(short i = 0; i < 28*80; i++)
	{
 		if(m_aapsDungeonFeatures[i%80][i/80]->m_eType == FT_MAZE)
		{
			if(m_aapsDungeonFeatures[i%80][i/80]->m_ucFlags & 0x10)
			{
				SDungeonRoom * sRoom1 = containingRoom(COORD{i%80 + 1, i/80});
				SDungeonRoom * sRoom2 = containingRoom(COORD{i%80 - 1, i/80});
				SDungeonPassage * sPassage = new SDungeonPassage {sRoom1, sRoom2, m_aapsDungeonFeatures[i%80][i/80]};
				sRoom1->addLink(sPassage);
				sRoom2->addLink(sPassage);
			}
			else
			{
 				SDungeonRoom * sRoom1 = containingRoom(COORD{i%80, i/80 - 1});
				SDungeonRoom * sRoom2 = containingRoom(COORD{i%80, i/80 + 1});
				SDungeonPassage * sPassage = new SDungeonPassage {sRoom1, sRoom2, m_aapsDungeonFeatures[i%80][i/80]};
				sRoom1->addLink(sPassage);
				sRoom2->addLink(sPassage);
			}
		}
	}
	std::list<SDungeonPassage *> vsPassages;
	if(m_asRooms[0] != nullptr)
	{
		m_asRooms[0]->m_bGenRoomVisited = true;
		for(int iI = 0; iI < 16; iI++)
		{
			if(m_asRooms[0]->m_asAdjacents[iI] != nullptr) vsPassages.push_back(m_asRooms[0]->m_asAdjacents[iI]);
		}
		while(vsPassages.size() != 0)
		{
			std::list<SDungeonPassage*>::iterator sSelectedPassageIterator = vsPassages.begin();
			int iIndex = rand();
			iIndex %= vsPassages.size();
			for(int i = 0; i < iIndex; i++) sSelectedPassageIterator++;
			SDungeonPassage* sSelectedPassage = *sSelectedPassageIterator;
			if(sSelectedPassage->m_sRoom1->m_bGenRoomVisited != sSelectedPassage->m_sRoom2->m_bGenRoomVisited)
			{
				sSelectedPassage->m_sPassage->m_ucFlags &= 0xF7; // bit 0x08 to 0
				if(sSelectedPassage->m_sRoom1->m_bGenRoomVisited)
				for(int iI = 0; iI < 16; iI++)
				{
					if(sSelectedPassage->m_sRoom2->m_asAdjacents[iI] != nullptr) vsPassages.push_back(sSelectedPassage->m_sRoom2->m_asAdjacents[iI]);
					sSelectedPassage->m_sRoom2->m_bGenRoomVisited = true;
				}
				else
				for(int iI = 0; iI < 16; iI++)
				{
					if(sSelectedPassage->m_sRoom1->m_asAdjacents[iI] != nullptr) vsPassages.push_back(sSelectedPassage->m_sRoom1->m_asAdjacents[iI]);
					sSelectedPassage->m_sRoom1->m_bGenRoomVisited = true;
				}
			}
			else
			{
				if(rand() % 100 < (sSelectedPassage->m_sPassage->m_ucFlags & 0x20?8:-20)) sSelectedPassage->m_sPassage->m_ucFlags &= 0xF7;
				else sSelectedPassage->m_sPassage->m_ucFlags |= 0x08;
			}
			sSelectedPassage->m_sPassage->update();
			vsPassages.remove(sSelectedPassage);
		}
	}
}

SDungeonRoom * SDungeonLevel::containingRoom(COORD k)
{
	for(unsigned char i = 0; i < 200; i++)
	{
		if(m_asRooms[i] != nullptr) 
		{
			if(m_asRooms[i]->m_sArea->getTileVisibility(k)) 
			{
				return m_asRooms[i];
			}
		}
	}
	return nullptr;
}

void SDungeonLevel::addRoom(SDungeonRoom *sRoom)
{
	for(unsigned char i = 0; i < 200; i++)
	{
		if(m_asRooms[i] == nullptr) 
		{
			m_asRooms[i] = sRoom;
			break;
		}
	}
}


SDungeonFeature* SDungeonLevel::getFeatureAt(COORD* k)
{
return m_aapsDungeonFeatures[k->X][k->Y];
};
SDungeonFeature* SDungeonLevel::getFeatureAt(int iX, int iY)
{
	return m_aapsDungeonFeatures[iX][iY];
};
//Random Spawn Generator
SEntity * getNewEntity(int iDungeonDepth)
{
	switch (iDungeonDepth)
	{
	case 0://level 1
	{
		switch (abs(rand()) % 3) // randomizing between 8 Mobs starting for 0 element
		{
		case 0: return new SEntityMimic;
		case 1: return new SEntityGreenSlime;
		case 2: return new SEntityPossessedStick;
		}
	}
	case 1://level 1
	{
		switch (abs(rand()) % 7) // randomizing between 8 Mobs starting for 0 element
		{
		case 0: return new SEntityGreenSlime;
		case 1: return new SEntityGoblin;
		case 2: return new SEntityPossessedStick;
		case 3: return new SEntityTinyRat;
		case 4: return new SEntityGlowingMushroom;
		case 5: return new SEntityBoar;
		case 6: return new SEntityMosquito;
		}
	}
	case 2: // level 2
	{
		switch (abs(rand() % 10))
		{
		case 0: return new SEntityBlueSlime;
		case 1: return new SEntityGooglyEyes;
		case 2: return new SEntityBouncyBall;
		case 3: return new SEntityMadRabbit;
		case 4: return new SEntityLostSoul;
		case 5: return new SEntityFireSalamander;
		case 6: return new SEntityWarningSign;
		case 7: return new SEntityLargeRat;
		case 8: return new SEntityPuppy;
		case 9: return new SEntityBeast;
		}
	}
	case 3: // level 3
	{
		switch (abs(rand() % 9))
		{
		case 0: return new SEntityRedSlime;
		case 1: return new SEntityDerangedWolf;
		case 2: return new SEntitySkeletalWarrior;
		case 3: return new SEntitySkeletalArcher;
		case 4: return new SEntityIrritatedGhost;
		case 5: return new SEntityChameleon;
		case 6: return new SEntityEvenLargerRat;
		case 7: return new SEntityMinorLightningElemental;
		case 8: return new SEntityLich;
		}
	}
	case 4: // Level 4
	{
		switch (abs(rand() % 12))
		{
		case 0: return new SEntityFrostKobold;
		case 1: return new SEntityOrcWarrior;
		case 2: return new SEntityGoblinWolfrider;
		case 3: return new SEntityGoblinDartShooter;
		case 4: return new SEntityBabyTroll;
		case 5: return new SEntityWisp;
		case 6: return new SEntityDrunkGoblin;
		case 7: return new SEntityPurpleSlime;
		case 8: return new SEntityWizard;
		case 9: return new SEntityMimic;
		case 10: return new SEntityOrcShaman;
		case 11: return new SEntityOrcWarchief;
		}
	}
	case 5: //Level 5
	{
		switch (abs(rand() % 11))
		{
		case 0: return new SEntityYellowSlime;
		case 1: return new SEntityDwarvenWarrior;
		case 2: return new SEntityDwarvenLongbowman;
		case 3: return new SEntityDwarvenShieldbearer;
		case 4: return new SEntityDwarvenBlacksmith;
		case 5: return new SEntityDwarfChief;
		case 6: return new SEntityGiantWorm;
		case 7: return new SEntityAnts;
		case 8: return new SEntityIronGolem;
		case 9: return new SEntityEarthDragon;
		}
	}
	case 6: // Level 6
	{
		switch (abs(rand() % 8))
		{
		case 0:return new SEntityMotherSlime;
		case 1:return new SEntityEnragedTroll;
		case 2:return new SEntityGoblinAssassin;
		case 3:return new SEntityArmouredTroll;
		case 4:return new SEntityHumongousRat;
		case 5:return new SEntityLocustSwarm;
		case 6:return new SEntityMetalScorpion;
		case 7:return new SEntityTrollChieftain;
		}
	}
	case 7://Level 7
	{
		switch (abs(rand() % 11))
		{
		case 0:return new SEntityJungleSlime;
		case 1:return new SEntityElfFighter;
		case 2:return new SEntityElfDuelist;
		case 3:return new SEntityElfLongbowman;
		case 4:return new SEntityElfMage;
		case 5:return new SEntityWolfFamiliar;
		case 6:return new SEntityHighElfWizard;
		case 7:return new SEntityElfLeader;
		case 8:return new SEntityAmarok;
		case 9:return new SEntityDarkElfAssassin;
		}
	}
	case 8: //Level 8
	{
		switch (abs(rand() % 12))
		{
		case 0: return new SEntityIlluminantSlime;
		case 1: return new SEntityRobo_Rat_3000;
		case 2: return new SEntityCentaurSpearman;
		case 3: return new SEntityCentaurBowman;
		case 4: return new SEntityCentaurChampion;
		case 5: return new SEntityBasilisk;
		case 6: return new SEntityMinotaur;
		case 7: return new SEntitySuspiciousLookingMountain;
		case 8: return new SEntityGiantTortoise;
		case 9: return new SEntityWaterDragon;
		}
	}
	case 9://Level 9
	{
		switch (abs(rand() % 11))
		{
		case 0: return new SEntityKingSlime;
		case 1: return new SEntityGreaterWaterElemental;
		case 2: return new SEntityPowderKeg;
		case 3: return new SEntityForgottenBlade;
		case 4: return new SEntityFireDragon;
		case 5: return new SEntityWindDragon;
		case 6: return new SEntityCerberus;
		case 7: return new SEntityCyclops;
		case 8: return new SEntityElderDragon;
		case 9: return new SEntityJormungand;
		}
	}
	}
}


void SDungeonLevel::generateEntities(int iDungeonDepth)
{
  int iEntitiesRemaining = iDungeonDepth * 2 + 5;
  for(int i = 0; i < 28*80; i++)
  {
	  if(m_aapsDungeonFeatures[i%80][i/80]->getMapChar() == '.' || m_aapsDungeonFeatures[i%80][i/80]->getMapChar() == '#') 
	  {
		if(rand() % 25000 < 10000 + 20 * iEntitiesRemaining)
		{
			SEntity *sEntity = getNewEntity(iDungeonDepth);
			sEntity->m_cLocation.X = i % 80;
			sEntity->m_cLocation.Y = i / 80;
			m_sEnemies.addEntity(sEntity);
		}
	  }
  }	
}

SDungeonLevel::~SDungeonLevel()
{
  for (int i = 0; i < 28 * 80; i++)
  {
    delete m_aapsDungeonFeatures[i % 80][i / 80];
  }
}

bool SDungeonLevel::hasEnemy(COORD c)
{
	for(SEntity * sEntity : m_sEnemies)
	{
		if(sEntity == nullptr) continue;
		if(sEntity->m_cLocation.X == c.X && sEntity->m_cLocation.Y == c.Y) return true;
	}
	return false;
}

SEntity * SDungeonLevel::getEnemyAt(COORD c)
{
	for(SEntity * sEntity : m_sEnemies)
	{
		if(sEntity == nullptr) continue;
		if(sEntity->m_cLocation.X == c.X && sEntity->m_cLocation.Y == c.Y) return sEntity;
	}
	return nullptr;
}


bool SDungeonLevel::isUnoccupied(COORD c)
{
  return !hasEnemy(c) && getFeatureAt(&c)->canBeMovedInto();
}

bool SDungeonLevel::canPlayerSeeEnemy(COORD c)
{
  if(hasEnemy(c) && getEnemyAt(c)->m_bHidden) 
  {
	  sendMessage("Wait! That's " + getEnemyAt(c)->m_sAName + "!");
	  getEnemyAt(c)->m_bHidden = false;
	  return false;
  }
  return !hasEnemy(c);
}

bool SDungeonLevel::lineOfSight(COORD sA, COORD sB)
{
	return lineOfSight(sA, sB, 0.5, 0.5, 0.5, 0.5);
}

bool SDungeonLevel::lineOfSight(COORD sA, COORD sB, double sOffsetXA, double sOffsetYA, double sOffsetXB, double sOffsetYB)
{
	double dAX = sA.X + sOffsetXA, dAY = sA.Y + sOffsetYA, dBX = sB.X + sOffsetXB, dBY = sB.Y + sOffsetYB;
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

void SVisibilityMap::setTileVisibility(COORD c, bool b)
{
	if(b) m_aacTileVisibility[(c.X / 16) * 28 + c.Y] |= (1 << (c.X % 16));
	else m_aacTileVisibility[(c.X / 16) * 28 + c.Y] &= ~(char(1 << (c.X % 16)));
}

bool SVisibilityMap::getTileVisibility(COORD c)
{
	return (m_aacTileVisibility[(c.X / 16) * 28 + c.Y] & 1 << (c.X%16));
};

void SVisibilityMap::assimilate(SVisibilityMap *s)
{
	for(int i = 0; i < 5 * 28; i++)
	{
		m_aacTileVisibility[i] = m_aacTileVisibility[i] | s->m_aacTileVisibility[i];
	}
};

void SVisibilityMap::mask(SVisibilityMap *s)
{
	for(int i = 0; i < 5 * 28; i++)
	{
		m_aacTileVisibility[i] = m_aacTileVisibility[i] & s->m_aacTileVisibility[i];
	}
};

SVisibilityMap* SDungeonLevel::tilesWithLineOfSight(COORD sFrom)
{
	SVisibilityMap *sVisibility = new SVisibilityMap();
	if(getFeatureAt(&sFrom)->getMapChar() == '.')
	{
		floodFillRoom (sFrom, sVisibility);
	}
	SVisibilityMap *sLight = new SVisibilityMap();
	floodFillAdjacent (sFrom, sLight, 4);
	sVisibility->assimilate(sLight);

	return sVisibility;
}

void SDungeonLevel::floodFillRoom(COORD sFrom, SVisibilityMap * sMap, char toSearch)
{
	sMap->setTileVisibility(sFrom, true);
	if(getFeatureAt(&sFrom)->getMapChar() != toSearch) return;
	if(!sMap->getTileVisibility(COORD{sFrom.X + 1, sFrom.Y})) floodFillRoom(COORD{sFrom.X + 1, sFrom.Y}, sMap, toSearch);
	if(!sMap->getTileVisibility(COORD{sFrom.X + 1, sFrom.Y+1})) floodFillRoom(COORD{sFrom.X + 1, sFrom.Y+1}, sMap, toSearch);
	if(!sMap->getTileVisibility(COORD{sFrom.X - 1, sFrom.Y})) floodFillRoom(COORD{sFrom.X - 1, sFrom.Y}, sMap, toSearch);
	if(!sMap->getTileVisibility(COORD{sFrom.X + 1, sFrom.Y-1})) floodFillRoom(COORD{sFrom.X + 1, sFrom.Y-1}, sMap, toSearch);
	if(!sMap->getTileVisibility(COORD{sFrom.X, sFrom.Y + 1})) floodFillRoom(COORD{sFrom.X, sFrom.Y + 1}, sMap, toSearch);
	if(!sMap->getTileVisibility(COORD{sFrom.X - 1, sFrom.Y+1})) floodFillRoom(COORD{sFrom.X - 1, sFrom.Y+1}, sMap, toSearch);
	if(!sMap->getTileVisibility(COORD{sFrom.X, sFrom.Y - 1})) floodFillRoom(COORD{sFrom.X, sFrom.Y - 1}, sMap, toSearch);
	if(!sMap->getTileVisibility(COORD{sFrom.X - 1, sFrom.Y-1})) floodFillRoom(COORD{sFrom.X - 1, sFrom.Y-1}, sMap, toSearch);

}

void SDungeonLevel::floodFillAdjacent(COORD sFrom, SVisibilityMap * sMap, int range)
{
	if(sFrom.X < 0 || sFrom.Y < 0 || sFrom.X > 79 || sFrom.Y > 79) return;
	sMap->setTileVisibility(sFrom, true);
	if(range < 1) return;
	if(!(getFeatureAt(&sFrom)->transparent())) return;
	SVisibilityMap * newSMaps [8] = {};
	for(short i = 0, j = 0; i < 9; i++)
	{
		if (i==4) continue;
		newSMaps[j] = new SVisibilityMap(); 
		if(!sMap->getTileVisibility(COORD{sFrom.X + i % 3 - 1, sFrom.Y + i / 3 - 1})) floodFillAdjacent(COORD{sFrom.X + i % 3 - 1, sFrom.Y + i / 3 - 1}, newSMaps[j], range-1);
		++j;
	}
	for(short i = 0; i < 8; i++)
	{
		sMap->assimilate(newSMaps[i]);
	}
}
