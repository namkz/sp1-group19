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
	if(m_iHealth < 0) die();
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

void SEntityFlamerTroll::takeTurn()
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