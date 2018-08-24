#ifndef _BUFF_H
#define _BUFF_H
#include "game.h"

	
class SBuff
{
public :
	double dExpireTime;
	int m_iDefense;
};

class sBuffList
{
	SBuff *m_asBuff[10];
public: 
	bool addBuffs(SBuff* sAddBuffs)
	{
		for (int i = 0; i < 9;i++)
		{
			if (m_asBuff == nullptr)
			{
				m_asBuff[i] = sAddBuffs;
				return true;
			}
		}
		return false;
	}
	SBuff** Begin()
	{
		return &m_asBuff[0];
	}
	SBuff** end()
	{
		return &m_asBuff[9];
	}
	void cleanExpiredDebuffs()
	{
		for (int i = 0; i < 10; i++)
		{
			if (m_asBuff[i] != nullptr && m_asBuff[i] ->dExpireTime < g_dElapsedTime)
			{
				delete m_asBuff[i];
				m_asBuff[i] = nullptr;
			}
		}
	}
};




#endif // !

