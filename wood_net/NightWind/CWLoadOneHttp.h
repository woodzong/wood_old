/*
Author: woodzong@qq.com
Date: 2009-07-12
*/

#ifndef _CWLOAD_ONE_HTTP_ASDASD1238791283791742
#define _CWLOAD_ONE_HTTP_ASDASD1238791283791742

#include <sstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <errno.h>
#include <string.h>
#include "CWHttpReq.h"
#include "CWHttpResp.h"
#include "CWCommInclude.h"

namespace wood{

//find target pointer in buffer by divider buffer;

class CWLoadOneHttp{
	public:
		CWLoadOneHttp();

        int Load(const wood::CWHttpReq& oWHttpReq, CWHttpResp & oWHttpResp);

    private:
        int Connect(const CWHttpReq & oReq);
        int SendAndRead(int iConnectedFd, const CWHttpReq & oReq, CWHttpResp & oWHttpResp);
        
        
	private:
        int m_iRetCode;
        char m_sErrMsg[1024];
        int m_iSocketFd;
		CWHttpResp m_oWHttpResp;
        CWHttpReq  m_oWHttpReq;
};

};

#endif


