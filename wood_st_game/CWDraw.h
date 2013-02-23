#ifndef __CWDRAW_H_12314asdfas123123
#define __CWDRAW_H_12314asdfas123123
#include <windows.h>   // include important windows stuff
#include <windowsx.h> 
#include <d3dx9.h>



// internal clipping codes
#define CLIP_CODE_C  0x0000
#define CLIP_CODE_N  0x0008
#define CLIP_CODE_S  0x0004
#define CLIP_CODE_E  0x0002
#define CLIP_CODE_W  0x0001

#define CLIP_CODE_NE 0x000a
#define CLIP_CODE_SE 0x0006
#define CLIP_CODE_NW 0x0009 
#define CLIP_CODE_SW 0x0005


class CWDraw{
public:
	CWDraw(IDirect3DSurface9* _pSurface);
	~CWDraw();
	void Init();
	void Release();
	
	void DrawPoint32Bit(int x,int y,DWORD dwColor){
		if(m_bIsReleased){	return; }
		m_pData[ y*m_iPitch32+x ] = dwColor;
	}

	void DrawClipLine32Bit(int x0,int y0, int x1, int y1, DWORD dwColor);

	void DrawLine32Bit(int x0, int y0, // starting position 
						  int x1, int y1, // ending position
						  DWORD dwColor);     // color index

private:
	int ClipLine(int &x1,int &y1,int &x2, int &y2); //0 for outside the screen, 1 for cliped.

private:
	IDirect3DSurface9* m_pSurface;
	int m_iPitch32;
	int m_iPitch16;
	DWORD * m_pData;
	bool m_bIsReleased;
	int m_iScreenWidth;
	int m_iScreenHeight;
};


#endif