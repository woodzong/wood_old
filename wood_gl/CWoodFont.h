#ifndef CWOODFONT_H_ASDASDsdfggasdf
#define CWOODFONT_H_ASDASDsdfggasdf

#include "CSingleton.h"

namespace wood{
	namespace comm{
		namespace font{
			class CWoodFont{
			public:
				int Init();
				void ShowString( GLfloat fX, GLfloat fY, GLfloat fColorR,GLfloat fColorG,GLfloat fColorB,char * sFormat,...);
				void ShowString( GLfloat fX, GLfloat fY, char * sFormat,...);
				void Print(char *s);

			private:
				bool m_bIsInited;
			};
		}
	}
}


typedef CSingletonT<wood::comm::font::CWoodFont, CDummyLock> CWoodFontSingleton;
#define CWOOD_FONT CWoodFontSingleton::Instance()


#endif