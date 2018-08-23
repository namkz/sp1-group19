#ifndef _MONSTER_H
#define _MONSTER_H

#include "entity.h"
#include "stats.h"
#include "Framework\console.h"

enum EAttackType {E_RANGED, E_MELEE};
enum EElement { E_NONE, E_FIRE, E_WATER, E_EARTH, E_AIR, E_LIGHTNING, E_ICE, E_COUNT};
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
//test monster database ( might use later on)
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
			m_iHealth = 23;
			m_iAttack = 12;
			m_iMana = 0;
			m_iDefense = 5;
			m_iScore = 4;
			m_dTurnInterval = 0.500;
			m_dAttackInterval = 1.500;
		}
		void takeTurn();
		void attack(SEntity *sTarget);
		void die();
};

//Level 1
class SEntityGreenSlime : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	unsigned char m_cJumpState;
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
		m_cJumpState = 0;
		m_dTurnInterval = 0.100;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
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
		m_dTurnInterval = 0.600;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
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
		m_dTurnInterval = 0.700;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
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
		m_dTurnInterval = 0.400;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
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
		m_dTurnInterval = 1.200;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
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
		m_dTurnInterval = 0.450;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
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
		m_dTurnInterval = 0.300;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
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
	void die();
};
//Level 2 Monsters database
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
	void die();
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
	void die();
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
	void die();
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
	void die();
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
	void die();
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
	void die();
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
	/*void takeTurn();
	void attack(SEntity *sTarget);*/
	void die();
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
	void die();
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
	void die();
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
	void die();
};
//Level 3 Monsters database
class SEntityRedSlime : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityRedSlime()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'S';
		m_cColor = 0x0C;
		m_sName = "Red Slime";
		m_sTheName = "the red slime";
		m_sAName = "a red slime";
		m_sCTheName = "The red slime";
		m_sCAName = "A red slime";
		m_iHealth = 180;
		m_iAttack = 36;
		m_iMana = 0;
		m_iDefense = 36;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntitySpearGoblin : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntitySpearGoblin()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'S';
		m_cColor = 0x0C;
		m_sName = "Spear Goblin";
		m_sTheName = "the spear goblin";
		m_sAName = "a spear goblin";
		m_sCTheName = "The spear goblin";
		m_sCAName = "A spear goblin";
		m_iHealth = 160;
		m_iAttack = 34;
		m_iMana = 0;
		m_iDefense = 16;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityDerangedWolf : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityDerangedWolf()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'S';
		m_cColor = 0x0C;
		m_sName = "Deranged Wolf";
		m_sTheName = "the deranged wolf";
		m_sAName = "a deranged wolf";
		m_sCTheName = "The deranged wolf";
		m_sCAName = "A deranged wolf";
		m_iHealth = 170;
		m_iAttack = 38;
		m_iMana = 0;
		m_iDefense = 22;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};	
class SEntitySkeletalWarrior : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntitySkeletalWarrior()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'S';
		m_cColor = 0x07;
		m_sName = "Skeletal Warrior";
		m_sTheName = "the skeletal warrior";
		m_sAName = "a skeletal warrior";
		m_sCTheName = "The skeletal warrior";
		m_sCAName = "A skeletal warrior";
		m_iHealth = 210;
		m_iAttack = 33;
		m_iMana = 0;
		m_iDefense = 27;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.000;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntitySkeletalArcher : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntitySkeletalArcher()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'S';
		m_cColor = 0x08;
		m_sName = "Skeletal Archer";
		m_sTheName = "the skeletal archer";
		m_sAName = "a skeletal archer";
		m_sCTheName = "The skeletal archer";
		m_sCAName = "A skeletal archer";
		m_iHealth = 170;
		m_iAttack = 26;
		m_iMana = 0;
		m_iDefense = 18;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.000;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityIrritatedGhost : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityIrritatedGhost()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'I';
		m_cColor = 0x07;
		m_sName = "Irritated Ghost";
		m_sTheName = "the irritated ghost";
		m_sAName = "a irritated ghost";
		m_sCTheName = "The irritated ghost";
		m_sCAName = "A irritated ghost";
		m_iHealth = 150;
		m_iAttack = 25;
		m_iMana = 0;
		m_iDefense = 25;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityChameleon : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityChameleon()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = '.';
		m_cColor = 0x07;
		m_sName = "Chameleon";
		m_sTheName = "the chameleon";
		m_sAName = "a chameleon";
		m_sCTheName = "The chameleon";
		m_sCAName = "A chameleon";
		m_iHealth = 300;
		m_iAttack = 25;
		m_iMana = 0;
		m_iDefense = 29;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityEvenLargerRat : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityEvenLargerRat()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'F';
		m_cColor = 0x07;
		m_sName = "Even Larger Rat";
		m_sTheName = "the even larger rat";
		m_sAName = "an even larger rat";
		m_sCTheName = "The even larger rat";
		m_sCAName = "A even larger rat";
		m_iHealth = 300;
		m_iAttack = 25;
		m_iMana = 0;
		m_iDefense = 20;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityMinorLightningElemental : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityMinorLightningElemental()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'F';
		m_cColor = 0x07;
		m_sName = "Minor Lighting Elemental";
		m_sTheName = "the minor lightning elemental";
		m_sAName = "a minor lightning elemental";
		m_sCTheName = "The minor lightning elemental";
		m_sCAName = "A minor lightning elemental";
		m_iHealth = 120;
		m_iAttack = 46;
		m_iMana = 0;
		m_iDefense = 30;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityLich : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityLich()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'L';
		m_cColor = 0x07;
		m_sName = "Lich";
		m_sTheName = "the lich";
		m_sAName = "a lich";
		m_sCTheName = "The lich";
		m_sCAName = "A lich";
		m_iHealth = 300;
		m_iAttack = 66;
		m_iMana = 0;
		m_iDefense = 54;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
//Level 4 Monsters database
class SEntityFrostKobold : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityFrostKobold()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'K';
		m_cColor = 0x0B;
		m_sName = "Frost Kobold";
		m_sTheName = "the frost kobold";
		m_sAName = "a frost kobold";
		m_sCTheName = "The frost kobold";
		m_sCAName = "A frost kobold	";
		m_iHealth = 300;
		m_iAttack = 48;
		m_iMana = 0;
		m_iDefense = 48;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityOrcWarrior : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityOrcWarrior()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'O';
		m_cColor = 0x02;
		m_sName = "Orc Warrior";
		m_sTheName = "the orc warrior";
		m_sAName = "a orc warrior";
		m_sCTheName = "The orc warrior";
		m_sCAName = "A orc warrior	";
		m_iHealth = 340;
		m_iAttack = 44;
		m_iMana = 0;
		m_iDefense = 52;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityGoblinWolfrider : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityGoblinWolfrider()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'R';
		m_cColor = 0x0A;
		m_sName = "Goblin Wolfrider";
		m_sTheName = "the goblin wolfrider";
		m_sAName = "a goblin wolfrider";
		m_sCTheName = "The goblin wolfrider";
		m_sCAName = "A goblin wolfrider";
		m_iHealth = 400;
		m_iAttack = 57;
		m_iMana = 0;
		m_iDefense = 51;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityGoblinDartShooter : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityGoblinDartShooter()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'G';
		m_cColor = 0x0A;
		m_sName = "Goblin Dartshooter";
		m_sTheName = "the goblin dartshooter";
		m_sAName = "a goblin dart shooter";
		m_sCTheName = "The goblin dartshooter";
		m_sCAName = "A goblin dart shooter";
		m_iHealth = 250;
		m_iAttack = 55;
		m_iMana = 0;
		m_iDefense = 30;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityBabyTroll : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityBabyTroll()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'T';
		m_cColor = 0x0B;
		m_sName = "Baby Troll";
		m_sTheName = "the baby troll";
		m_sAName = "a baby troll";
		m_sCTheName = "The baby troll";
		m_sCAName = "A baby troll";
		m_iHealth = 380;
		m_iAttack = 60;
		m_iMana = 0;
		m_iDefense = 45;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityWisp : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityWisp()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'W';
		m_cColor = 0x0A;
		m_sName = "Wisp";
		m_sTheName = "the wisp";
		m_sAName = "a wisp";
		m_sCTheName = "The wisp";
		m_sCAName = "A wisp";
		m_iHealth = 200;
		m_iAttack = 55;
		m_iMana = 0;
		m_iDefense = 40;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityDrunkGoblin : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityDrunkGoblin()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'D';
		m_cColor = 0x0A;
		m_sName = "Drunk Goblin";
		m_sTheName = "the drunk goblin";
		m_sAName = "a drunk goblin";
		m_sCTheName = "The drunk goblin";
		m_sCAName = "A drunk goblin";
		m_iHealth = 260;
		m_iAttack = 68;
		m_iMana = 0;
		m_iDefense = 40;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityPurpleSlime   : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityPurpleSlime()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'P';
		m_cColor = 0x05;
		m_sName = "Purple Slime";
		m_sTheName = "the purple slime";
		m_sAName = "a purple slime";
		m_sCTheName = "The purple slime";
		m_sCAName = "A purple slime";
		m_iHealth = 320;
		m_iAttack = 51;
		m_iMana = 0;
		m_iDefense = 51;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityWizard : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityWizard()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'W';
		m_cColor = 0x09;
		m_sName = "Wizard";
		m_sTheName = "the wizard";
		m_sAName = "a wizard";
		m_sCTheName = "The wizard";
		m_sCAName = "A wizard";
		m_iHealth = 280;
		m_iAttack = 59;
		m_iMana = 0;
		m_iDefense = 37;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityOrcShaman : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityOrcShaman()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'O';
		m_cColor = 0x02;
		m_sName = "Orc Shaman";
		m_sTheName = "the orc shaman";
		m_sAName = "a orc shaman";
		m_sCTheName = "The orc shaman";
		m_sCAName = "A orc shaman";
		m_iHealth = 350;
		m_iAttack = 46;
		m_iMana = 0;
		m_iDefense = 50;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityMimic : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityMimic()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		switch(rand() % 10)
		{
		case 0: 
			m_cMonsterClass = '!';
		case 1: 
			m_cMonsterClass = '$';
		case 2: 
			m_cMonsterClass = '%';
		case 3: 
			m_cMonsterClass = '*';
		case 4: 
			m_cMonsterClass = '(';
		case 5: 
			m_cMonsterClass = ')';
		case 6: 
			m_cMonsterClass = '[';
		case 7: 
			m_cMonsterClass = ']';
		case 8: 
			m_cMonsterClass = '+';
		case 9: 
			m_cMonsterClass = '"';
		}
		m_cColor = rand() % 15 + 1;
		m_sName = "Mimic";
		m_sTheName = "the mimic";
		m_sAName = "a mimic";
		m_sCTheName = "The mimic";
		m_sCAName = "A mimic";
		m_iHealth = 400;
		m_iAttack = 65;
		m_iMana = 0;
		m_bHidden = true;
		m_iDefense = 60;
		m_dTurnInterval = 0.100;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void takeDamage(SDamagePacket *sDamage);
	void attack(SEntity *sTarget);
	void die();
};
class SEntityOrcWarchief : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityOrcWarchief()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'O';
		m_cColor = 0x02;
		m_sName = "Orc Warchief";
		m_sTheName = "the orc warchief";
		m_sAName = "a orc warchief";
		m_sCTheName = "The orc warchief";
		m_sCAName = "A orc warchief";
		m_iHealth = 500;
		m_iAttack = 76;
		m_iMana = 0;
		m_iDefense = 80;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};

//Level 5
class SEntityYellowSlime : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityYellowSlime()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'Y';
		m_cColor = 0x0D;
		m_sName = "Yellow Slime";
		m_sTheName = "the yellow slime";
		m_sAName = "a yellow slime";
		m_sCTheName = "The yellow slime";
		m_sCAName = "A yellow slime";
		m_iHealth = 450;
		m_iAttack = 60;
		m_iMana = 0;
		m_iDefense = 60;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityDwarvenWarrior : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityDwarvenWarrior()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'D';
		m_cColor = 0x08;
		m_sName = "Dwarven Warrior";
		m_sTheName = "the dwarven warrior";
		m_sAName = "a dwarven warrior";
		m_sCTheName = "The dwarven warrior";
		m_sCAName = "A dwarven warrior";
		m_iHealth = 480;
		m_iAttack = 66;
		m_iMana = 0;
		m_iDefense = 58;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityDwarvenLongbowman : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityDwarvenLongbowman()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'D';
		m_cColor = 0x08;
		m_sName = "Dwarven Longbowman";
		m_sTheName = "the dwarven longbowman";
		m_sAName = "a dwarven longbowman";
		m_sCTheName = "The dwarven longbowman";
		m_sCAName = "A dwarven longbowman";
		m_iHealth = 390;
		m_iAttack = 58;
		m_iMana = 0;
		m_iDefense = 48;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityDwarvenShieldbearer : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityDwarvenShieldbearer()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'D';
		m_cColor = 0x08;
		m_sName = "Dwarven Shieldbearer";
		m_sTheName = "the dwarven Shieldbearer";
		m_sAName = "a dwarven Shieldbearer";
		m_sCTheName = "The dwarven Shieldbearer";
		m_sCAName = "A dwarven Shieldbeaer";
		m_iHealth = 550;
		m_iAttack = 54;
		m_iMana = 0;
		m_iDefense = 70;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityDwarvenBlacksmith : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityDwarvenBlacksmith()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'D';
		m_cColor = 0x0B;
		m_sName = "Dwarven Blacksmith";
		m_sTheName = "the dwarven blacksmith";
		m_sAName = "a dwarven blacksmith";
		m_sCTheName = "The dwarven blacksmith";
		m_sCAName = "A dwarven blacksmith";
		m_iHealth = 500;
		m_iAttack = 74;
		m_iMana = 0;
		m_iDefense = 55;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityDwarfChief : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityDwarfChief()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'D';
		m_cColor = 0x08;
		m_sName = "Dwarf Chief";
		m_sTheName = "the dwarf chief";
		m_sAName = "a dwarf chief";
		m_sCTheName = "The dwarf chief";
		m_sCAName = "A dwarf chief";
		m_iHealth = 750;
		m_iAttack = 82;
		m_iMana = 0;
		m_iDefense = 78;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityGiantWorm : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityGiantWorm()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'W';
		m_cColor = 0x08;
		m_sName = "Giant Worm";
		m_sTheName = "the giant worm";
		m_sAName = "a giant worm";
		m_sCTheName = "The giant worm";
		m_sCAName = "A giant worm";
		m_iHealth = 1200;
		m_iAttack = 80;
		m_iMana = 0;
		m_iDefense = 60;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityAnts : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityAnts()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'a';
		m_cColor = 0x18;
		m_sName = "Ant";
		m_sTheName = "the ant";
		m_sAName = "an ant";
		m_sCTheName = "The ant";
		m_sCAName = "An ant";
		m_iHealth = 400;
		m_iAttack = 85;
		m_iMana = 0;
		m_iDefense = 53;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityIronGolem : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityIronGolem()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = '\'';
		m_cColor = 0x18;
		m_sName = "Iron Golem";
		m_sTheName = "the iron golem";
		m_sAName = "an iron golem";
		m_sCTheName = "The iron golem";
		m_sCAName = "An iron golem";
		m_iHealth = 900;
		m_iAttack = 84;
		m_iMana = 0;
		m_iDefense = 90;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityEarthDragon : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityEarthDragon()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'E';
		m_cColor = 0x18;
		m_sName = "Earth Dragon";
		m_sTheName = "the earth dragon";
		m_sAName = "an earth dragon";
		m_sCTheName = "The earth dragon";
		m_sCAName = "An earth dragon";
		m_iHealth = 1500;
		m_iAttack = 120;
		m_iMana = 0;
		m_iDefense = 135;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};	

//Level 6
class SEntityMotherSlime : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityMotherSlime()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'S';
		m_cColor = 0x0F;
		m_sName = "Mother Slime";
		m_sTheName = "the mother slime";
		m_sAName = "a mother slime";
		m_sCTheName = "The mother slime";
		m_sCAName = "A mother slime";
		m_iHealth = 750;
		m_iAttack = 85;
		m_iMana = 0;
		m_iDefense = 85;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityEnragedTroll : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityEnragedTroll()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'T';
		m_cColor = 0x02;
		m_sName = "Enraged Troll";
		m_sTheName = "the enraged troll";
		m_sAName = "an enraged troll";
		m_sCTheName = "The enraged troll";
		m_sCAName = "A enraged slime";
		m_iHealth = 900;
		m_iAttack = 94;
		m_iMana = 0;
		m_iDefense = 88;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityGoblinAssassin : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityGoblinAssassin()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'G';
		m_cColor = 0x02;
		m_sName = "Goblin Assassin";
		m_sTheName = "the goblin assassin";
		m_sAName = "a goblin assassin";
		m_sCTheName = "The goblin assassin";
		m_sCAName = "A goblin assassin";
		m_iHealth = 210;
		m_iAttack = 65;
		m_iMana = 0;
		m_iDefense = 50;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityArmouredTroll : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityArmouredTroll()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'T';
		m_cColor = 0x08;
		m_sName = "Armoured Troll";
		m_sTheName = "the armoured troll";
		m_sAName = "an armoured troll";
		m_sCTheName = "The armoured troll";
		m_sCAName = "A armoured troll";
		m_iHealth = 1050;
		m_iAttack = 94;
		m_iMana = 0;
		m_iDefense = 100;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityHumongousRat : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityHumongousRat()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'R';
		m_cColor = 0x08;
		m_sName = "Humongous Rat";
		m_sTheName = "the humongous rat";
		m_sAName = "an humongous rat";
		m_sCTheName = "The humongous rat";
		m_sCAName = "A humongous rat";
		m_iHealth = 1200;
		m_iAttack = 90;
		m_iMana = 0;
		m_iDefense = 85;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityLocustSwarm : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityLocustSwarm()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'L';
		m_cColor = 0x0E;
		m_sName = "Locust Swarm";
		m_sTheName = "the locust swarm";
		m_sAName = "a locust swarm";
		m_sCTheName = "The locust swarm";
		m_sCAName = "A locust swarm";
		m_iHealth = 800;
		m_iAttack = 105;
		m_iMana = 0;
		m_iDefense = 75;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityMetalScorpion : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityMetalScorpion()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'M';
		m_cColor = 0x08;
		m_sName = "Metal Scorpion";
		m_sTheName = "the metal scorpion";
		m_sAName = "a metal scorpion";
		m_sCTheName = "The metal scorpion";
		m_sCAName = "A metal scorpion";
		m_iHealth = 1100;
		m_iAttack = 99;
		m_iMana = 0;
		m_iDefense = 88;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityTrollChieftain : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityTrollChieftain()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'T';
		m_cColor = 0x0B;
		m_sName = "Troll Chieftain";
		m_sTheName = "the troll Chieftain";
		m_sAName = "a troll chieftain";
		m_sCTheName = "The troll chieftain";
		m_sCAName = "A troll chieftain";
		m_iHealth = 1300;
		m_iAttack = 114;
		m_iMana = 0;
		m_iDefense = 108;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
//Level 7
class SEntityJungleSlime : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityJungleSlime()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'S';
		m_cColor = 0x02;
		m_sName = "Jungle Slime";
		m_sTheName = "the jungle slime";
		m_sAName = "a jungle slime";
		m_sCTheName = "The jungle slime";
		m_sCAName = "A jungle slime";
		m_iHealth = 1150;
		m_iAttack = 120;
		m_iMana = 0;
		m_iDefense = 120;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityElfFighter : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityElfFighter()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'E';
		m_cColor = 0x016;
		m_sName = "Elf Fighter";
		m_sTheName = "the elf fighter";
		m_sAName = "a elf fighter";
		m_sCTheName = "The elf fighter";
		m_sCAName = "A elf fighter";
		m_iHealth = 1200;
		m_iAttack = 128;
		m_iMana = 0;
		m_iDefense = 115;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityElfDuelist : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityElfDuelist()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = '/';
		m_cColor = 0x016;
		m_sName = "Elf Duelist";
		m_sTheName = "the elf duelist";
		m_sAName = "a elf duelist";
		m_sCTheName = "The elf duelist";
		m_sCAName = "A elf duelist";
		m_iHealth = 1050;
		m_iAttack = 140;
		m_iMana = 0;
		m_iDefense = 110;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityElfLongbowman : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityElfLongbowman()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = '/';
		m_cColor = 0x016;
		m_sName = "Elf Longbowman";
		m_sTheName = "the elf longbowman";
		m_sAName = "a elf longbowman";
		m_sCTheName = "The elf longbowman";
		m_sCAName = "A elf longbowman";
		m_iHealth = 900;
		m_iAttack = 145;
		m_iMana = 0;
		m_iDefense = 105;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityElfMage: public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityElfMage()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'M';
		m_cColor = 0x016;
		m_sName = "Elf Mage";
		m_sTheName = "the elf mage";
		m_sAName = "a elf mage";
		m_sCTheName = "The elf mage";
		m_sCAName = "A elf mage";
		m_iHealth = 950;
		m_iAttack = 145;
		m_iMana = 0;
		m_iDefense = 115;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityWolfFamiliar : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityWolfFamiliar()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'W';
		m_cColor = 0x0B;
		m_sName = "Wolf Familiar";
		m_sTheName = "the wolf familiar";
		m_sAName = "a wolf familiar";
		m_sCTheName = "The wolf familiar";
		m_sCAName = "A wolf familiar";
		m_iHealth = 1300;
		m_iAttack = 140;
		m_iMana = 0;
		m_iDefense = 135;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityHighElfWizard : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityHighElfWizard()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'W';
		m_cColor = 0x0E;
		m_sName = "High Elf Wizard";
		m_sTheName = "the high elf wizard";
		m_sAName = "a high elf wizard";
		m_sCTheName = "The high elf wizard";
		m_sCAName = "A high elf wizard";
		m_iHealth = 1100;
		m_iAttack = 160;
		m_iMana = 0;
		m_iDefense = 130;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityDarkElfAssassin : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityDarkElfAssassin()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'A';
		m_cColor = 0x04;
		m_sName = "Dark Elf Assassin";
		m_sTheName = "the dark elf assassin";
		m_sAName = "a dark elf assassin";
		m_sCTheName = "The dark elf assassin";
		m_sCAName = "A dark elf assassin";
		m_iHealth = 1250;
		m_iAttack = 180;
		m_iMana = 0;
		m_iDefense = 120;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityElfLeader : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityElfLeader()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'L';
		m_cColor = 0x06;
		m_sName = "Elf Leader";
		m_sTheName = "the elf leader";
		m_sAName = "a elf leader";
		m_sCTheName = "The elf leader";
		m_sCAName = "A elf leader";
		m_iHealth = 1500;
		m_iAttack = 170;
		m_iMana = 0;
		m_iDefense = 160;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityAmarok : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityAmarok()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'd';
		m_cColor = 0x08;
		m_sName = "Amarok";
		m_sTheName = "the amarok";
		m_sAName = "a amarok";
		m_sCTheName = "The amarok";
		m_sCAName = "A amarok";
		m_iHealth = 3000;
		m_iAttack = 250;
		m_iMana = 0;
		m_iDefense = 235;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
//Level 8 
class SEntityIlluminantSlime : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityIlluminantSlime()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'S';
		m_cColor = 0x07;
		m_sName = "Illuminant Slime";
		m_sTheName = "the illuminant slime";
		m_sAName = "a illuminant slime";
		m_sCTheName = "The illuminant slime";
		m_sCAName = "A illuminant slime";
		m_iHealth = 1750;
		m_iAttack = 170;
		m_iMana = 0;
		m_iDefense = 170;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityRobo_Rat_3000 : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityRobo_Rat_3000()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'S';
		m_cColor = 0x07;
		m_sName = "Robo-Rat-3000";
		m_sTheName = "the robo-rat-3000";
		m_sAName = "a robo-rat-3000";
		m_sCTheName = "The robo-rat-3000";
		m_sCAName = "A robo-rat-3000";
		m_iHealth = 2200;
		m_iAttack = 190;
		m_iMana = 0;
		m_iDefense = 185;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityCentaurSpearman : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityCentaurSpearman()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'C';
		m_cColor = 0x06;
		m_sName = "Centaur Spearman";
		m_sTheName = "the centaur spearman";
		m_sAName = "a centaur spearman";
		m_sCTheName = "The centaur spearman";
		m_sCAName = "A centaur spearman";
		m_iHealth = 1900;
		m_iAttack = 200;
		m_iMana = 0;
		m_iDefense = 180;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityCentaurBowman : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityCentaurBowman()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'C';
		m_cColor = 0x08;
		m_sName = "Centaur Bowman";
		m_sTheName = "the centaur bowman";
		m_sAName = "a centaur bowman";
		m_sCTheName = "The centaur bowman";
		m_sCAName = "A centaur bowman";
		m_iHealth = 1700;
		m_iAttack = 185;
		m_iMana = 0;
		m_iDefense = 165;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityCentaurChampion : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityCentaurChampion()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'C';
		m_cColor = 0x0B;
		m_sName = "Centaur Champion";
		m_sTheName = "the centaur champion";
		m_sAName = "a centaur champion";
		m_sCTheName = "The centaur champion";
		m_sCAName = "A centaur champion";
		m_iHealth = 2400;
		m_iAttack = 210;
		m_iMana = 0;
		m_iDefense = 200;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityBasilisk : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityBasilisk()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'S';
		m_cColor = 0x0B;
		m_sName = "Basilisk";
		m_sTheName = "the basilisk";
		m_sAName = "a basilisk";
		m_sCTheName = "The basilisk";
		m_sCAName = "A basilisk";
		m_iHealth = 2100;
		m_iAttack = 180;
		m_iMana = 0;
		m_iDefense = 170;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};

class SEntityMinotaur : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityMinotaur()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'B';
		m_cColor = 0x0B;
		m_sName = "Minotaur";
		m_sTheName = "the minotaur";
		m_sAName = "a minotaur";
		m_sCTheName = "The minotaur";
		m_sCAName = "A minotaur";
		m_iHealth = 2700;
		m_iAttack = 230;
		m_iMana = 0;
		m_iDefense = 160;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};

class SEntitySuspiciousLookingMountain : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntitySuspiciousLookingMountain()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'G';
		m_cColor = 0x0B;
		m_sName = "Suspicious Looking Mountain";
		m_sTheName = "the suspicious looking mountain";
		m_sAName = "a suspicious looking mountain";
		m_sCTheName = "The suspicious looking mountain";
		m_sCAName = "A suspicious looking mountain";
		m_iHealth = 1000;
		m_iAttack = 0;
		m_iMana = 0;
		m_iDefense = 0;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
/*	void takeTurn();
	void attack(SEntity *sTarget);*/
	void die();
};
class SEntityGiantTortoise : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityGiantTortoise()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'T';
		m_cColor = 0x0B;
		m_sName = "Giant Tortoise";
		m_sTheName = "the giant tortoise";
		m_sAName = "a giant tortoise";
		m_sCTheName = "The giant tortoise";
		m_sCAName = "A giant tortoise";
		m_iHealth = 5000;
		m_iAttack = 200;
		m_iMana = 0;
		m_iDefense = 400;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityWaterDragon : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityWaterDragon()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'D';
		m_cColor = 0x0B;
		m_sName = "Water Dragon";
		m_sTheName = "the water dragon";
		m_sAName = "a water dragon";
		m_sCTheName = "The water dragon";
		m_sCAName = "A water dragon";
		m_iHealth = 3500;
		m_iAttack = 330;
		m_iMana = 0;
		m_iDefense = 270;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
//Level 9
class SEntityKingSlime : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityKingSlime()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'D';
		m_cColor = 0x0B;
		m_sName = "King Slime";
		m_sTheName = "the king slime";
		m_sAName = "a king slime";
		m_sCTheName = "The king slime";
		m_sCAName = "A king slime";
		m_iHealth = 2750;
		m_iAttack = 250;
		m_iMana = 0;
		m_iDefense = 250;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityGreaterWaterElemental : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityGreaterWaterElemental()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'D';
		m_cColor = 0x0B;
		m_sName = "Greater Water Elemental";
		m_sTheName = "the greater water elemental";
		m_sAName = "a greater water elemental";
		m_sCTheName = "The greater water elemental";
		m_sCAName = "A greater water elemental";
		m_iHealth = 3200;
		m_iAttack = 275;
		m_iMana = 0;
		m_iDefense = 260;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityPowderKeg : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityPowderKeg()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'K';
		m_cColor = 0x016;
		m_sName = "Powder Keg";
		m_sTheName = "the powder keg";
		m_sAName = "a powder keg";
		m_sCTheName = "The powder keg";
		m_sCAName = "A powder keg";
		m_iHealth = 500;
		m_iAttack = 0;
		m_iMana = 0;
		m_iDefense = 0;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	/*void takeTurn();
	void attack(SEntity *sTarget);*/
	void die();
};
class SEntityForgottenBlade : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityForgottenBlade()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = '/';
		m_cColor = 0x0B;
		m_sName = "Forgotten Blade";
		m_sTheName = "the forogtten blade";
		m_sAName = "a forgotten blade";
		m_sCTheName = "The forgotten blade";
		m_sCAName = "A forgotten blade";
		m_iHealth = 2500;
		m_iAttack = 350;
		m_iMana = 0;
		m_iDefense = 300;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityFireDragon : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityFireDragon()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'D';
		m_cColor = 0x014;
		m_sName = "Fire Dragon";
		m_sTheName = "the fire dragon";
		m_sAName = "a fire dragon";
		m_sCTheName = "The fire dragon";
		m_sCAName = "A forgotten blade";
		m_iHealth = 3600;
		m_iAttack = 330;
		m_iMana = 0;
		m_iDefense = 320;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityWindDragon : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityWindDragon()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'D';
		m_cColor = 0x0E;
		m_sName = "Wind Dragon";
		m_sTheName = "the wind dragon";
		m_sAName = "a wind dragon";
		m_sCTheName = "The wind dragon";
		m_sCAName = "A wind dragon";
		m_iHealth = 3400;
		m_iAttack = 310;
		m_iMana = 0;
		m_iDefense = 340;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityCerberus : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityCerberus()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'C';
		m_cColor = 0x0C;
		m_sName = "Cerberus";
		m_sTheName = "the cerberus";
		m_sAName = "a cerberus";
		m_sCTheName = "The cerberus";
		m_sCAName = "A cerberus";
		m_iHealth = 3800;
		m_iAttack = 340;
		m_iMana = 0;
		m_iDefense = 380;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityCyclops : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityCyclops()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'C';
		m_cColor = 0x07;
		m_sName = "Cyclops";
		m_sTheName = "the cyclops";
		m_sAName = "a cyclops";
		m_sCTheName = "The cyclops";
		m_sCAName = "A cyclops";
		m_iHealth = 4500;
		m_iAttack = 360;
		m_iMana = 0;
		m_iDefense = 340;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityElderDragon : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityElderDragon()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'D';
		m_cColor = 0x0D;
		m_sName = "Elder Dragon";
		m_sTheName = "the elder dragon";
		m_sAName = "a elder dragon";
		m_sCTheName = "The elder dragon";
		m_sCAName = "A elder dragon";
		m_iHealth = 5500;
		m_iAttack = 450;
		m_iMana = 0;
		m_iDefense = 500;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};
class SEntityJormungand : public SEntity
{
public:
	COORD m_cLastSeenTarget;
	SEntityJormungand()
	{
		m_cLastSeenTarget.X = -1;
		m_cLastSeenTarget.Y = 0;
		m_cMonsterClass = 'J';
		m_cColor = 0x0D;
		m_sName = "Jourmungand";
		m_sTheName = "the jourmungand";
		m_sAName = "a jourmungand";
		m_sCTheName = "The jourmungand";
		m_sCAName = "A jormungand";
		m_iHealth = 15000;
		m_iAttack = 400;
		m_iMana = 0;
		m_iDefense = 450;
		m_dTurnInterval = 0.500;
		m_dAttackInterval = 1.500;
	}
	void takeTurn();
	void attack(SEntity *sTarget);
	void die();
};


#endif