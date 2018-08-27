#ifndef _ITEM_H
#define _ITEM_H

#include "monster.h"
#include <string>

enum EEquipSlots
{
	ES_AMULET,
	ES_WEAPON,
	ES_SHOES,
	ES_GLOVES,
	ES_ROBES,
	ES_HAT
};

class SItem
{
	public:
		char m_cDroppedIcon;
		char m_cDroppedColour;
		EEquipSlots m_eSlot;
		std::string m_sName;
		std::string m_sEquippedName1;
		std::string m_sEquippedName2;
		std::string m_sHealth;
		std::string m_sMana;
		std::string m_sAttack;
		std::string m_sDefense;
		std::string m_sSpecial1;
		std::string m_sSpecial2;
		std::string m_sSpecial3;
		std::string m_sSpecial4;
		std::string m_sDescription1;
		std::string m_sDescription2;
		std::string m_sDescription3;

		virtual void onHolderHit(SDamagePacket *sDamage) {return; };
};

struct SInventorySlotInfo
{
	unsigned short X, Y;
};

class SInventory
{
public:
	class SItem *m_asContents[38];

	SInventory()
	{
		for (int i = 0; i < 38; i++)
		{
			m_asContents[i] = nullptr;
		}
	}

	bool addItem(SItem *psItem)
	{
		for (int i = 6; i < 38; i++)
		{
			if (m_asContents[i] == nullptr)
			{
				m_asContents[i] = psItem;
				return true;
			}
		}
		return false;
	};
	
	bool equipItemToSlot(short sIndex)
	{
		if (m_asContents[sIndex] == nullptr) return false;
		EEquipSlots sEquipSlot = m_asContents[sIndex]->m_eSlot;
		if (m_asContents[sEquipSlot] != nullptr)
		{
			SItem temp;
			temp = *m_asContents[sEquipSlot];
			m_asContents[sEquipSlot] = m_asContents[sIndex];
			*m_asContents[sIndex] = temp;
		}
		else
		{
			m_asContents[sEquipSlot] = m_asContents[sIndex];
			m_asContents[sIndex] = nullptr;
		}
		return true;
	};

	void unequipItemFromSlot(short sIndex)
	{
		addItem(m_asContents[sIndex]);
		m_asContents[sIndex] = nullptr;
	}

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

class SItemIntellectualWizardHat : public SItem
{
public:
	SItemIntellectualWizardHat()
	{
		m_cDroppedIcon = '^';
		m_cDroppedColour = 0x0C;
		m_eSlot = ES_HAT;
		m_sName = "Intellectual Wizard's Hat";
		m_sEquippedName1 = "Int.";
		m_sEquippedName2 = "Wiz. Hat";
		m_sHealth = "5+5%";
		m_sMana = "10+8%";
		m_sAttack = "3+4%";
		m_sDefense = "5+3%";
		m_sSpecial1 = ">10% defense piercing";
		m_sSpecial2 = ">30% chance of dealing"; // rand()% 100 < 30
		m_sSpecial3 = "25% more damage";
		m_sDescription1 = "The smart wizard chooses not";
		m_sDescription2 = "to shoot at the tougher bits";
	}

	void onHolderHit(SDamagePacket *sDamage)
	{
		if (sDamage->m_eElement == E_FIRE) sDamage->m_iDamage *= 1.1;
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


#endif