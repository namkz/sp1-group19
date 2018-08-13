#include "stats.h"

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

void playerLevelUp(int healthIncrease, int manaIncrease, int AttackIncrease, int defenseIncrease)
{
	SPlayerStats maxPlayerStats;
	maxPlayerStats.sPlayerHealth = maxPlayerStats.sPlayerHealth + healthIncrease;
	maxPlayerStats.sPlayerMana = maxPlayerStats.sPlayerMana + manaIncrease;
	maxPlayerStats.sPlayerAttack = maxPlayerStats.sPlayerAttack + AttackIncrease;
	maxPlayerStats.sPlayerDefense = maxPlayerStats.sPlayerDefense + defenseIncrease;
	playerStats.sPlayerHealth = maxPlayerStats.sPlayerHealth;
	playerStats.sPlayerMana = maxPlayerStats.sPlayerMana;
	playerStats.sPlayerAttack = maxPlayerStats.sPlayerAttack;
	playerStats.sPlayerDefense = maxPlayerStats.sPlayerDefense;

}

