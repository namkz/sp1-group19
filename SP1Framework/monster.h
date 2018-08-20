#ifndef _MONSTER_H
#define _MONSTER_H

#include "entity.h"
#include "stats.h"
#include "Framework\console.h"

enum EAttackType {E_RANGED, E_MELEE};
enum EElement {E_FIRE, E_WATER, E_EARTH, E_AIR, E_PHYSICAL, E_MEME};
int getEightDirectionOf(COORD cInput, COORD cTarget);
COORD nStepsIn(COORD cInput, int iN, int iDirection);


struct SAttack
{
	enum EAttackType m_eAttack;
	enum EElement m_eElement;
	unsigned short m_sMaxRange;
	unsigned int m_iBaseDamage;
	unsigned int m_iDamageVariance;
};

class SEntityFlamerTroll : public SEntity
{
	public:
		COORD m_cLastSeenTarget;
		SEntityFlamerTroll()
		{
			m_cLastSeenTarget.X = -1;
			m_cLastSeenTarget.Y = 0;
			m_cMonsterClass = 'T';
			m_cColor = 0x0c;
			m_sName = "flamer troll";
			m_sTheName = "the flamer troll";
			m_sAName = "a flamer troll";
			m_sCTheName = "The flamer troll";
			m_sCAName = "A flamer troll";
			m_iHealth = 20;
			m_iAttack = 12;
			m_iMana = 0;
			m_iDefense = 5;
			m_dTurnInterval = 0.500;
			m_dAttackInterval = 1.500;
		}
		void takeTurn();
		void attack(SEntity *sTarget);
		void die();
};
class SEntityGreenSlime : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityGreenSlime()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'P';
		m_cColor = 0x0A;
		m_sName = "green slime";
		m_sTheName = "the green slime";
		m_sAName = "a green slime";
		m_sCTheName = "The green slime";
		m_sCAName = "A green slime";
		m_iHealth = 50;
		m_iAttack = 8;
		m_iMana = 0;
		m_iDefense = 12;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
};
class SEntityGoblin : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityGoblin()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'o';
		m_cColor = 0x0A;
		m_sName = "goblin";
		m_sTheName = "the goblin";
		m_sAName = "a goblin";
		m_sCTheName = "The goblin";
		m_sCAName = "A goblin";
		m_iHealth = 60;
		m_iAttack = 13;
		m_iMana = 0;
		m_iDefense = 6;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
};
class SEntityPossessedStick : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityPossessedStick()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = '\'';
		m_cColor = 0x0B;
		m_sName = "possessed stick";
		m_sTheName = "the possessed stick";
		m_sAName = "a possessed stick";
		m_sCTheName = "The possessed stick";
		m_sCAName = "A possessed stick";
		m_iHealth = 40;
		m_iAttack = 16;
		m_iMana = 0;
		m_iDefense = 0;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
};
class SEntityTinyRat : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityTinyRat()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'r';
		m_cColor = 0x03;
		m_sName = "tiny rat";
		m_sTheName = "the tiny rat";
		m_sAName = "a tiny rat";
		m_sCTheName = "The tiny rat";
		m_sCAName = "A tiny rat";
		m_iHealth = 30;
		m_iAttack = 6;
		m_iMana = 0;
		m_iDefense = 10;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
};
class SEntityRedSnail : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityRedSnail()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'z';
		m_cColor = 0x0B;
		m_sName = "red snail";
		m_sTheName = "the red snail";
		m_sAName = "a red snail";
		m_sCTheName = "The red snail";
		m_sCAName = "A red snail";
		m_iHealth = 80;
		m_iAttack = 4;
		m_iMana = 0;
		m_iDefense = 16;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
};
class SEntityGlowingMushroom : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityGlowingMushroom()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'F';
		m_cColor = 0x0B	;
		m_sName = "glowing mushroom";
		m_sTheName = "the glowing mushroom";
		m_sAName = "a glowing mushroom";
		m_sCTheName = "The glowing mushroom";
		m_sCAName = "A glowing mushroom";
		m_iHealth = 50;
		m_iAttack = 8;
		m_iMana = 0;
		m_iDefense = 8;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
};
class SEntityBoar : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityBoar()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'q';
		m_cColor = 0x03;
		m_sName = "boar";
		m_sTheName = "the boar";
		m_sAName = "a boar";
		m_sCTheName = "The boar";
		m_sCAName = "A boar";
		m_iHealth = 70;
		m_iAttack = 14;
		m_iMana = 0;
		m_iDefense = 14;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
};
class SEntityMosquito : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityMosquito()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'a';
		m_cColor = 0x01;
		m_sName = "mosquito";
		m_sTheName = "the mosquito";
		m_sAName = "a mosquito";
		m_sCTheName = "The mosquito";
		m_sCAName = "A mosquito";
		m_iHealth = 60;
		m_iAttack = 20;
		m_iMana = 0;
		m_iDefense = 6;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
};
class SEntityBlueSlime : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityBlueSlime()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'P';
		m_cColor = 0x0B;
		m_sName = "blue slime";
		m_sTheName = "the blue slime";
		m_sAName = "a blue slime";
		m_sCTheName = "The blue slime";
		m_sCAName = "A blue slime";
		m_iHealth = 80;
		m_iAttack = 16;
		m_iMana = 0;
		m_iDefense = 12;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
};
class SEntityGooglyEyes : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityGooglyEyes()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'e';
		m_cColor = 0x0D;
		m_sName = "googly eyes";
		m_sTheName = "the googly eyes";
		m_sAName = "a googly eye";
		m_sCTheName = "The googly eye";
		m_sCAName = "A googly eye";
		m_iHealth = 90;
		m_iAttack = 20;
		m_iMana = 0;
		m_iDefense = 10;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.000;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
};
class SEntityBouncyBall : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityBouncyBall()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'e';
		m_cColor = 0x0B;
		m_sName = "bouncy ball";
		m_sTheName = "the bouncy ball";
		m_sAName = "a bouncy ball";
		m_sCTheName = "The bouncy ball";
		m_sCAName = "A bouncy ball";
		m_iHealth = 70;
		m_iAttack = 13;
		m_iMana = 0;
		m_iDefense = 14;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.000;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
};
class SEntityMadRabbit : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityMadRabbit()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'R';
		m_cColor = 0x0E;
		m_sName = "mad rabbit";
		m_sTheName = "the mad rabbit";
		m_sAName = "a mad rabbit";
		m_sCTheName = "The mad rabbit";
		m_sCAName = "A mad rabbit";
		m_iHealth = 110;
		m_iAttack = 15;
		m_iMana = 0;
		m_iDefense = 10;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.000;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
};
class SEntityLostSoul : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityLostSoul()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = '@';
		m_cColor = 0x07;
		m_sName = "lost soul";
		m_sTheName = "the lost soul";
		m_sAName = "a lost soul";
		m_sCTheName = "The lost soul";
		m_sCAName = "A lost soul";
		m_iHealth = 120;
		m_iAttack = 18;
		m_iMana = 0;
		m_iDefense = 13;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
};
class SEntityFireSalamander : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityFireSalamander()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = ':';
		m_cColor = 0x0C;
		m_sName = "fire salamander";
		m_sTheName = "the fire salamander";
		m_sAName = "a fire salamander";
		m_sCTheName = "The fire salamander";
		m_sCAName = "A fire salamander";
		m_iHealth = 115;
		m_iAttack = 15;
		m_iMana = 0;
		m_iDefense = 12;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
};
class SEntityWarningSign : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityWarningSign()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = '!';
		m_cColor = 0x02;
		m_sName = "ominous warning sign";
		m_sTheName = "the ominous warning sign";
		m_sAName = "an ominous warning sign";
		m_sCTheName = "The ominous warning sign";
		m_sCAName = "An ominous warning sign";
		m_iHealth = 60;
		m_iAttack = 2;
		m_iMana = 0;
		m_iDefense = 0;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
};
class SEntityLargeRat : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityLargeRat()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'r';
		m_cColor = 0x08;
		m_sName = "large rat";
		m_sTheName = "the large rat";
		m_sAName = "a large rat";
		m_sCTheName = "The large rat";
		m_sCAName = "A large rat";
		m_iHealth = 85;
		m_iAttack = 22;
		m_iMana = 0;
		m_iDefense = 23;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
};
class SEntityPuppy : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityPuppy()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'd';
		m_cColor = 0x05;
		m_sName = "innocent-looking puppy";
		m_sTheName = "the innocent-looking puppy";
		m_sAName = "a innocent-looking puppy";
		m_sCTheName = "The innocent-looking puppy";
		m_sCAName = "A innocent-looking puppy";
		m_iHealth = 100;
		m_iAttack = 0;
		m_iMana = 0;
		m_iDefense = 0;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
};
class SEntityBeast : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityBeast()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'd';
		m_cColor = 0x0A;
		m_sName = "savage beast";
		m_sTheName = "the savage beast";
		m_sAName = "a savage beast";
		m_sCTheName = "The savage beast";
		m_sCAName = "A savage beast";
		m_iHealth = 200;
		m_iAttack = 35;
		m_iMana = 0;
		m_iDefense = 30;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
};











#endif