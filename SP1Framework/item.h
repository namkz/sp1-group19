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
	class SItem *m_asContents[20];

	// [!] GET DONE BY 14TH 6PM - Function for adding an item to the inventory
	// I need the signature bool addItem(*SItem k);
	// You need to pass in SItem by reference (that is, pass in its address as a pointer)
	// as items can be quite big data chunks
	// See notes/optimization.txt
	// [!] GET DONE BY 14TH 6PM - Function for removing an item from the inventory - please add
	// Recommendation: Empty slot = null pointer (nullptr)

};

class SIntellectualWizardHat ;


#endif