/*
Author: woodzong@qq.com
Date: 2009-07-12
*/

#ifndef _CWHTTPRESP_WWW_DDEEE1238178648714671234
#define _CWHTTPRESP_WWW_DDEEE1238178648714671234

#include <sstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include "CWCommInclude.h"

namespace wood{

typedef struct{
	int iContentLen;
	int iTransferEncodingType;  //1 // 1 for chunked...
	int iContentEncodingType;   // 1 for gzip;
} STRespHttpHeader;


//find target pointer in buffer by divider buffer;

class CWHttpResp{
	public:
		CWHttpResp();

        CWHttpResp& operator = ( CWHttpResp& oWHttpReq );
        
		int AddReadBuffer(const char * pBufferIn, int iInBufferLen);
		

		bool IsLoadedHeaderInfo();


		void DebugDumpHeaderInfo(std::ostream & oss);
		

		int GetHeaderContentLen();
		

		int GetHeaderTransferEncodingType();
		

		int GetHeaderContentEncodingType();

		
		bool IsStillNeedToRead();
		

		const char * GetLastErrMsg(){
            return m_sErrMsg;
        }
        

		int GetLastRetCode(){
            return m_iRetCode;
        }

	
		int MakeBodyInfo();


		const std::string & GetBodyInfo(){
            return m_strBodyInfo;
        }

        STRespHttpHeader & GetRespHttpHeader(){
            return m_stRespHttpHeader;
        }

        const std::string & GetHeaderContent(){
            return m_strHeaderContent;
        }

        int GetAllContentLen(){
            return m_iAllContentLen;
        }

        const char * GetAllContentBuffer(){
            return m_sAllContentBuffer;
        }

        int GetBodyLen(){
            return m_iBodyLen;
        }

        const char * GetBodyBuffer(){
            return m_sBodyPureInfo;
        }

	private:	

		bool LoadHeaderInfo();        


        const char * GetNextChunkSize(const char * pStart, int iSize, int & iChunkedSize );
		
	
		void ConverRespHeaderIntoStruct(const std::string & strHeaderInfo,STRespHttpHeader & stRespHttpHeaderInfo);

        
        
	private:
		int m_iRetCode;
		char m_sErrMsg[1024];

		bool m_bIsLoadedHeaderInfo;
	
		//header info
		STRespHttpHeader m_stRespHttpHeader;
		std::string m_strHeaderContent;
		//body info
		int m_iAllContentLen;
		char m_sAllContentBuffer[NW_HTTP_CONTENT_MAX_SIZE];
		
		int m_iBodyLen;
		char m_sBodyPureInfo[NW_HTTP_CONTENT_MAX_SIZE];
		
		std::string m_strBodyInfo;
};

};

#endif


