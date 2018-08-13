#include "item.h"

class SItemIntellectualWizardHat : SItem
{
	char m_cDroppedIcon = '^';
	char m_cDroppedColour = 0x0C;

	void onHolderHit(SDamagePacket *sDamage)
	{
		if(sDamage -> eElement == E_FIRE) sDamage->iDamage *= 1.1;
	};
};