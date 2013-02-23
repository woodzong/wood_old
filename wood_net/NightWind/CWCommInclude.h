/*
Author: woodzong@qq.com
Date: 2009-07-12
*/

#ifndef _CWCOMM_INCLUDE_1371893asdfasdf
#define _CWCOMM_INCLUDE_1371893asdfasdf

namespace wood{

#define NW_HTTP_NORMAL_DIVIDER     "\r\n"
#define NW_HTTP_HEADER_DIVIDER     "\r\n\r\n"
#define NW_HTTP_RESP_CHUNCKED_END  "\r\n0\r\n\r\n"


#define NW_HTTP_CONTENT_MAX_SIZE (1024*1024)   // 1M

enum NW_TRANSFER_ENCODE_TYPE{
	NW_TE_TYPE_DEFAULT  = 0,
	NW_TE_TYPE_CHUNCKED = 1,	
};

enum NW_CONTENT_ENCODE_TYPE{
	NW_CE_TYPE_DEFAULT  = 0,
	NW_CE_TYPE_GZIP     = 1,	
};


#define CW_ERR_INFO_FORMAT        "FILE:%s LN:%d FUN:%s EN:%d EM:"
#define CW_DEBUG_INFO_FORMAT      "[FILE:%s LN:%d FUN:%s] INFO:"
#define CW_LOG_DEBUG_INFO_CONTENT     __FILE__,__LINE__,__FUNCTION__

#define SNPRINTF_DEBUG(buffer,isize,sformat,args...)do{\
    snprintf(buffer,isize,CW_DEBUG_INFO_FORMAT sformat,CW_LOG_DEBUG_INFO_CONTENT,##args);\
}while(0)

#define SNPRINTF_ERR(iErrCode,buffer,isize,sformat,args...)do{\
    snprintf(buffer,isize,CW_ERR_INFO_FORMAT sformat,CW_LOG_DEBUG_INFO_CONTENT,iErrCode,##args);\
}while(0)

#define CW_LOG(sformat,args...)do{\
    printf(sformat"\n",##args);\
}while(0)

#define CW_LOG_ERR(iErrCode,sformat,args...)do{\
    printf(CW_ERR_INFO_FORMAT sformat"\n",CW_LOG_DEBUG_INFO_CONTENT,iErrCode,##args);\
}while(0)

#define CW_LOG_DEBUG(sformat,args...)do{\
    printf(CW_DEBUG_INFO_FORMAT sformat"\n",CW_LOG_DEBUG_INFO_CONTENT,##args);\
}while(0)


};

#endif


