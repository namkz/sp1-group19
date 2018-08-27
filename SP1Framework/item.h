#ifndef _ITEM_H
#define _ITEM_H

#include "monster.h"
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
	class SItem *m_asEquipment[ES_COUNT];

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

class SItemIntellectualWizardHat : public SItem
{
public:
	SItemIntellectualWizardHat()
	{
		m_cDroppedIcon = '^';
		m_cDroppedColour = 0x0C;
		m_sName = "Intellectual Wizard's Hat";
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

class SItemRobesofGuidance : public SItem
{
public:
	SItemRobesofGuidance()
	{
		m_cDroppedIcon = '&';
		m_cDroppedColour = 0x0C;
		m_sName = "Robes of Guidance";
		m_sHealth = "10+8%";
		m_sMana = "25+4%";
		m_sAttack = "0";
		m_sDefense = "10+3%";
		m_sSpecial1 = "Water Spells do 10% more damage.";
		m_sSpecial2 = "Reduces water damage taken by 10%"; 
		m_sDescription1 = "Guidance along the way";
		m_sDescription2 = "Darkness shall cometh over";
	}

	void onHolderHit(SDamagePacket *sDamage)
	{
		if (sDamage->m_eElement == E_WATER) sDamage->m_iDamage *= 0.9;
	};
	int processHealth(int iInput)
	{
		iInput = 10 + iInput * 0.08;
		return iInput;
	}
	int processMana(int iInput)
	{
		iInput = 25 + iInput * 0.04;
		return iInput;
	}
	int processAttack(int iInput)
	{
		iInput = 0;
		return iInput;
	}
	int processDefense(int iInput)
	{
		iInput = 10 + iInput * 0.03;
		return iInput;
	}
};

class SItemVampiricAmulet : public SItem
{
public:
	SItemVampiricAmulet()
	{
		m_cDroppedIcon = '%';
		m_cDroppedColour = 0x0C;
		m_sName = "Vampiric Amulet";
		m_sHealth = "0";
		m_sMana = "30";
		m_sAttack = "15";
		m_sDefense = "0";
		m_sSpecial1 = "Deals 5% more fire damage.";
		m_sSpecial2 = "5% fire damage dealt"; // rand()% 100 < 30
		m_sSpecial3 = "is converted to health";
		m_sDescription1 = "Amulet of the drunken Vampire";
		m_sDescription2 = "Everything has a price";
		m_sDescription3 = "Fire Engulfs the user";
	}

	void onHolderHit(SDamagePacket *sDamage)
	{
		if (sDamage->m_eElement == E_FIRE) sDamage->m_iDamage *= 2;
	};
	int processHealth(int iInput)
	{
		iInput = 0;
		return iInput;
	}
	int processMana(int iInput)
	{
		iInput = 30;
		return iInput;
	}
	int processAttack(int iInput)
	{
		iInput = 15;
		return iInput;
	}
	int processDefense(int iInput)
	{
		iInput = 0;
		return iInput;
	}
};

class SItemClawsofSolace : public SItem
{
public:
	SItemClawsofSolace()
	{
		m_cDroppedIcon = '#';
		m_cDroppedColour = 0x0C;
		m_sName = "Claws of Solace";
		m_sHealth = "0";
		m_sMana = "25+3%";
		m_sAttack = "4+5%";
		m_sDefense = "2+10%";
		m_sDescription1 = "Sharp as a blade";
		
	}

	void onHolderHit(SDamagePacket *sDamage)
	{
		if (sDamage->m_eElement == E_FIRE) sDamage->m_iDamage *= 1.1;
	};
	int processHealth(int iInput)
	{
		iInput = 0;
		return iInput;
	}
	int processMana(int iInput)
	{
		iInput = 25 + iInput * 0.03;
		return iInput;
	}
	int processAttack(int iInput)
	{
		iInput = 4 + iInput * 0.05;
		return iInput;
	}
	int processDefense(int iInput)
	{
		iInput = 2 + iInput * 0.10;
		return iInput;
	}
};

class SItemStaffofMayonnaise : public SItem
{
public:
	SItemStaffofMayonnaise()
	{
		m_cDroppedIcon = '!';
		m_cDroppedColour = 0x0C;
		m_sName = "Staff of Mayonnaise";
		m_sHealth = "10+2%";
		m_sMana = "10+2%";
		m_sAttack = "5+5%";
		m_sDefense = "0";
		m_sDescription1 = "Mayonnaise have special powers!";
		
	}

	void onHolderHit(SDamagePacket *sDamage)
	{
		if (sDamage->m_eElement == E_LIGHTNING) sDamage->m_iDamage *= 0.8;
	};
	int processHealth(int iInput)
	{
		iInput = 10 + iInput * 0.02;
		return iInput;
	}
	int processMana(int iInput)
	{
		iInput = 10 + iInput * 0.02;
		return iInput;
	}
	int processAttack(int iInput)
	{
		iInput = 5 + iInput * 0.05;
		return iInput;
	}
	int processDefense(int iInput)
	{
		iInput = 0;
		return iInput;
	}
};

class SItemShoesofIllusion : public SItem
{
public:
	SItemShoesofIllusion()
	{
		m_cDroppedIcon = '=';
		m_cDroppedColour = 0x0C;
		m_sName = "Shoes of Illusion";
		m_sHealth = "35";
		m_sMana = "5";
		m_sAttack = "4";
		m_sDefense = "1";
		m_sSpecial1 = "Increases Health regen";
		m_sSpecial2 = "by 20%";
		m_sDescription1 = "Illusionary";


	}

	void onHolderHit(SDamagePacket *sDamage)
	{
		if (sDamage->m_eElement == E_AIR) sDamage->m_iDamage *= 1.5;
	};
	int processHealth(int iInput)
	{
		iInput = 35;
		return iInput;
	}
	int processMana(int iInput)
	{
		iInput = 5;
		return iInput;
	}
	int processAttack(int iInput)
	{
		iInput = 4;
		return iInput;
	}
	int processDefense(int iInput)
	{
		iInput = 1;
		return iInput;
	}
};

#endif