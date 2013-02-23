//author: woodzong
//create date: 2009-12-09
/////////////////////////////////////////

#define INITGUID  // make sure al the COM interfaces are available
#define WIN32_LEAN_AND_MEAN  

#include <windows.h>   // include important windows stuff
#include <windowsx.h> 

#include <d3dx9.h>
#include <string>

#include <assert.h>
#include <stdio.h> 
#include <math.h>


#include "CWoodLog.h"
#include "CWMath.h"
#include "CWDraw.h"
#include "CWCamera.h"
#include "CWRender.h"
#include "CWObject.h"



#define W_WINDOW_CLASS_NAME "WoodFreeClass"
#define W_WINDOW_TITLE      "T3D Graphics Console Ver 2.0"
#define W_WINDOW_WIDTH      1024   // size of window
#define W_WINDOW_HEIGHT     768


// this builds a 32 bit color value in A.8.8.8 format (8-bit alpha mode)
#define _RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))


int g_iIndex = 0;


POINT4D        g_poly_pos = {0,0,100,1}; // world position of polygon

// all your initialization code goes here...
VECTOR4D g_vscale={5.0,5.0,5.0,1}, g_vpos = {0,0,0,1}, g_vrot = {0,0,0,1};

CWRender g_oWRender;

ST_POLY4REND g_stPloy4Rend;

CWCamera g_oWCamera;

CWObject g_oWObjectMyCube;


int Game_Init(HWND hWnd,IDirect3DDevice9** pD3D9Device, void *parms = NULL, int num_parms = 0)
{
	int iRetCode = 0;
	IDirect3D9* pD3d9 = NULL;
    pD3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if( pD3d9 == NULL ){
		iRetCode = -1;
		CWOOD_LOG_ERR(iRetCode,"Direct3DCreate9 Failed pD3d9:%d D3D_SDK_VERSION:%d",pD3d9,D3D_SDK_VERSION);
		return iRetCode;
	}

	D3DDEVTYPE enDevType = D3DDEVTYPE_HAL; 
	D3DCAPS9 stD3DCaps;
	pD3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, enDevType, &stD3DCaps);
	int vp = 0;
	if( stD3DCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ){
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}



	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth            = W_WINDOW_WIDTH;
	d3dpp.BackBufferHeight           = W_WINDOW_HEIGHT;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hWnd;
	d3dpp.Windowed                   = true;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	HRESULT hrRetCode = 0;
	hrRetCode = pD3d9->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		enDevType,         // device type
		hWnd,               // window associated with device
		vp,                 // vertex processing
	    &d3dpp,             // present parameters
	    pD3D9Device);            // return created device
	if( FAILED(hrRetCode) ){
		CWOOD_LOG_ERR(hrRetCode,"CreateDevice Failed pD3d9:%d ColorDepth 32",pD3d9);

		// try again using a 16-bit depth buffer
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		
		hrRetCode = pD3d9->CreateDevice(D3DADAPTER_DEFAULT,enDevType,hWnd,vp,&d3dpp,pD3D9Device);
		if( FAILED(hrRetCode) ){
			CWOOD_LOG_ERR(hrRetCode,"CreateDevice Failed pD3d9:%d ColorDepth 16",pD3d9);
			pD3d9->Release(); // done with d3d9 object
			return hrRetCode;
		}
	}

	pD3d9->Release(); // done with d3d9 object

	CWOOD_LOG_DEBUG("GameInit Finished, DeviceP:%d",pD3D9Device);


	// initialize a single polygon
	g_stPloy4Rend.iState  = CW_POLY4DV1_STATE_ACTIVE;
	g_stPloy4Rend.iAttr   =  0; 
	g_stPloy4Rend.iColor  = _RGB32BIT(255,0,255,255);
	  
	g_stPloy4Rend.vlist[0].x = 0;
	g_stPloy4Rend.vlist[0].y = 30;
	g_stPloy4Rend.vlist[0].z = 0;
	g_stPloy4Rend.vlist[0].w = 1;

	g_stPloy4Rend.vlist[1].x = 30;
	g_stPloy4Rend.vlist[1].y = -30;
	g_stPloy4Rend.vlist[1].z = 0;
	g_stPloy4Rend.vlist[1].w = 1;

	g_stPloy4Rend.vlist[2].x = -30;
	g_stPloy4Rend.vlist[2].y = -30;
	g_stPloy4Rend.vlist[2].z = 0;
	g_stPloy4Rend.vlist[2].w = 1;

	g_stPloy4Rend.pNext = NULL;
	g_stPloy4Rend.pPrev = NULL;



	POINT4D  stPointCamPosition = {0,0,-100,1};
	VECTOR4D stVecCamDirection  = {0,0,0,1};

	g_oWCamera.Init(CW_CAM_MODEL_EULER,&stPointCamPosition,&stVecCamDirection, NULL,50.0,500.0,90.0,W_WINDOW_WIDTH,W_WINDOW_HEIGHT);

	// all your initialization code goes here...
	VECTOR4D vscale={5.0,5.0,5.0,1}, vpos = {0,0,0,1}, vrot = {0,0,0,1};
	g_oWObjectMyCube.InitFromFile("./wood_cube.plg", &vscale, &vpos, &vrot);

	return 0;		

} // end Game_Init





int Game_Main(HWND hWnd,IDirect3DDevice9* pD3D9Device,void *parms = NULL, int num_parms = 0){
	LONG lRetCode = 0;

	if(pD3D9Device == NULL){
		lRetCode =-99;
		CWOOD_LOG_ERR_L(lRetCode,"Dying Alert!! pD3D9Device is NULL, pD3D9Device:%d",pD3D9Device);		
		return 0;
	}

	// generate rotation matrix around y axis
	static MATRIX4X4 stMatrixRotaion; // general rotation matrix
	CWMath::Build_XYZ_Rotation_MATRIX4X4(g_iIndex, 0, 0, &stMatrixRotaion);

	static MATRIX4X4 stMatrixMove; // general move matrix
	CWMath::Build_XYZ_Move_MATRIX4X4(10, 0, 0, &stMatrixMove);

	g_oWRender.Reset();

	g_oWRender.AddPoly4Rend(g_stPloy4Rend);

	//g_oWObjectMyCube.TransForm(&stMatrixMove,CW_TRANSFORM_TRANS_ONLY);
	
	g_oWObjectMyCube.TransForm(&stMatrixRotaion,CW_TRANSFORM_TRANS_ONLY);

	g_oWRender.AddObject(g_oWObjectMyCube,0);	

	//g_oWRender.Transform(&stMatrixRotaion,CW_TRANSFORM_LOCAL_ONLY);

	g_oWRender.ModelToWorld(&g_poly_pos,CW_TRANSFORM_LOCAL_TO_TRANS);

	g_oWCamera.MakeMatrixEuler(CW_CAM_ROT_SEQ_ZYX);

	g_oWRender.WorldToCamera(&g_oWCamera);

	g_oWRender.CameraToPerspective(&g_oWCamera);

	g_oWRender.PerspectiveToScreen(&g_oWCamera);

	/*
	IDirect3DSurface9* pD9Surface = NULL;
	iRetCode = pD3D9Device->CreateOffscreenPlainSurface(
		W_WINDOW_WIDTH, 
		W_WINDOW_HEIGHT, 
		D3DFMT_X8R8G8B8, 
		D3DPOOL_DEFAULT, 
		&pD9Surface,
		NULL);
	if(iRetCode != D3D_OK){
		CWOOD_LOG_ERR(iRetCode,"CreateOffscreenPlainSurface Failed, pD9Surface:%d",pD9Surface);
		return iRetCode;
	}
	*/


	pD3D9Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, _RGB32BIT(255,99,99,99), 1.0f, 0);
	pD3D9Device->BeginScene();

	IDirect3DSurface9* pBackSurface = NULL;
	lRetCode = pD3D9Device->GetBackBuffer( 0,0,D3DBACKBUFFER_TYPE_MONO,&pBackSurface );
	if(lRetCode != D3D_OK){
		CWOOD_LOG_ERR_L(lRetCode,"pD3D9Device->GetBackBuffer Failed, pBackSurface:%d",pBackSurface);
		return lRetCode;
	}

	g_iIndex ++;
	if(g_iIndex>=360000){
		g_iIndex = 0;		
	}

	CWDraw oWDraw(pBackSurface);

	g_oWRender.RendItOut(oWDraw);

	oWDraw.Release();

	pBackSurface->Release();

	pD3D9Device->EndScene();

	CWOOD_LOG_DEBUG("GOOD===============================");
	// Swap the back and front buffers.
	pD3D9Device->Present(0, 0, 0, 0);
	return 0;

}


int Game_Shutdown(void *parms = NULL, int num_parms = 0)
{

	return 0;

} // end Game_Shutdown


//deal the main process for windows.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT stMsg,WPARAM wParam,LPARAM lParam){

	PAINTSTRUCT		ps;		// used in WM_PAINT

	switch(stMsg){
		case WM_PAINT: 
		{
			// simply validate the window 
			//BeginPaint(hWnd,&ps);	 

			// end painting
			//EndPaint(hWnd,&ps);
			// return success
			return(0);
		} 
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:break;
	}
	return (DefWindowProc(hWnd, stMsg, wParam, lParam));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow){

	int iRetCode = CWOOD_OLOG->Init("WoodFree","C:\\CWLOG",true);
	if( iRetCode != 0 ){
		//W_PRINT_ERROR_MSG_CHAR_INFO(iRetCode,"CWOOD_OLOG->Init Failed ErrMsg:%s",CWOOD_OLOG->GetLastErrMsg());
		W_BOX_ALERT_CHAR_INFO(iRetCode,"CWOOD_OLOG->Init Failed ErrMsg:%s",CWOOD_OLOG->GetLastErrMsg());
		return iRetCode;
	}

	CWOOD_LOG_DEBUG(" ");
	CWOOD_LOG_DEBUG(" ==================================== Compile Time:%s %s",__DATE__,__TIME__);

	WNDCLASS stWinclass;	// this will hold the class we create
	HWND	 hWnd;		// generic window handle
	MSG		 stMsg;		// generic message
	HDC      hDc;       // generic dc
	PAINTSTRUCT stPaint;     // generic paintstruct

	// first fill in the window class stucture
	stWinclass.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	stWinclass.lpfnWndProc	    = WindowProc;
	stWinclass.cbClsExtra		= 0;
	stWinclass.cbWndExtra		= 0;
	stWinclass.hInstance		= hInstance;
	stWinclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	stWinclass.hCursor		    = LoadCursor(NULL, IDC_ARROW);
	stWinclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	stWinclass.lpszMenuName	    = NULL; 
	stWinclass.lpszClassName	= W_WINDOW_CLASS_NAME;

	// register the window class
	iRetCode = RegisterClass(&stWinclass);
	if ( iRetCode == 0 ){ //0 means error. NULL
		CWOOD_LOG_ERR(iRetCode,"%s","RegisterClass Failed!!");
		return iRetCode;
	}

	// create the window, note the test to see if WINDOWED_APP is
	// true to select the appropriate window flags
	hWnd = CreateWindow(W_WINDOW_CLASS_NAME, // class
		W_WINDOW_TITLE,	 // title
		WS_POPUP | WS_VISIBLE,
		0,0,	   // x,y
		W_WINDOW_WIDTH,  // width
		W_WINDOW_HEIGHT, // height
		NULL,	   // handle to parent 
		NULL,	   // handle to menu
		hInstance,// instance from outside.
		NULL);

	if( hWnd == NULL ){
		iRetCode = -1;
		CWOOD_LOG_ERR(iRetCode,"%s %d","CreateWindow Failed!!");
		return iRetCode;
	}
	/*
	RECT stWindowsRect = {0,0,W_WINDOW_WIDTH-1,W_WINDOW_HEIGHT-1};

	// make the call to adjust window_rect
	AdjustWindowRectEx(&stWindowsRect,
		GetWindowLong(hWnd, GWL_STYLE),
		GetMenu( hWnd ) != NULL,
		GetWindowLong(hWnd, GWL_EXSTYLE));

	// save the global client offsets, they are needed in DDraw_Flip()
	int window_client_x0 = -stWindowsRect.left;
	int window_client_y0 = -stWindowsRect.top;

	// now resize the window with a call to MoveWindow()

	MoveWindow(hWnd,
		0, // x position
		0, // y position
		stWindowsRect.right - stWindowsRect.left, // width
		stWindowsRect.bottom - stWindowsRect.top, // height
		FALSE);

	ShowWindow(hWnd, SW_SHOW);
	*/

	IDirect3DDevice9* pD3D9Device = NULL;

	iRetCode = Game_Init(hWnd,&pD3D9Device);
	if ( iRetCode != 0 ){
		CWOOD_LOG_ERR(iRetCode,"%s","Game_Init Failed!!");
		return iRetCode;
	}

	//============================================================================================
	//============================================================================================

	while(TRUE)
	{
		// test if there is a message in queue, if so get it
		if (PeekMessage(&stMsg,NULL,0,0,PM_REMOVE))
		{ 
			// test if this is a quit
			if (stMsg.message == WM_QUIT)
				break;

			// translate any accelerator keys
			TranslateMessage(&stMsg);

			// send the message to the window proc
			DispatchMessage(&stMsg);
		} // end if
		if (KEYDOWN(VK_ESCAPE)){
				SendMessage(hWnd,WM_CLOSE,0,0);	
		}
		// main game processing goes here		
		Game_Main(hWnd,pD3D9Device);
	} // end while

	Game_Shutdown();	

	return(stMsg.wParam);

}