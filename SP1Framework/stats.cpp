#include "stats.h"
#include "game.h"

extern SGameChar g_sChar;

void levelUp()
{
	g_sChar.m_iLevel++;
	g_sChar.m_iMaxPlayerAttack = ((pow(g_sChar.m_iLevel, 2) / 2) + (g_sChar.m_iLevel * 5) + 10);
	g_sChar.m_iMaxPlayerDefense = g_sChar.m_iMaxPlayerAttack / 1.5;
	g_sChar.m_iMaxPlayerMana = (pow((5* g_sChar.m_iLevel),2) + 100);
	g_sChar.m_iMaxPlayerHealth = g_sChar.m_iMaxPlayerMana;
	// Set player's health and mana as the new max
} 
