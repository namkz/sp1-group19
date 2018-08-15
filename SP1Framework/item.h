#ifndef _ITEM_H
#define _ITEM_H

#include "monster.h"
#include "game.h"
#include "stats.h"
#include <string>

enum EEquipSlots
{
	ES_HAT,
	ES_NECK,
	ES_ROBE,
	ES_GLOVES,
	ES_WEAPON,
	ES_SHOES,
	ES_COUNT
};

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
	class SItem *m_asEquipment[ES_COUNT];


	bool addItem(SItem *psItem)
	{
		for (int i = 0; i < 20; i++)
		{
			if (m_asContents[i] == nullptr)
			{
				m_asContents[i] = psItem;
				break;
			}
		}
	};
	
	bool equipItemToSlot(short sIndex, EEquipSlots sEquipSlot)
	{
		m_asEquipment[sEquipSlot] = m_asContents[sIndex];
		m_asContents[sIndex] = nullptr;
	};

	SItem * removeItemAt(short sIndex)
	{
		SItem *sHold = m_asContents[sIndex];
		m_asContents[sIndex] = nullptr;
		return (sHold);
	};
	// [!] GET DONE BY 14TH 6PM - Function for adding an item to the inventory
	// I need the signature bool addItem(SItem *k);
	// You need to pass in SItem by reference (that is, pass in its address as a pointer)
	// as items can be quite big data chunks
	// See notes/optimization.txt
	// [!] GET DONE BY 14TH 6PM - Function for removing an item from the inventory - please add
	// Recommendation: Empty slot = null pointer (nullptr)
};

class SIntellectualWizardHat ;


#endif