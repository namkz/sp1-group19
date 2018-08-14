#include "monster.h"

extern SDungeonLevel g_sLevel;
extern SGameChar g_sChar;

bool adjacent(COORD sA, COORD sB)
{
	return(sA.X >= sB.X - 1 && sA.Y >= sB.Y - 1 && sA.X <= sB.X + 1 && sA.Y <= sB.Y + 1);
}

void SEntityFlamerTroll::takeTurn()
{
	if(g_sLevel.lineOfSight(g_sChar.m_cLocation, m_cLocation))
	{
		if(adjacent(g_sChar.m_cLocation, m_cLocation))
		{
			sendMessage("The troll hits!");
		}
		else
		{
			sendMessage("The troll screeches!");			
		}
	}
}