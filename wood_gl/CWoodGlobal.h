#ifndef CWOOD_GLOBAL_H_ASDASD123123123
#define CWOOD_GLOBAL_H_ASDASD123123123
#include "CSingleton.h"
#include "Windows.h"

//<!---just for windows...
class CWoodTime{
public:
	CWoodTime(){
		LARGE_INTEGER lv;
		QueryPerformanceFrequency( &lv );
		m_fSecsPerTick = (float)lv.QuadPart;
		QueryPerformanceCounter(&m_lvPreTime);
	}

	float GetUsedTime(){
		float flTmp = GetUsedTimePart();
		ResetPreTimePart();
		return flTmp;
	}

	float GetUsedTimePart(){
		LARGE_INTEGER   currentTime;   
		QueryPerformanceCounter(&currentTime);   
		return (float)(currentTime.QuadPart   -   m_lvPreTime.QuadPart) / m_fSecsPerTick;		
	}	

	void ResetPreTimePart(){
		QueryPerformanceCounter(&m_lvPreTime);
	}

public:
	LARGE_INTEGER m_lvPreTime; 
	float m_fFPS;
	float m_fSecsPerTick;
};
//----------------!>

typedef CSingletonT<CWoodTime, CDummyLock> CWoodTimeSingleton;
#define CWOOD_TIME CWoodTimeSingleton::Instance()


class CWoodGlobal{
	public:
		CWoodGlobal(){
			m_fFlames = 0;
			m_fFPS    = 0;
		}

	public:
		float m_fFlames;	
		float m_fFPS;
};

typedef CSingletonT<CWoodGlobal, CDummyLock> CWoodGlobalSingleton;
#define CWOOD_GLOBAL CWoodGlobalSingleton::Instance()

#endif 