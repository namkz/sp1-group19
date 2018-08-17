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
	sendMessage("The flamer troll swears unintelligibly at you as you die");
	//score += 4;
	//player gainXP here as well!!
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

void SEntityRedSnail::takeTurn() // Large Snail for level 1
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

void SEntityRedSnail::attack(SEntity* sTarget)
{
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

void SEntityBoar::takeTurn()//Common Boar Spawn for level 1
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

void SEntityBoar::attack(SEntity* sTarget)
{
}

void SEntityMosquito::takeTurn()//Big Mosquito for level 1
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

void SEntityMosquito::attack(SEntity* sTarget)
{
}
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
void SEntityMadRabbit::takeTurn()//Big Mosquito for level 1
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

void SEntityMadRabbit::attack(SEntity* sTarget)
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