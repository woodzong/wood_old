#ifndef CWOODTIMER_H_ASDASD8973912846123619236123
#define CWOODTIMER_H_ASDASD8973912846123619236123

#include <windows.h>

namespace wood{
	namespace comm{
		namespace timer{

			typedef struct{
				__int64       frequency;									// m_stWoodTimer Frequency
				float         resolution;									// m_stWoodTimer Resolution
				unsigned long mm_timer_start;								// Multimedia m_stWoodTimer Start Value
				unsigned long mm_timer_elapsed;							// Multimedia m_stWoodTimer Elapsed Time
				bool			performance_timer;							// Using The Performance m_stWoodTimer?
				__int64       performance_timer_start;					// Performance m_stWoodTimer Start Value
				__int64       performance_timer_elapsed;					// Performance m_stWoodTimer Elapsed Time
			}ST_WOOD_TIMER;


			class CWoodTimer{
			public:
				CWoodTimer();
				void Init();
				float GetTime();

			private:
				bool m_bIsInited;
				ST_WOOD_TIMER m_stWoodTimer;
			};

		}
	}
}



#endif