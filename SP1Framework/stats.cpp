#include "stats.h"
#include "game.h"
extern SGameChar g_sChar;

void levelUp()
{
	g_sChar.m_iLevel++;
	g_sChar.m_iMaxPlayerAttack = g_sChar.m_iLevel * 5;
	g_sChar.m_iMaxPlayerDefense = g_sChar.m_iLevel * 5;
	g_sChar.m_iMaxPlayerMana = g_sChar.m_iLevel * 15;
	g_sChar.m_iMaxPlayerHealth = g_sChar.m_iLevel * 15;
}
