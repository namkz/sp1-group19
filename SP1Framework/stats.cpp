#include "stats.h"
#include "game.h"
extern SGameChar g_sChar;

void enemyHealthUpdate(int spellDamage, int enemyIndex)
{
	SEnemyStats enemy[10];
	if (spellDamage > enemy[enemyIndex].sEnemyDefense)
	{
		enemy[enemyIndex].sEnemyHealth = enemy[enemyIndex].sEnemyHealth - (spellDamage - enemy[enemyIndex].sEnemyDefense);
	}
}

void enemyAttackDebuff(int atkDebuff, int enemyIndex)
{
	SEnemyStats enemy[10];
	if (atkDebuff > 0)
	{
		if (atkDebuff > enemy[enemyIndex].sEnemyAttack)
		{
			enemy[enemyIndex].sEnemyAttack = 0;
		}
		else
		{
			enemy[enemyIndex].sEnemyAttack = enemy[enemyIndex].sEnemyAttack - atkDebuff;
		}
	}
}

void enemyDefenseDebuff(int defDebuff, int enemyIndex)
{
	if (defDebuff > 0)
	{
		if (defDebuff > enemy[enemyIndex])
	}
}

void playerLevelUp(int healthIncrease, int manaIncrease, int attackIncrease, int defenseIncrease)
{
	g_sChar.sMaxPlayerHealth = g_sChar.sMaxPlayerHealth + healthIncrease;
	g_sChar.sMaxPlayerMana = g_sChar.sMaxPlayerMana + manaIncrease;
	g_sChar.sMaxPlayerAttack = g_sChar.sMaxPlayerAttack + attackIncrease;
	g_sChar.sMaxPlayerDefense = g_sChar.sMaxPlayerDefense + defenseIncrease;
	g_sChar.sPlayerHealth = g_sChar.sMaxPlayerHealth;
	g_sChar.sPlayerMana = g_sChar.sMaxPlayerMana;
	g_sChar.sPlayerAttack = g_sChar.sMaxPlayerAttack;
	g_sChar.sPlayerDefense = g_sChar.sMaxPlayerDefense;
}

