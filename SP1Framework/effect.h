#ifndef _EFFECT_H
#define _EFFECT_H
#include "Framework\console.h"
#include "game.h"

class SRenderedEffect
{	
	public:
		double m_dExpiryTime;
		virtual void drawEffect() {};
};


class SEffectLine : public SRenderedEffect
{
	public:
		COORD m_cEnd1;
		COORD m_cEnd2;
		char m_cLineChar;
		unsigned char m_cColor;
		SEffectLine(COORD cEnd1, COORD cEnd2, char cLineChar, unsigned char cColor, double dLifespan)
		{
			m_cEnd1 = cEnd1;
			m_cEnd2 = cEnd2;
			m_cLineChar = cLineChar;
			m_cColor = cColor;
			m_dExpiryTime = g_dElapsedTime + dLifespan;
		}
		void drawEffect()
		{
			double dAX = m_cEnd1.X + 0.5, dAY = m_cEnd1.Y + 0.5, dBX = m_cEnd2.X + 0.5, dBY = m_cEnd2.Y + 0.5;
			double dDeltaX = dBX - dAX, dDeltaY = dBY - dAY;
			double dInterpolateX = dAX, dInterpolateY = dAY;
			int distance = floor(sqrt((dBX-dAX)*(dBX-dAX)+(dBY+dAY)*(dBY+dAY)));

			for(short s = 0; s < distance + 1; s++)
			{
				dInterpolateX += dDeltaX / (distance + 1);
				dInterpolateY += dDeltaY / (distance + 1);
				writeToBuffer(COORD{short(dInterpolateX), short(dInterpolateY)}, m_cLineChar, m_cColor); 
			}
		}
};

class SRenderedEffectList
{
	public: 
		SRenderedEffect* m_asEffects[400];

		SRenderedEffectList()
		{
			for(int i = 0; i < 399; i++)
			{
				m_asEffects[i] = nullptr;
			}
		}
		void clearExpiredEffects()
		{
			for(int i = 0; i < 399; i++)
			{
				if(m_asEffects[i] == nullptr) continue;
				if(m_asEffects[i]->m_dExpiryTime < g_dElapsedTime)
				{
 					delete m_asEffects[i];
					m_asEffects[i] = nullptr;
				}
			}
		};
		void addEffect(SRenderedEffect * sEffect)
		{
			for(int i = 0; i < 399; i++)
			{
				if(m_asEffects[i] == nullptr) 
				{
					m_asEffects[i] = sEffect;
					return;
				}
			}
		}
		void renderAllEffects()
		{
			for(int i = 0; i < 399; i++)
			{
				if(m_asEffects[i] == nullptr) continue;
				m_asEffects[i]->drawEffect();
			}
		}
};
#endif
