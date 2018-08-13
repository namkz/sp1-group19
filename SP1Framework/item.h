#ifndef _ITEM_H
#define _ITEM_H

#include "monster.h"
#include "stats.h"
#include <string>

class SItem
{
	public:
		char m_cDroppedIcon;
		char m_cDroppedColour;
		std::string m_sName;

		virtual void onHolderHit(SDamagePacket *sDamage) {return; };
};

class SInventory
{
public:
	struct SItem m_asContents[20];
};

class SIntellectualWizardHat ;


#endif