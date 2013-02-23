#ifndef __WOODFIRE_COMM_H_ASDSDASDASDASDA____
#define __WOODFIRE_COMM_H_ASDSDASDASDASDA____

#include <GL/glut.h>
#include <GL/glext.h>
#include <iostream>

#define LOG_NOW(sInfo) do{\
	std::cout<<sInfo<<std::endl;\
}while(0)\

enum DWOOD_DIRECTION{  //0 for up , 3 for right, 2 for down, 1 for left;
	DWOOD_DIRECTION_UP    = 0,
	DWOOD_DIRECTION_LEFT  = 1,
	DWOOD_DIRECTION_DOWN  = 2,
	DWOOD_DIRECTION_RIGHT = 3,
	DWOOD_DIRECTION_NONE  = 4,
};

enum DWOOD_TANK_TYPE{
	DWOOD_TANK_TYPE_NONE           = 0,
	DWOOD_TANK_TYPE_ENEMY          = 1,
	DWOOD_TANK_TYPE_ENEMY_PLAYER_1 = 2,
};

enum DWOOD_BULLET_TYPE{
	DWOOD_BULLET_TYPE_NONE            = 0,
	DWOOD_BULLET_TYPE_ENEMY_PLAYER_1  = 1,
	DWOOD_BULLET_TYPE_ENEMY_PLAYER_2  = 2,
};

#define FWOOD_BORDERLINE_LEFT  0.0
#define FWOOD_BORDERLINE_RIGHT 15.0
#define FWOOD_BORDERLINE_DOWN  0.0
#define	FWOOD_BORDERLINE_UP    15.0


enum DWOOD_COLLISION_DIRECTION{
	DWOOD_COLLISION_DIRECTION_NO       = 0,
	DWOOD_COLLISION_DIRECTION_SAME     = 1,
	DWOOD_COLLISION_DIRECTION_OPPOSITE = 2,
	DWOOD_COLLISION_DIRECTION_CROSSED  = 3,
};


typedef struct{
	float fXLeftDown;
	float fYLeftDown;
	float fXRightUp;
	float fYRightUp;
}ST_WOOD_RECT;


//<!----------------- for windows
#include <sys/types.h>
#include <sys/timeb.h>
inline unsigned GetRandomNum(){
   struct __timeb64 timebuffer;
   _ftime64_s( &timebuffer );
	return (unsigned)timebuffer.millitm;
}

//--------------!>





#endif
