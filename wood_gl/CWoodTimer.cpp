#include "CWoodTimer.h"

using namespace wood::comm::timer;

CWoodTimer::CWoodTimer():m_bIsInited(false){
}

void CWoodTimer::Init(){
	memset(&m_stWoodTimer, 0, sizeof(m_stWoodTimer));

	//声明:
	//QueryPerformanceFrequency(
	//  var lpFrequency: TLargeInteger {此参数获取定时器每秒的频率数; TLargeInteger = Int64}
	//): BOOL;                         {返回 False 表示硬件不支持高性能定时器}


	// Check To See If A Performance Counter Is Available
	// If One Is Available The m_stWoodTimer Frequency Will Be Updated
	if ( !QueryPerformanceFrequency((LARGE_INTEGER *) &m_stWoodTimer.frequency) )
	{
		// No Performace Counter Available
		m_stWoodTimer.performance_timer	= FALSE;					// Set Performance m_stWoodTimer To FALSE
		m_stWoodTimer.mm_timer_start	= timeGetTime();			// Use timeGetTime() To Get Current Time
		m_stWoodTimer.resolution		= 1.0f/1000.0f;				// Set Our m_stWoodTimer Resolution To .001f
		m_stWoodTimer.frequency			= 1000;						// Set Our m_stWoodTimer Frequency To 1000
		m_stWoodTimer.mm_timer_elapsed	= m_stWoodTimer.mm_timer_start;		// Set The Elapsed Time To The Current Time
	}
	else
	{
		//QueryPerformanceCounter 返回定时器当前计数值.
		// Performance Counter Is Available, Use It Instead Of The Multimedia m_stWoodTimer
		// Get The Current Time And Store It In performance_timer_start
		QueryPerformanceCounter((LARGE_INTEGER *) &m_stWoodTimer.performance_timer_start);
		m_stWoodTimer.performance_timer			= TRUE;				// Set Performance m_stWoodTimer To TRUE
		// Calculate The m_stWoodTimer Resolution Using The m_stWoodTimer Frequency
		m_stWoodTimer.resolution				= (float) (((double)1.0f)/((double)m_stWoodTimer.frequency));
		// Set The Elapsed Time To The Current Time
		m_stWoodTimer.performance_timer_elapsed	= m_stWoodTimer.performance_timer_start;
	}

	m_bIsInited = true;
}



float CWoodTimer::GetTime()										// Get Time In Milliseconds
{
	if(m_bIsInited == false){
		return 0.0f;
	}

	__int64 time;											// time Will Hold A 64 Bit Integer

	if (m_stWoodTimer.performance_timer)							// Are We Using The Performance m_stWoodTimer?
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &time);	// Grab The Current Performance Time
		// Return The Current Time Minus The Start Time Multiplied By The Resolution And 1000 (To Get MS)
		return ( (float) ( time - m_stWoodTimer.performance_timer_start) * m_stWoodTimer.resolution)*1000.0f;
	}
	else
	{
		// Return The Current Time Minus The Start Time Multiplied By The Resolution And 1000 (To Get MS)
		return( (float) ( timeGetTime() - m_stWoodTimer.mm_timer_start) * m_stWoodTimer.resolution)*1000.0f;
	}
}
