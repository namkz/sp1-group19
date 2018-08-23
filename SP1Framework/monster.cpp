#include "monster.h"
#include "game.h"
#include "plevel.h"
#include "entity.h"

extern SDungeonLevel * g_sLevel;
extern SGameChar g_sChar;

bool adjacent(COORD sA, COORD sB)
{
	return(sA.X >= sB.X - 1 && sA.Y >= sB.Y - 1 && sA.X <= sB.X + 1 && sA.Y <= sB.Y + 1);
}

int getEightDirectionOf(COORD cInput, COORD cTarget)
{
	double iDeltaX = cTarget.X - cInput.X;
	double iDeltaY = cTarget.Y - cInput.Y;	
	return int((atan2(iDeltaY, iDeltaX))/ (0.78539816339) + 8.5) % 8;
}

void SDamagePacket::printHitMessage()
{
	if(m_sHitMessage.length() != 0) sendMessage(m_sHitMessage);
}


COORD nStepsIn(COORD cInput, int iN, int iDirection)
{
	COORD cNewLocation = cInput;
	iDirection %= 8;
	if(iDirection >= 1 && iDirection <= 3) cNewLocation.Y+=iN;
	if(iDirection >= 3 && iDirection <= 5) cNewLocation.X-=iN;
	if(iDirection >= 5 && iDirection <= 7) cNewLocation.Y-=iN;
	if(iDirection >= 7 || iDirection <= 1) cNewLocation.X+=iN;
	return cNewLocation;
}

void SEntity::takeDamage(SDamagePacket *sDamage)
{
	m_iHealth -= sDamage->m_iDamage;
	sDamage->printHitMessage();
	if(m_iHealth <= 0) die();
}
	
void SEntity::die()
{
	 m_bAlive = false;
	 playerLevel(2);
}


void SEntity::moveTowards(COORD c, bool bTryOtherPaths)
{
	COORD cNewLocation = nStepsIn(m_cLocation,1,getEightDirectionOf(m_cLocation, c));
	COORD cNewLocationLess = nStepsIn(m_cLocation,1,getEightDirectionOf(m_cLocation, c)-1);
	COORD cNewLocationMore = nStepsIn(m_cLocation,1,getEightDirectionOf(m_cLocation, c)+1);
	if(g_sLevel->isUnoccupied(cNewLocation))
	{
		m_cLocation = cNewLocation;
		return;
	}
	if(bTryOtherPaths)
	{
		if(g_sLevel->isUnoccupied(cNewLocationLess)) 
		{
			m_cLocation = cNewLocationLess;
			return;
		}
		if(g_sLevel->isUnoccupied(cNewLocationMore)) 
		{
			m_cLocation = cNewLocationMore;
			return;
		}
	}
}
//Level 1
void SEntityFlamerTroll::takeTurn() //Test monster might spawn later levels.
{
	if(!m_bAlive) return;
	if(g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if(adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if(g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if(m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if(m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityFlamerTroll::attack(SEntity* sTarget)
{
}
void SEntityFlamerTroll::die()
{
	sendMessage("The flamer troll swears unintelligibly at you as it dies!");
	m_bAlive = false;
	playerLevel(4);
	//player gainXP here as well!!
}


void SEntityGreenSlime::takeTurn() // Green Slime for level 1
{
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
	if (!m_bAlive) return;
	m_cJumpState = (m_cJumpState + 1 )%10;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			if(m_cJumpState >= 8) moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			if(m_cJumpState >= 8) moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			m_cJumpState = (m_cJumpState - 1);
		}
	}
}
void SEntityGreenSlime::attack(SEntity* sTarget)
{
	if(adjacent(m_cLocation, g_sChar.m_cLocation))
	{
		g_sChar.takeDamage(new SDamagePacket(2, E_NONE, "The green slime", false));
	}
}
void SEntityGreenSlime::die()
{
	g_sChar.m_iScore += 2;
	m_bAlive = false;
	playerLevel(4);
}

void SEntityGoblin::takeTurn() // Goblin Spawn for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityGoblin::attack(SEntity* sTarget)
{
}
void SEntityGoblin::die()
{
	g_sChar.m_iScore += 3;
	m_bAlive = false;
	playerLevel(5);
}

void SEntityPossessedStick::takeTurn()//Possessed Stick Spawn for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityPossessedStick::attack(SEntity* sTarget)
{
	if(adjacent(m_cLocation, g_sChar.m_cLocation))
	{
		g_sChar.takeDamage(new SDamagePacket(13, E_NONE, "The possessed stick", false));
	}
}
void SEntityPossessedStick::die()
{
	g_sChar.m_iScore += 5;
	
	m_bAlive = false;
	playerLevel(5);
}

void SEntityTinyRat::takeTurn()//Tiny rat spawn for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityTinyRat::attack(SEntity* sTarget)
{ 
		g_sChar.takeDamage(new SDamagePacket(13, E_NONE, "The tiny rat bites!", "The tiny rat misses!", ""));
}
void SEntityTinyRat::die()
{
	g_sChar.m_iScore += 3;
	m_bAlive = false;
	playerLevel(4);
}

void SEntityRedSnail::takeTurn() // Large Snail for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityRedSnail::attack(SEntity* sTarget)
{
}
void SEntityRedSnail::die()
{
	g_sChar.m_iScore += 4;
	m_bAlive = false;
	playerLevel(4);
}

void SEntityGlowingMushroom::takeTurn()//Glowing Mushroom Spawn for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityGlowingMushroom::attack(SEntity* sTarget)
{
}
void SEntityGlowingMushroom::die()
{
	g_sChar.m_iScore += 3;
	m_bAlive = false;
	playerLevel(4);
}

void SEntityBoar::takeTurn()//Common Boar Spawn for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityBoar::attack(SEntity* sTarget){}

void SEntityBoar::die()
{
	g_sChar.m_iScore += 5;
	m_bAlive = false;
	playerLevel(4);
}

void SEntityMosquito::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityMosquito::attack(SEntity* sTarget)
{
}
void SEntityMosquito::die()
{
	g_sChar.m_iScore += 6;
	m_bAlive = false;
	playerLevel(4);
}
//Level 2
void SEntityBlueSlime::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityBlueSlime::attack(SEntity* sTarget)
{
}
void SEntityBlueSlime::die()
{
	g_sChar.m_iScore += 8;
	m_bAlive = false;
	playerLevel(4);
}

void SEntityGooglyEyes::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityGooglyEyes::attack(SEntity* sTarget)
{
}
void SEntityGooglyEyes::die()
{
	g_sChar.m_iScore += 8;
	m_bAlive = false;
	playerLevel(4);
}

void SEntityBouncyBall::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityBouncyBall::attack(SEntity* sTarget)
{
}
void SEntityBouncyBall::die()
{
	g_sChar.m_iScore += 9;
	m_bAlive = false;
	playerLevel(4);
}

void SEntityMadRabbit::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityMadRabbit::attack(SEntity* sTarget)
{
}
void SEntityMadRabbit::die()
{
	g_sChar.m_iScore += 7;
	m_bAlive = false;
	playerLevel(4);
}


void SEntityLostSoul::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityLostSoul::attack(SEntity* sTarget)
{
}
void SEntityLostSoul::die()
{
	g_sChar.m_iScore += 7;
	m_bAlive = false;
	playerLevel(4);
}

void SEntityFireSalamander::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityFireSalamander::attack(SEntity* sTarget)
{
}
void SEntityFireSalamander::die()
{
	g_sChar.m_iScore += 9;
	m_bAlive = false;
	playerLevel(4);
}
/*
void SEntityWarningSign::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityWarningSign::attack(SEntity* sTarget)
{
}*/
void SEntityWarningSign::die()
{
	g_sChar.m_iScore -= 10;
	m_bAlive = false;
	playerLevel(4);
}

void SEntityLargeRat::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityLargeRat::attack(SEntity* sTarget)
{
}
void SEntityLargeRat::die()
{
	g_sChar.m_iScore += 10;
	m_bAlive = false;
	playerLevel(4);
}

void SEntityPuppy::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityPuppy::attack(SEntity* sTarget)
{
}
void SEntityPuppy::die()
{
	g_sChar.m_iScore -= 15;
}

void SEntityBeast::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityBeast::attack(SEntity* sTarget)
{
}
void SEntityBeast::die()
	{
	g_sChar.m_iScore += 20;
	}
//Level 3
void SEntityRedSlime::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityRedSlime::attack(SEntity* sTarget)
{
}
void SEntityRedSlime::die()
{
	g_sChar.m_iScore += 20;
}

void SEntitySpearGoblin::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntitySpearGoblin::attack(SEntity* sTarget)
{
}
void SEntitySpearGoblin::die()
{
	g_sChar.m_iScore += 18;
}

void SEntityDerangedWolf::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityDerangedWolf::attack(SEntity* sTarget)
{
}
void SEntityDerangedWolf::die()
{
	g_sChar.m_iScore += 19;
}

void SEntitySkeletalWarrior::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntitySkeletalWarrior::attack(SEntity* sTarget)
{
}
void SEntitySkeletalWarrior::die()
{
	g_sChar.m_iScore += 21;
}

void SEntitySkeletalArcher::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntitySkeletalArcher::attack(SEntity* sTarget)
{
}
void SEntitySkeletalArcher::die()
{
	g_sChar.m_iScore += 17;
}

void SEntityIrritatedGhost::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityIrritatedGhost::attack(SEntity* sTarget)
{
}
void SEntityIrritatedGhost::die()
{
	g_sChar.m_iScore += 23;
}

void SEntityChameleon::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityChameleon::attack(SEntity* sTarget)
{
}
void SEntityChameleon::die()
{
	m_bAlive = false;
	g_sChar.m_iScore += 26;
}

void SEntityEvenLargerRat::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityEvenLargerRat::attack(SEntity* sTarget)
{
}
void SEntityEvenLargerRat::die()
{
	m_bAlive = false;
	g_sChar.m_iScore += 23;
}

void SEntityMinorLightningElemental::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityMinorLightningElemental::attack(SEntity* sTarget)
{
}
void SEntityMinorLightningElemental::die()
{
	m_bAlive = false;
	g_sChar.m_iScore += 30;
}

void SEntityLich::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityLich::attack(SEntity* sTarget)
{
}
void SEntityLich::die()
{
	m_bAlive = false;
	g_sChar.m_iScore += 40;
}
//Level 4
void SEntityFrostKobold::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityFrostKobold::attack(SEntity* sTarget)
{
}
void SEntityFrostKobold::die()
{
	m_bAlive = false;
	g_sChar.m_iScore += 33;
}

void SEntityOrcWarrior::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityOrcWarrior::attack(SEntity* sTarget)
{
}
void SEntityOrcWarrior::die()
{
	m_bAlive = false;
	g_sChar.m_iScore += 34;
}

void SEntityGoblinWolfrider::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityGoblinWolfrider::attack(SEntity* sTarget)
{
}
void SEntityGoblinWolfrider::die()
{
	m_bAlive = false;
	g_sChar.m_iScore += 37;
}

void SEntityGoblinDartShooter::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityGoblinDartShooter::attack(SEntity* sTarget)
{
}
void SEntityGoblinDartShooter::die()
{
	m_bAlive = false;
	g_sChar.m_iScore += 37;
}

void SEntityBabyTroll::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityBabyTroll::attack(SEntity* sTarget)
{
}
void SEntityBabyTroll::die()
{
	m_bAlive = false;
	g_sChar.m_iScore += 40;
}

void SEntityDrunkGoblin::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityDrunkGoblin::attack(SEntity* sTarget)
{
}
void SEntityDrunkGoblin::die()
{
	m_bAlive = false;
	g_sChar.m_iScore += 48;
}

void SEntityWisp::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityWisp::attack(SEntity* sTarget)
{
}
void SEntityWisp::die()
{
	m_bAlive = false;
	g_sChar.m_iScore += 50;
}

void SEntityPurpleSlime::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityPurpleSlime::attack(SEntity* sTarget)
{
}
void SEntityPurpleSlime::die()
{
	m_bAlive = false;
	g_sChar.m_iScore += 55;
}

void SEntityWizard::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityWizard::attack(SEntity* sTarget)
{
}
void SEntityWizard::die()
{
	m_bAlive = false;
	g_sChar.m_iScore += 55;
}

void SEntityOrcShaman::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityOrcShaman::attack(SEntity* sTarget)
{
}
void SEntityOrcShaman::die()
{
	m_bAlive = false;
	g_sChar.m_iScore += 55;
}

void SEntityMimic::takeDamage(SDamagePacket* sDamage)
{
	m_iHealth -= sDamage->m_iDamage;
	sDamage->printHitMessage();
	if(m_iHealth <= 0) die();
	m_bHidden = false;
}

void SEntityMimic::takeTurn()
{
	if(m_bHidden) return;
	if(!m_bAlive) return;
	m_cColor = 0x0C;
	m_cMonsterClass = 'm';
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}

void SEntityMimic::attack(SEntity* sTarget)
{
	sTarget->takeDamage(new SDamagePacket(6, E_NONE, "The mimic bites!", "The mimic misses!", ""));
}
void SEntityMimic::die()
{
	g_sChar.m_iScore += 55;
	m_bAlive = false;
}

void SEntityOrcWarchief::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityOrcWarchief::attack(SEntity* sTarget)
{
}
void SEntityOrcWarchief::die()
{
	g_sChar.m_iScore += 55;
}
// Level 5
void SEntityYellowSlime::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityYellowSlime::attack(SEntity* sTarget)
{
}
void SEntityYellowSlime::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityDwarvenWarrior::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityDwarvenWarrior::attack(SEntity* sTarget)
{
}
void SEntityDwarvenWarrior::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityDwarvenBlacksmith::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityDwarvenBlacksmith::attack(SEntity* sTarget)
{
}
void SEntityDwarvenBlacksmith::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityDwarvenLongbowman::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityDwarvenLongbowman::attack(SEntity* sTarget)
{
}
void SEntityDwarvenLongbowman::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityDwarvenShieldbearer::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityDwarvenShieldbearer::attack(SEntity* sTarget)
{
}
void SEntityDwarvenShieldbearer::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityDwarfChief::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityDwarfChief::attack(SEntity* sTarget)
{
}
void SEntityDwarfChief::die()
{
	g_sChar.m_iScore += 55;
}	 

void SEntityGiantWorm::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityGiantWorm::attack(SEntity* sTarget)
{
}
void SEntityGiantWorm::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityAnts::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityAnts::attack(SEntity* sTarget)
{
}
void SEntityAnts::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityIronGolem::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityIronGolem::attack(SEntity* sTarget)
{
}
void SEntityIronGolem::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityEarthDragon::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityEarthDragon::attack(SEntity* sTarget)
{
}
void SEntityEarthDragon::die()
{
	g_sChar.m_iScore += 55;
}

//Level 6
void SEntityMotherSlime::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityMotherSlime::attack(SEntity* sTarget)
{
}
void SEntityMotherSlime::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityEnragedTroll::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityEnragedTroll::attack(SEntity* sTarget)
{
}
void SEntityEnragedTroll::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityGoblinAssassin::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityGoblinAssassin::attack(SEntity* sTarget)
{
}
void SEntityGoblinAssassin::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityArmouredTroll::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityArmouredTroll::attack(SEntity* sTarget)
{
}
void SEntityArmouredTroll::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityHumongousRat::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityHumongousRat::attack(SEntity* sTarget)
{
}
void SEntityHumongousRat::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityLocustSwarm::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityLocustSwarm::attack(SEntity* sTarget)
{
}
void SEntityLocustSwarm::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityMetalScorpion::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityMetalScorpion::attack(SEntity* sTarget)
{
}
void SEntityMetalScorpion::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityTrollChieftain::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityTrollChieftain::attack(SEntity* sTarget)
{
}
void SEntityTrollChieftain::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityJungleSlime::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityJungleSlime::attack(SEntity* sTarget)
{
}
void SEntityJungleSlime::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityElfFighter::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityElfFighter::attack(SEntity* sTarget)
{
}
void SEntityElfFighter::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityElfDuelist::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityElfDuelist::attack(SEntity* sTarget)
{
}
void SEntityElfDuelist::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityElfLongbowman::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityElfLongbowman::attack(SEntity* sTarget)
{
}
void SEntityElfLongbowman::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityElfMage::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityElfMage::attack(SEntity* sTarget)
{
}
void SEntityElfMage::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityWolfFamiliar::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityWolfFamiliar::attack(SEntity* sTarget)
{
}
void SEntityWolfFamiliar::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityHighElfWizard::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityHighElfWizard::attack(SEntity* sTarget)
{
}
void SEntityHighElfWizard::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityDarkElfAssassin::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityDarkElfAssassin::attack(SEntity* sTarget)
{

}
void SEntityDarkElfAssassin::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityElfLeader::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityElfLeader::attack(SEntity* sTarget)
{
}
void SEntityElfLeader::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityAmarok::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityAmarok::attack(SEntity* sTarget)
{
}
void SEntityAmarok::die()
{
	g_sChar.m_iScore += 55;
}

//Level 7 
void SEntityIlluminantSlime::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityIlluminantSlime::attack(SEntity* sTarget)
{
}
void SEntityIlluminantSlime::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityRobo_Rat_3000::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityRobo_Rat_3000::attack(SEntity* sTarget)
{
}
void SEntityRobo_Rat_3000::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityCentaurSpearman::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityCentaurSpearman::attack(SEntity* sTarget)
{
}
void SEntityCentaurSpearman::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityCentaurBowman::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityCentaurBowman::attack(SEntity* sTarget)
{
}
void SEntityCentaurBowman::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityCentaurChampion::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityCentaurChampion::attack(SEntity* sTarget)
{
}
void SEntityCentaurChampion::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityBasilisk::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityBasilisk::attack(SEntity* sTarget)
{
}
void SEntityBasilisk::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityMinotaur::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityMinotaur::attack(SEntity* sTarget)
{
}
void SEntityMinotaur::die()
{
	g_sChar.m_iScore += 55;
}
/*
void SEntitySuspiciousLookingMountain::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntitySuspiciousLookingMountain::attack(SEntity* sTarget)
{
}*/
void SEntitySuspiciousLookingMountain::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityGiantTortoise::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityGiantTortoise::attack(SEntity* sTarget)
{
}
void SEntityGiantTortoise::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityWaterDragon::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityWaterDragon::attack(SEntity* sTarget)
{
}
void SEntityWaterDragon::die()
{
	g_sChar.m_iScore += 55;
}

//Level 9
void SEntityKingSlime::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityKingSlime::attack(SEntity* sTarget)
{
}
void SEntityKingSlime::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityGreaterWaterElemental::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityGreaterWaterElemental::attack(SEntity* sTarget)
{
}
void SEntityGreaterWaterElemental::die()
{
	g_sChar.m_iScore += 55;
}  

/*void SEntityPowderKeg::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}*/
/*void SEntityPowderKeg::attack(SEntity* sTarget)
{
}*/
void SEntityPowderKeg::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityForgottenBlade::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityForgottenBlade::attack(SEntity* sTarget)
{
}
void SEntityForgottenBlade::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityFireDragon::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityFireDragon::attack(SEntity* sTarget)
{
}
void SEntityFireDragon::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityWindDragon::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityWindDragon::attack(SEntity* sTarget)
{
}
void SEntityWindDragon::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityCerberus::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityCerberus::attack(SEntity* sTarget)
{
}
void SEntityCerberus::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityCyclops::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityCyclops::attack(SEntity* sTarget)
{
}
void SEntityCyclops::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityElderDragon::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityElderDragon::attack(SEntity* sTarget)
{
}
void SEntityElderDragon::die()
{
	g_sChar.m_iScore += 55;
}

void SEntityJormungand::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel->lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if (adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if (g_dElapsedTime > m_dNextAttack)
			{
				attack(&g_sChar);
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation, true);
		}
		m_cLastSeenTarget = g_sChar.m_cLocation;
	}
	else
	{
		if (m_cLastSeenTarget.X != -1)
		{
			moveTowards(m_cLastSeenTarget, true);
			if (m_cLocation.X == m_cLastSeenTarget.X && m_cLocation.Y == m_cLastSeenTarget.Y)m_cLastSeenTarget.X = -1;
		}
		else
		{
			moveTowards(nStepsIn(m_cLocation, 5, abs(rand() % 8)), true);
		}
	}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
}
void SEntityJormungand::attack(SEntity* sTarget)
{
}
void SEntityJormungand::die()
{
	g_sChar.m_iScore += 55;
}