#ifndef WCOMM_H_12314asdfas123123
#define WCOMM_H_12314asdfas123123

//#include <stdint.h>
#define int32_t  int
#define uint8_t  unsigned char
#define uint32_t unsigned int
#define W_LP_BUFFER void*

#define WOOD_ERR_INFO_FORMAT        "FILE:%s LN:%d FUN:%s RET:%d EM:"
#define WOOD_ERR_INFO_FORMAT_LONG        "FILE:%s LN:%d FUN:%s RET:%lu EM:"
#define WOOD_DEBUG_INFO_FORMAT      "FILE:%s LN:%d FUN:%s INFO:"
#define WOOD_DEBUG_INFO_CONTENT     __FILE__,__LINE__,__FUNCTION__

#define W_MAKE_ERROR_MSG_CHAR_INFO(sErrMsg,iRetCode,sformat,...)_snprintf_s( sErrMsg, sizeof(sErrMsg),sizeof(sErrMsg),"FILE:%s LN:%d FUN:%s RET:%d EM:"sformat,__FILE__,__LINE__,__FUNCTION__,iRetCode,__VA_ARGS__)
#define W_PRINT_ERROR_MSG_CHAR_INFO(iRetCode,sformat,...)printf( "FILE:%s LN:%d FUN:%s RET:%d EM:"sformat,__FILE__,__LINE__,__FUNCTION__,iRetCode,__VA_ARGS__)
#define W_BOX_ALERT_CHAR_INFO(iRetCode,sformat,...)do{\
	char sBuffer[1024];\
	W_MAKE_ERROR_MSG_CHAR_INFO(sBuffer,iRetCode,sformat,__VA_ARGS__);\
	MessageBox(NULL,sBuffer,"ERROR!!",MB_OK|MB_ICONEXCLAMATION);\
}while(0)


#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

// initializes a direct draw struct, basically zeros it and sets the dwSize field
#define DDRAW_INIT_STRUCT(ddstruct){ memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }

#define _RGB16BIT555(r,g,b) ((b & 31) + ((g & 31) << 5) + ((r & 31) << 10))
// this builds a 16 bit color value in 5.6.5 format (green dominate mode)
#define _RGB16BIT565(r,g,b) ((b & 31) + ((g & 63) << 5) + ((r & 31) << 11))
// this builds a 32 bit color value in A.8.8.8 format (8-bit alpha mode)
#define _RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))
#define RGB32BIT(r,g,b) ((b) + ((g) << 8) + ((r) << 16) + (0 << 24))

// bit manipulation macros
#define SET_BIT(word,bit_flag)   ((word)=((word) | (bit_flag)))
#define RESET_BIT(word,bit_flag) ((word)=((word) & (~bit_flag)))

#endif