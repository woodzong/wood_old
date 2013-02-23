/*
Author: woodzong@qq.com
Date: 2009-07-12
*/

#ifndef _CWZipOp_WWW_DDEEE1238178648714671234
#define _CWZipOp_WWW_DDEEE1238178648714671234

#include "zlib.h" 

namespace wood{
    class CWZipOp{
        public:
            static int InFlateHttpGZip(unsigned char * sInContent, int iInContentSize,unsigned char * sOutBuffer,
                                        int iOutBufferSize, char * strErrMsg, int iErrMsgSize );
    };
};

#endif


