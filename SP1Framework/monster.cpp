#include "monster.h"
#include "game.h"
#include "plevel.h"

extern SDungeonLevel g_sLevel;
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

COORD nStepsIn(COORD cInput, int iN, int iDirection)
{
	COORD cNewLocation = cInput;
	iDirection %= 8;
	if(iDirection >= 1 && iDirection <= 3) cNewLocation.Y++;
	if(iDirection >= 3 && iDirection <= 5) cNewLocation.X--;
	if(iDirection >= 5 && iDirection <= 7) cNewLocation.Y--;
	if(iDirection >= 7 || iDirection <= 1) cNewLocation.X++;
	return cNewLocation;
}

void SEntity::takeDamage(SDamagePacket *sDamage)
{
	m_iHealth -= sDamage->m_iDamage;
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
	if(g_sLevel.isUnoccupied(cNewLocation))
	{
		m_cLocation = cNewLocation;
		return;
	}
	if(bTryOtherPaths)
	{
		if(g_sLevel.isUnoccupied(cNewLocationLess)) 
		{
			m_cLocation = cNewLocationLess;
			return;
		}
		if(g_sLevel.isUnoccupied(cNewLocationMore)) 
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
	if(g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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
	g_sChar.m_iScore += 2;
}

void SEntityGreenSlime::takeTurn() // Green Slime for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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
void SEntityGreenSlime::attack(SEntity* sTarget)
{
}
void SEntityGreenSlime::die()
{
	g_sChar.m_iScore += 2;
}

void SEntityGoblin::takeTurn() // Goblin Spawn for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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
	g_sChar.m_iScore += 2;
}

void SEntityPossessedStick::takeTurn()//Possessed Stick Spawn for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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
}
void SEntityPossessedStick::die()
{
	g_sChar.m_iScore += 3;
}

void SEntityTinyRat::takeTurn()//Tiny rat spawn for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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
}
void SEntityTinyRat::die()
{
	g_sChar.m_iScore += 3;
}

void SEntityLargeSnail::takeTurn() // Large Snail for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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
void SEntityLargeSnail::attack(SEntity* sTarget)
{
}
void SEntityLargeSnail::die()
{
	g_sChar.m_iScore += 4;
}

void SEntityGlowingMushroom::takeTurn()//Glowing Mushroom Spawn for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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

void SEntityCommonBoar::takeTurn()//Common Boar Spawn for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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
void SEntityCommonBoar::attack(SEntity* sTarget)
{
}

void SEntityBigMosquito::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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
void SEntityBigMosquito::attack(SEntity* sTarget)
{
}
//Level 2
void SEntityBlueSlime::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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

void SEntityGooglyEyes::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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

void SEntityBouncyBall::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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

void SEntityCrazyRabbit::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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
void SEntityCrazyRabbit::attack(SEntity* sTarget)
{
}


void SEntityLostSoul::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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

void SEntityFireSalamander::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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

void SEntityWarningSign::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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
}

void SEntityLargeRat::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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

void SEntityPuppy::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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

void SEntityBeast::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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
//Level 3
void SEntityRedSlime::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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

void SEntitySpearGoblin::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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

void SEntityDerangedWolf::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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

void SEntitySkeletalWarrior::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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

void SEntitySkeletalArcher::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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

void SEntityIrritatedGhost::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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

void SEntityChameleon::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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

void SEntityFrostKobold::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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

void SEntityMinorLightningElemental::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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

void SEntityLich::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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
//Level 4
void SEntityChaosKobold::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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
void SEntityChaosKobold::attack(SEntity* sTarget)
{
}

void SEntityOrcWarrior::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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

void SEntityFeralWolf::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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
void SEntityFeralWolf::attack(SEntity* sTarget)
{
}

void SEntityWisp::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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

void SEntityWisp::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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

void SEntityDelusionalGoblin::takeTurn()//Big Mosquito for level 1
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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
void SEntityDelusionalGoblin::attack(SEntity* sTarget)
{
}

void SEntityYellowSlime::takeTurn()
{
	if (!m_bAlive) return;
	if (g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
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
