#ifndef _EFFECT_H
#define _EFFECT_H
#include "Framework\console.h"
#include "game.h"

class SRenderedEffect
{	
	public:
		double m_dExpiryTime;
		int m_iLayer = 0;
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
			m_iLayer = 0;
		}
		SEffectLine(COORD cEnd1, COORD cEnd2, char cLineChar, unsigned char cColor, double dLifespan, int iLayer)
		{
			m_cEnd1 = cEnd1;
			m_cEnd2 = cEnd2;
			m_cLineChar = cLineChar;
			m_cColor = cColor;
			m_dExpiryTime = g_dElapsedTime + dLifespan;
			m_iLayer = iLayer;
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

class SEffectParticle : public SRenderedEffect
{
	public:
		double m_dLocationX, m_dLocationY; // in pixels
		double m_dVelocityX, m_dVelocityY; // in pixels/s
		double m_dAccelerationX, m_dAccelerationY; // in pixels/s^2
		double m_dDamping; // in pixels/s
		double m_dUpdateTime;
		double m_dLastUpdateTime;
		char m_cChar;
		unsigned char m_cColor;
		SEffectParticle(COORD cLocation, char cChar, unsigned char cColor, double dLifespan, bool bStationary, int iLayer)
		{
			m_dLocationX = cLocation.X + 0.5;
			m_dLocationY = cLocation.Y + 0.5;
			m_cChar = cChar;
			m_cColor = cColor;
			m_dExpiryTime = g_dElapsedTime + dLifespan;
			m_dUpdateTime = g_dElapsedTime + 1.0/50;
			m_iLayer = iLayer;
			m_dAccelerationX = 0;
			m_dAccelerationY = 0;
			if(bStationary)
			{
				m_dVelocityX = 0;
				m_dVelocityY = 0;
				m_dDamping = 0;
			}
			else
			{
				m_dVelocityX = (rand() % 400 - 200) / 50.0;
				m_dVelocityY = (rand() % 400 - 200) / 50.0;
				m_dDamping = 2;
			}
		}
		void drawEffect()
		{
			if(m_dUpdateTime < g_dElapsedTime)
			{
				m_dLocationX += m_dVelocityX * (1.0/50);
				m_dLocationY += m_dVelocityY * (1.0/50);
				m_dVelocityX += m_dAccelerationX * (1.0/50) + m_dVelocityX * m_dDamping * (-1.0/50);
				m_dVelocityY += m_dAccelerationY * (1.0/50) + m_dVelocityY * m_dDamping * (-1.0/50);
				
				m_dUpdateTime = g_dElapsedTime + 1.0/50;
			}
			writeToBuffer(COORD{short(m_dLocationX), short(m_dLocationY)}, m_cChar, m_cColor); 
			m_dLastUpdateTime = g_dElapsedTime;
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
		void renderAllEffects(int iLayer)
		{
			for(int i = 0; i < 399; i++)
			{
				if(m_asEffects[i] == nullptr) continue;
				if(m_asEffects[i]->m_iLayer != iLayer) continue;
				m_asEffects[i]->drawEffect();
			}
		}
};
#endif
