#include "item.h"

class SItemIntellectualWizardHat : SItem
{
	char m_cDroppedIcon = '^';
	char m_cDroppedColour = 0x0C;
	std::string m_sName = "Intellectual Wizard's Hat";

	void onHolderHit(SDamagePacket *sDamage)
	{
		if(sDamage -> eElement == E_FIRE) sDamage->iDamage *= 1.1;
	};
};

// [!] ONGOING: Get as many items done as possible. Add more items.
// [!] EVENTUALLY: 