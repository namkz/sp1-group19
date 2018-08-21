#include "item.h"
// All strings for special effects and description should be equal to or under 30 characters long. Any longer effects or descriptions should be split up into two lines
// There are 4 lines for Special effects and 3 lines for Description
//
class SItemIntellectualWizardHat : SItem
{
	char m_cDroppedIcon = '(';
	char m_cDroppedColour = 0x0C;
	std::string m_sName = "Intellectual Wizard's Hat";
	std::string m_sHealth = "5+5%";
	std::string m_sMana = "10+8%";
	std::string m_sAttack = "3+4%";
	std::string m_sDefense = "5+3%";
	std::string m_sSpecial1 = ">10% defense piercing";
	std::string m_sSpecial2 = ">30% chance of dealing"; // rand()% 100 < 30
	std::string m_sSpecial3 = "25% more damage";
	std::string m_sDescription1 = "The smart wizard chooses not to shoot the tougher bits";

	void onHolderHit(SDamagePacket *sDamage)
	{
		if(sDamage -> m_eElement == E_FIRE) sDamage->m_iDamage *= 1.1;
	};
	int processHealth(int iInput)
	{
		iInput = 5 + iInput*0.05;
		return iInput;
	}
	int processMana(int iInput)
	{
		iInput = 10 + iInput*0.08;
		return iInput;
	}
	int processAttack(int iInput)
	{
		iInput = 3 + iInput*0.04;
		return iInput;
	}
	int processDefense(int iInput)
	{
		iInput = 5 + iInput*0.03;
		return iInput;
	}
};


// [!] ONGOING: Get as many items done as possible. Add more items.
// [!] EVENTUALLY: 