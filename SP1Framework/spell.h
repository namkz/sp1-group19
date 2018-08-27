#ifndef _SPELL_H
#define _SPELL_H
#include "game.h"
#include <string>
#include "entity.h"
#include "effect.h"
#include "vect2.h"
#define _CLOSERTHAN(POS1, POS2, DISTANCE) (((POS1.X - POS2.X)*(POS1.X - POS2.X)) + ((POS1.Y - POS2.Y)*(POS1.Y - POS2*Y)) < DISTANCE * DISTANCE)


extern SDungeonLevel * g_sLevel;
extern SGameChar g_sChar;
extern SRenderedEffectList *g_sEffects;

enum ESpellComponents{SC_FIRE, SC_LIGHTNING, SC_AIR, SC_WATER, SC_EARTH, SC_ICE, SC_COUNT, SC_NONE=-1};

class SSpell
{
	public:
		int m_iMPCost;
		EElement m_eElement;
		int m_iDamage;

		virtual void executeSpell();
};

class SSpellNode 
{
	public:
		SSpellNode *m_sNextSpells[SC_COUNT];
		SSpell *m_sSpell;
		SSpellNode()
		{
			for(int i = 0; i < SC_COUNT; i++)
			{
				m_sNextSpells[i] = nullptr;
			}
			m_sSpell = nullptr;
		};
		~SSpellNode()
		{
			for(SSpellNode *sDeleteSpell : m_sNextSpells)
			{
				delete sDeleteSpell;
			}
			delete m_sSpell;
		};
		bool addSpellToTree(SSpell *sNode, enum ESpellComponents aeSequence[]);
		void updateSpellManaCosts();
		SSpell* lookupSpell(ESpellComponents aeSequence[]);
};


class SSpellElementalBasic : public SSpell
{
	std::string m_sName;
	char m_cColor;
	public:	
		SSpellElementalBasic(int iDamage, EElement eElement, int iMPCost, std::string sName, unsigned char cColor)
		{
			 m_iDamage = iDamage;
			 m_eElement = eElement;
			 m_iMPCost = iMPCost;
			 m_sName = sName;
			 m_cColor = cColor;
		}

		void executeSpell()//Cast : 1 of any basic element spell 
		{
			if (g_sChar.m_iMana < 4)
			{
				sendMessage("Insufficient mana to cast " + m_sName);
				return;
			}
			g_sChar.m_iMana -= m_iMPCost;
			for(SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
			{
				if(sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
				if(!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
				SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your " + m_sName), sEntity->m_sTheName); // construct damage packet
				sEntity->takeDamage(sDamage); // deal damage packet
				g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, '*', m_cColor, 0.3)); // draw effect. if you need an effect @ me on discord lmao
				return;
			}
			sendMessage("Your " + m_sName + " glows, then fades.");
		}
};

class SSpellElementalDragonFlame : public SSpell
{
public:
	SSpellElementalDragonFlame(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of any Dual element spell
	{
		if (g_sChar.m_iMana < m_iMPCost) 
		{
			sendMessage("Insufficient mana to cast Dragonflame");
		}
		SVisibilityMap* map = new SVisibilityMap();
		g_sLevel->floodFillAdjacent(COORD{g_sChar.m_cLocation.X + g_sChar.m_sFacingX*(g_sChar.m_sFacingY?1:2), g_sChar.m_cLocation.Y + g_sChar.m_sFacingY*(g_sChar.m_sFacingX?1:2)}, map, 1);
		for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
		{
			if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
			if (!map->getTileVisibility(sEntity->m_cLocation)) continue;
			SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Dragonflame"), sEntity->m_sTheName); // construct damage packet
			sEntity->takeDamage(sDamage); // deal damage packet
			g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, '#', 0x04, 0.3)); // draw effect. if you need an effect @ me on discord lmao
		}
	}
};

class SSpellElementalSteamedHams : public SSpell
{
public:
	SSpellElementalSteamedHams(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of Steamed Hams
	{
		if (g_sChar.m_iMana < 20 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 2)
		{
			sendMessage("Insufficient mana to cast Steamed Hams");
			return;
		}
		for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
		{
			if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
			if ((sEntity->m_cLocation.X != g_sChar.m_cLocation.X + g_sChar.m_sFacingX) || (sEntity->m_cLocation.Y != g_sChar.m_cLocation.Y + g_sChar.m_sFacingY)) // if the entity is not directly in front of the player, skip
			{
				continue;
			}
			SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, "You steamed hams " + sEntity->m_sTheName + "!", "Your steamed hams miss " + sEntity->m_sTheName + "!", ""); // construct damage packet
			sEntity->takeDamage(sDamage); // deal damage packet
			g_sEffects->addEffect(new SEffectParticle(sEntity->m_cLocation, '%', 0x04, 0.9, true, 1)); // draw effect. if you need an effect @ me on discord lmao
			mciSendString(L"close \"steamed_hams.wav\"", NULL, 0, NULL);
			mciSendString(L"open \"steamed_hams.wav\" type waveaudio", NULL, 0, NULL);
			mciSendString(L"play \"steamed_hams.wav\"", NULL, 0, NULL);  
			break; // this for single-target (break after first hit)
		}
	}
};

class SSpellElementalFireStorm : public SSpell
{
public:
	SSpellElementalFireStorm(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of Firestorm
	{
		if (g_sChar.m_iMana < 40 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 4)
		{
			sendMessage("Insufficient mana to cast Firestorm");
			return;
		}
		else
		{
			for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
			{
				if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
				if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
				SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Firestorm"), sEntity->m_sTheName); // construct damage packet
				sEntity->takeDamage(sDamage); // deal damage packet
				g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, '#', 0x04, 0.3)); // draw effect. if you need an effect @ me on discord lmao
				break; // this for single-target (break after first hit)
			}
		}
	}
};

class SSpellElementalBlazingLightning : public SSpell
{
public:
	SSpellElementalBlazingLightning(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of Blazing Lightning
	{
		if (g_sChar.m_iMana < 30 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 3)
		{
			sendMessage("Insuficent mana to cast Blazing Lightning");
			return;
		}
		else
		{
			for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
			{
				if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
				if (angleBetweenVectors(vect2(sEntity->m_cLocation) - vect2(g_sChar.m_cLocation), vect2(g_sChar.m_sFacingX, g_sChar.m_sFacingY)) > 0.02) continue; // if the entity is not in line of sight, skip
				if (!g_sLevel->lineOfSight(g_sChar.m_cLocation, sEntity->m_cLocation)) continue;
				SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your blazing lightning"), sEntity->m_sTheName); // construct damage packet
				sEntity->takeDamage(sDamage); // deal damage packet
				g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, 'z', 0x0E, 0.3)); // draw effect. if you need an effect @ me on discord lmao
				break; // this for single-target (break after first hit)
			}
		}
	}
};

class SSpellElementalFrostFire : public SSpell
{
public:
	SSpellElementalFrostFire(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of FrostFire
	{
		if (g_sChar.m_iMana < 35 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 3.5)
		{
			sendMessage("insufficient Mana to cast FrostFire");
			return;
		}
		else
		{
			for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
			{
				if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
				if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
				SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Frostfire"), sEntity->m_sTheName); // construct damage packet
				sEntity->takeDamage(sDamage); // deal damage packet
				g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, '#', 0x09, 0.3)); // draw effect. if you need an effect @ me on discord lmao
				break; // this for single-target (break after first hit)
			}
		}
	}
};

class SEntityWaterWave : public SEntity
{
	vect2 m_vDirection;
	int m_iDamage;
public: 
	SEntityWaterWave(COORD cPosition, vect2 vDirection, int iDamage)
	{
		m_cLocation = cPosition;
		m_vDirection = vDirection;
		m_iDamage = iDamage;
		m_cColor = 0x09;
		m_cMonsterClass = '/';
		m_dTurnInterval = 0.05;
		m_dNextTurn = 0;
	}
	void takeTurn()
	{
		if(g_sLevel->getFeatureAt(&COORD(vect2(m_cLocation) + m_vDirection))->canBeMovedInto())
		{
			if(g_sLevel->hasEnemy(COORD(vect2(m_cLocation) + m_vDirection)))
			{
				SEntity * sHitEntity = g_sLevel->getEnemyAt(COORD(vect2(m_cLocation) + m_vDirection));
				sHitEntity->moveTowards(COORD(vect2(m_cLocation) + m_vDirection * 2),false);
				sHitEntity->takeDamage(new SDamagePacket(m_iDamage, E_WATER, "Your water wave hits " + sHitEntity->m_sTheName + "!", "This shouldn't happen!", ""));
			}
			moveTowards(COORD(vect2(m_cLocation) + m_vDirection), false);
		}
		else
		{
			die();
		}
		m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
	}
	void die()
	{
		m_bAlive = false;
	}
};

class SSpellElementalWaterWave : public SSpell
{
public:
	SSpellElementalWaterWave(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of WaterWave
	{
		if (g_sChar.m_iMana < 30 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 3)
		{
			sendMessage("Insufficent mana to cast Waterwave");
			return;
		}
		else
		{
			for(int i = -1; i <= 1; i++)
			{
				vect2 vWavePosition = vect2(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y) + vect2(g_sChar.m_sFacingX, g_sChar.m_sFacingY) + i * vect2(-g_sChar.m_sFacingY, g_sChar.m_sFacingX);
				SEntityWaterWave * sWave = new SEntityWaterWave(COORD{SHORT(vWavePosition.dX), SHORT(vWavePosition.dY)}, vect2(g_sChar.m_sFacingX, g_sChar.m_sFacingY), m_iDamage);
				g_sLevel->m_sEnemies.addEntity(sWave);
			}
		}
	}
};

class SSpellElementalQuagmire : public SSpell
{
public:
	SSpellElementalQuagmire(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of Quagmire
	{
		if (g_sChar.m_iMana < 40 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 4)
		{
			sendMessage("Insufficient mana to cast Quagmire");
			return;
		}
		else
		{
			for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
			{
				if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
				if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
				SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Quagmire"), sEntity->m_sTheName); // construct damage packet
				sEntity->takeDamage(sDamage); // deal damage packet
				g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, '&', 0x09, 0.3)); // draw effect. if you need an effect @ me on discord lmao
				break; // this for single-target (break after first hit)
			}
		}
	}
};

class SSpellElementalHurricane : public SSpell
{
public:
	SSpellElementalHurricane(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of Hurricane
	{
		if (g_sChar.m_iMana < 40 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 3)
		{
			sendMessage("Insufficient mana to cast Hurricane");
			return;
		}
		else
		{
			for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
			{
				if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
				if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
				SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Hurricane"), sEntity->m_sTheName); // construct damage packet
				sEntity->takeDamage(sDamage); // deal damage packet
				g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, 'O', 0xAA, 0.3)); // draw effect. if you need an effect @ me on discord lmao
				break; // this for single-target (break after first hit)
			}
		}
	}
};

class SSpellElementalShockwave : public SSpell
{
public:
	SSpellElementalShockwave(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of Shockwave
	{
		if (g_sChar.m_iMana < 25 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 2.5)
		{
			sendMessage("Insufficient mana to cast Shockwave");
			return;
		}
		for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
		{
			if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
			if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
			SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Shockwave"), sEntity->m_sTheName); // construct damage packet
			sEntity->takeDamage(sDamage); // deal damage packet
			g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, 'O', 0x88, 0.3)); // draw effect. if you need an effect @ me on discord lmao
			break; // this for single-target (break after first hit)
		}
	}
};

class SSpellElementalIceTomb : public SSpell
{
public:
	SSpellElementalIceTomb(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of IceTomb
	{
		if (g_sChar.m_iMana < 50 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 5)
		{
			sendMessage("Insufficient mana to cast Icetomb");
			return;
		}
		else
		{
			for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
			{
				if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
				if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
				SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Icetomb"), sEntity->m_sTheName); // construct damage packet
				sEntity->takeDamage(sDamage); // deal damage packet
				g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, 'O', 0x69, 0.3)); // draw effect. if you need an effect @ me on discord lmao
				break; // this for single-target (break after first hit)
			}
		}
	}
};

class SSpellElementalRockArmour : public SSpell
{
public:
	SSpellElementalRockArmour(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of RockArmour
	{
		if (g_sChar.m_iMana < 20 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 2)
		{
			sendMessage("Insufficient mana to cast Rock Armour");
			return;
		}
		else
		{
			for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
			{
				if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
				if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
				SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Rock Armour"), sEntity->m_sTheName); // construct damage packet
				sEntity->takeDamage(sDamage); // deal damage packet
				g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, 'O', 0x69, 0.3)); // draw effect. if you need an effect @ me on discord lmao
				break; // this for single-target (break after first hit)
			}
		}
	}
};

class SSpellElementalMeteorStorm : public SSpell
{
public:
	SSpellElementalMeteorStorm(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of Meteorstorm
	{
		if (g_sChar.m_iMana < 40 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 4)
		{
			sendMessage("Insufficient mana to cast Meteorstorm");
			return;
		}
		else
		{
			for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
			{
				if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
				if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
				SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Meteorstorm"), sEntity->m_sTheName); // construct damage packet
				sEntity->takeDamage(sDamage); // deal damage packet
				g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, 'O', 0xCC, 0.3)); // draw effect. if you need an effect @ me on discord lmao
				break; // this for single-target (break after first hit)
			}
		}
	}
};

class SSpellElementalEffectNegation : public SSpell
{
public:
	SSpellElementalEffectNegation(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of Effect Negation
	{
		if (g_sChar.m_iMana < 20 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 2)
		{
			sendMessage("Insufficient mana to cast Effect Negation");
			return;
		}
		else
		{
			for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
			{
				if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
				if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
				SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Effect negation"), sEntity->m_sTheName); // construct damage packet
				sEntity->takeDamage(sDamage); // deal damage packet
				g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, '*', 0x05, 0.3)); // draw effect. if you need an effect @ me on discord lmao
				break; // this for single-target (break after first hit)
			}
		}
	}
};

class SSpellElementalPitfall : public SSpell
{
public:
	SSpellElementalPitfall(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of Pitfall
	{
		if (g_sChar.m_iMana < 30 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 3)
		{
			sendMessage("Insufficient mana to cast Pitfall");
			return;
		}
		for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
		{
			if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
			if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
			SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Pitfall"), sEntity->m_sTheName); // construct damage packet
			sEntity->takeDamage(sDamage); // deal damage packet
			g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, 'X', 0x02, 0.3)); // draw effect. if you need an effect @ me on discord lmao
			break; // this for single-target (break after first hit)
		}
	}
};

class SSpellElementalDeflectiveBarrier : public SSpell
{
public:
	SSpellElementalDeflectiveBarrier(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of Deflective Barrier
	{
		if (g_sChar.m_iMana < 20 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 2)
		{
			sendMessage("Insufficient mana to cast Deflective Barrier");
			return;
		}
		else
		{
			for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
			{
				if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
				if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
				SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Deflective Barrier"), sEntity->m_sTheName); // construct damage packet
				sEntity->takeDamage(sDamage); // deal damage packet
				g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, 'O', 0x0D, 0.3)); // draw effect. if you need an effect @ me on discord lmao
				break; // this for single-target (break after first hit)
			}
		}
	}
};

class SSpellElementalLightningStorm : public SSpell
{
public:
	SSpellElementalLightningStorm(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of Lightning Storm
	{
		if (g_sChar.m_iMana < 50 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 5)
		{
			sendMessage("Insufficient mana to cast Lightning storm");
			return;
		}
		else
		{
			for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
			{
				if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
				if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
				SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Lightningstorm"), sEntity->m_sTheName); // construct damage packet
				sEntity->takeDamage(sDamage); // deal damage packet
				g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, 'z', 0x0E, 0.3)); // draw effect. if you need an effect @ me on discord lmao
				break; // this for single-target (break after first hit)
			}
		}
	}
};

class SSpellElementalIcicleBarrage : public SSpell
{
public:
	SSpellElementalIcicleBarrage(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of Icicle Barrage
	{
		if (g_sChar.m_iMana < 40 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 4)
		{
			sendMessage("Insufficient mana to cast Icicle Barrage");
			return;
		}
		else
		{
			for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
			{
				if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
				if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
				SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Icicle Barrage"), sEntity->m_sTheName); // construct damage packet
				sEntity->takeDamage(sDamage); // deal damage packet
				g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, '*', 0x0B, 0.3)); // draw effect. if you need an effect @ me on discord lmao
				break; // this for single-target (break after first hit)
			}
		}
	}
};
//Lightning Tree
//Chain Lightning
class SSpellElementalChainLightning : public SSpell
{
public:
	SSpellElementalChainLightning(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of Chain Lightning
	{
		if (g_sChar.m_iMana < 35 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 3.5)
		{
			sendMessage("Insufficient mana to cast Chain Lightning");
			return;
		}
		else
		{
			for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
			{
				if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
				if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
				SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Chain Lightning"), sEntity->m_sTheName); // construct damage packet
				sEntity->takeDamage(sDamage); // deal damage packet
				g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, 'X', 0x02, 0.3)); // draw effect. if you need an effect @ me on discord lmao
				break; // this for single-target (break after first hit)
			}
		}
	}
};
//Everfreezing Paralysis
class SSpellElementalEverlastingParalysis : public SSpell
{
public:
	SSpellElementalEverlastingParalysis(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of Everlasting Paralysis
	{
		if (g_sChar.m_iMana < 50 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 5)
		{
			sendMessage("Insufficient mana to cast Everlasting Paralysis");
			return;
		}
		for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
		{
			if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
			if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
			SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Everlasting paralysis"), sEntity->m_sTheName); // construct damage packet
			sEntity->takeDamage(sDamage); // deal damage packet
			g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, '\\', 0x0E, 0.3)); // draw effect. if you need an effect @ me on discord lmao
			break; // this for single-target (break after first hit)
		}
	}
};
//Ice Tree
//Ice Wall
class SSpellElementalIceWall : public SSpell
{
public:
	SSpellElementalIceWall(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of Ice wall
	{
		if (g_sChar.m_iMana < 50 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 5)
		{
			sendMessage("Insufficient mana to cast Ice Wall");
			return;
		}
		else
		{
			for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
			{
				if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
				if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
				SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Icewall"), sEntity->m_sTheName); // construct damage packet
				sEntity->takeDamage(sDamage); // deal damage packet
				g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, 'X', 0x02, 0.3)); // draw effect. if you need an effect @ me on discord lmao
				break; // this for single-target (break after first hit)
			}
		}
	}
};
//Ultimate Spell Tree
//Sun God Fury
class SSpellElementalSunGodFury : public SSpell
{
public:
	SSpellElementalSunGodFury(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of Sun God Fury
	{
		if (g_sChar.m_iMana < 500 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 80)
		{
			sendMessage("Insufficient mana to cast Sun God Fury");
			return;
		}
		else
		{
			for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
			{
				if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
				if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
				SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Sun God Fury"), sEntity->m_sTheName); // construct damage packet
				sEntity->takeDamage(sDamage); // deal damage packet
				g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, '#', 0x0E, 0.3)); // draw effect. if you need an effect @ me on discord lmao
				break; // this for single-target (break after first hit)
			}
		}
	}
};
//LandSlide
class SSpellElementalLandSlide : public SSpell
{
public:
	SSpellElementalLandSlide(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of LandSlide
	{
		if (g_sChar.m_iMana < 50 + ((double)g_sChar.m_iMaxPlayerMana / 100) * 5)
		{
			sendMessage("Insufficient mana to cast Landslide");
			return;
		}
		else
		{
			for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
			{
				if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
				if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
				SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Landslide"), sEntity->m_sTheName); // construct damage packet
				sEntity->takeDamage(sDamage); // deal damage packet
				g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, '8', 0x02, 0.3)); // draw effect. if you need an effect @ me on discord lmao
				break; // this for single-target (break after first hit)
			}
		}
	}
};
//Ritual of Madness
class SSpellElementalRitualofMadness : public SSpell
{
public:
	SSpellElementalRitualofMadness(double iDamage, EElement eElement, int iMPCost)
	{
		m_iDamage = iDamage;
		m_eElement = eElement;
		m_iMPCost = iMPCost;
	}

	void executeSpell()//Cast : 1 of RitualofMadness
	{
		if (g_sChar.m_iMana < g_sChar.m_iMaxPlayerMana)
		{
			sendMessage("Insufficient mana to cast Ritual of Madness");
			return;
		}
		else
		{
			for (SEntity *sEntity : g_sLevel->m_sEnemies) // loop through all enemies on the map
			{
				if (sEntity == nullptr) continue; // if entity is nonexistent / empty entity slot, skip. to avoid referencing a property of a nullptr this should always be first
				if (!g_sLevel->lineOfSight(sEntity->m_cLocation, g_sChar.m_cLocation)) continue; // if the entity is not in line of sight, skip
				SDamagePacket * sDamage = new SDamagePacket(m_iDamage, m_eElement, std::string("Your Ritual of Madness"), sEntity->m_sTheName); // construct damage packet
				sEntity->takeDamage(sDamage); // deal damage packet
				g_sEffects->addEffect(new SEffectLine(sEntity->m_cLocation, g_sChar.m_cLocation, '0', 0x02, 0.3)); // draw effect. if you need an effect @ me on discord lmao
				break; // this for single-target (break after first hit)
			}
		}
	}
};
#endif
