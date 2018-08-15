#include "monster.h"
#include "game.h"

extern SDungeonLevel g_sLevel;
extern SGameChar g_sChar;

bool adjacent(COORD sA, COORD sB)
{
	return(sA.X >= sB.X - 1 && sA.Y >= sB.Y - 1 && sA.X <= sB.X + 1 && sA.Y <= sB.Y + 1);
}

void SEntity::moveTowards(COORD c)
{
	double iDeltaX = m_cLocation.X - c.X;
	double iDeltaY = m_cLocation.Y - c.Y;
	COORD cNewLocation = m_cLocation;
	switch((int((atan2(iDeltaY, iDeltaX) - 0.39269908169) / (0.78539816339)) + 4)%8)
	{
	case 0:
	case 8:
		cNewLocation.X++;
		break;
	case 1:
		cNewLocation.X++;
		cNewLocation.Y++;
		break;
	case 2:
		cNewLocation.Y++;
		break;
	case 3:
		cNewLocation.X--;
		cNewLocation.Y++;
		break;
	case 4:
		cNewLocation.X--;
		break;
	case 5:
		cNewLocation.X--;
		cNewLocation.Y--;
		break;
	case 6:
		cNewLocation.Y--;
		break;
	case 7:
		cNewLocation.X++;
		cNewLocation.Y--;
		break;
	}
	if(g_sLevel.getFeatureAt(&cNewLocation)->canBeMovedInto())
	{
		m_cLocation = cNewLocation;
	}
}

void SEntityFlamerTroll::takeTurn()
{
	if(g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if(adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			if(g_dElapsedTime > m_dNextAttack)
			{
				sendMessage("The flamer troll hits!");
				m_dNextAttack = g_dElapsedTime + m_dAttackInterval;
			}
		}
		else
		{
			moveTowards(g_sChar.m_cLocation);
		}
	m_dNextTurn = g_dElapsedTime + m_dTurnInterval;
	}
}
