#include "inventory.h"
#include "game.h"


int minventory()
{
	SItem sitems[10];
	//Name , Type , health , mana , attack , defense all in order respectively.
	sitems[0] = { "Staff of Merlin" , 3 , 10 , 200 , 5, 2 }; 
	sitems[1] = { "Robes of Wisdom" , 2 ,  10 , 100 , 3 , 3 };
	sitems[2] = { "Shoes of Illusion" , 5 , 5 , 100 , 5 , 10 };
	sitems[3] = { "Wizard's Hat" , 0 , 3 , 10 , 10 , 50 };
	sitems[4] = { "Necklace of Good Fortune",1 , 3 , 3 , 3 ,3 };
	sitems[5] = { "Staff of Everlasting stars", 3 , 20 , 0 , 5 , 20 };
	sitems[6] = { "Robes of Granduer" , 2 , 0 , 10 , 20 , 10 };
	sitems[7] = { "Shoes of Granduer" , 5 , 0 , 5 , 10 , 20 };
	sitems[8] = { "Lord Red Hat" , 0 , 0 , 15 ,250 , 100 };
	sitems[9] = { "Necklace of descendancy" , 10, 15 , 0 , 150 };
	
	if (K_E)
	{
		clearScreen();
		printf("You look into your magical backpack \n i to view inventory \n E to view Equipment");
		if()
	}
}