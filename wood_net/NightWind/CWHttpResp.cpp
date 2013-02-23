/*
Author: woodzong@qq.com
Date: 2009-07-12
*/
#include "CWHttpResp.h"
#include "CWRegex.h"
#include "CWZipOp.h"
#include "CWTools.h"


using namespace std;
using namespace wood;


CWHttpResp::CWHttpResp(){
	m_iRetCode = 0;
	memset(m_sErrMsg,0,sizeof(m_sErrMsg));
	memset(&m_stRespHttpHeader,0,sizeof(m_stRespHttpHeader));
	m_iAllContentLen = 0;
	memset(m_sAllContentBuffer,0,sizeof(m_sAllContentBuffer));
	m_iBodyLen = 0;
	memset(m_sBodyPureInfo,0,sizeof(m_sBodyPureInfo));
	m_bIsLoadedHeaderInfo = false;
}

CWHttpResp& CWHttpResp::operator = ( CWHttpResp& oWHttpResp ){

    this->m_iRetCode = 0;          
    memset(this->m_sErrMsg,0,sizeof(this->m_sErrMsg));
    
    this->m_bIsLoadedHeaderInfo = oWHttpResp.IsLoadedHeaderInfo();
    
    memcpy(&(this->m_stRespHttpHeader),&(oWHttpResp.GetRespHttpHeader()),sizeof(this->m_stRespHttpHeader));

    this->m_strHeaderContent = oWHttpResp.GetHeaderContent();

    this->m_iAllContentLen = oWHttpResp.GetAllContentLen();

    memcpy(this->m_sAllContentBuffer,oWHttpResp.GetAllContentBuffer(),oWHttpResp.GetAllContentLen());

    this->m_iBodyLen = oWHttpResp.GetBodyLen();

	memcpy(this->m_sBodyPureInfo,oWHttpResp.GetBodyBuffer(),oWHttpResp.GetBodyLen());

	this->m_strBodyInfo = oWHttpResp.GetBodyInfo();
	
	return *this;
}   



int CWHttpResp::AddReadBuffer(const char * pBufferIn, int iInBufferLen){
	if( pBufferIn == NULL || iInBufferLen == 0 ){
		m_iRetCode = -1;
		SNPRINTF_ERR(m_iRetCode,m_sErrMsg,sizeof(m_sErrMsg),"Err Param InBuffer:%p BufferLen:%d",pBufferIn,iInBufferLen);
		return m_iRetCode;
	}
	
	if( (unsigned int)(m_iAllContentLen+iInBufferLen) > sizeof(m_sAllContentBuffer) ){
		m_iAllContentLen-=iInBufferLen;				
		m_iRetCode = -2;
		SNPRINTF_ERR(m_iRetCode,m_sErrMsg,sizeof(m_sErrMsg),"Target Buffer Is Too Large InBufferLen:%d ReadLen:%d",
					 iInBufferLen,m_iAllContentLen);		
		return m_iRetCode;
	}

	memcpy(m_sAllContentBuffer+m_iAllContentLen,pBufferIn,iInBufferLen);

	m_iAllContentLen += iInBufferLen;

	//start to load header info...
	if(	m_bIsLoadedHeaderInfo == false ){
		m_bIsLoadedHeaderInfo = LoadHeaderInfo();
	}
	
	return 0;
}


bool CWHttpResp::IsLoadedHeaderInfo(){
	return m_bIsLoadedHeaderInfo;
}


void CWHttpResp::DebugDumpHeaderInfo(std::ostream & oss){
	oss<<"====================================="<<std::endl;
	oss<<"==Pure HeaderInfo===================="<<std::endl;
	oss<<"==HeadInfoLen:"<<m_strHeaderContent.length()<<std::endl;
	oss<<m_strHeaderContent<<std::endl;
	oss<<"==HeaderInfo::ContentLen:"<<m_stRespHttpHeader.iContentLen<<std::endl;
	oss<<"==HeaderInfo::ContentEncodingType:"<<m_stRespHttpHeader.iContentEncodingType<<std::endl;
	oss<<"==HeaderInfo::TransferEncodingType:"<<m_stRespHttpHeader.iTransferEncodingType<<std::endl;	
	oss<<"====================================="<<std::endl;
}


int CWHttpResp::GetHeaderContentLen(){
	if(m_bIsLoadedHeaderInfo){
		return m_stRespHttpHeader.iContentLen;
	}
	return -1;
}


int CWHttpResp::GetHeaderTransferEncodingType(){
	if(m_bIsLoadedHeaderInfo){
		return m_stRespHttpHeader.iTransferEncodingType;
	}
	return -1;
}


int CWHttpResp::GetHeaderContentEncodingType(){
	if(m_bIsLoadedHeaderInfo){
		return m_stRespHttpHeader.iContentEncodingType;
	}
	return -1;
}


bool CWHttpResp::IsStillNeedToRead(){
	if( GetHeaderContentLen() > 0 ){
		return  (unsigned int) m_iAllContentLen < ( m_stRespHttpHeader.iContentLen + m_strHeaderContent.length() ) ;
	}
	else if( GetHeaderTransferEncodingType() == NW_TE_TYPE_CHUNCKED ){
		return CWTools::strstrBuffer(m_sAllContentBuffer, m_iAllContentLen, NW_HTTP_RESP_CHUNCKED_END, strlen(NW_HTTP_RESP_CHUNCKED_END) ) == NULL;
	}	
	return false;
}



int CWHttpResp::MakeBodyInfo(){
	//CW_LOG("==============GetHeaderContentLen():%d",GetHeaderContentLen());

	if( GetHeaderContentLen() > 0 ){ //fixed content len
	
		unsigned char * pTmp = (unsigned char *)(m_sAllContentBuffer + m_strHeaderContent.length());
		m_iBodyLen = m_iAllContentLen - m_strHeaderContent.length() - 2; // 2 for \r\n
		if( m_iBodyLen <= 0 ){
			m_iRetCode = -3;
			SNPRINTF_ERR(m_iRetCode,m_sErrMsg,sizeof(m_sErrMsg),"BodyLen Is Too Small BodyLen:%d AllContentLen:%d HeaderLen:%d",
							m_iBodyLen, m_iAllContentLen, m_strHeaderContent.length());			
			return m_iRetCode;
		}
		memcpy(m_sBodyPureInfo,pTmp,m_iBodyLen);

		//m_iBodyLen = iBodyLen;
	}
	else if( GetHeaderTransferEncodingType() == NW_TE_TYPE_CHUNCKED ){ //merge chunk data...
		//printf("--------------------------------NW_TE_TYPE_CHUNCKED\n");
		int iChunkedSize = 0;
		int iLeftSize = m_iAllContentLen - m_strHeaderContent.length();
		const char * pChunkedStart = m_sAllContentBuffer + m_strHeaderContent.length();
		const char * pChunkedEnd   = m_sAllContentBuffer + m_iAllContentLen;
		//unsigned char sChunkedBuffer[1024*1024] =  {0} ; // 1M.
		//int iFinChunkedSize = 0;

		do{
			iChunkedSize = 0;
			pChunkedStart = GetNextChunkSize(pChunkedStart,iLeftSize,iChunkedSize);
			if( iChunkedSize == 0 ){
				break;
			}

			//printf("ChunkedSize:%d Pointer:%p Pointer:%d \n",iChunkedSize,pChunkedStart,*pChunkedStart );

			//memcpy(sChunkedBuffer+iFinChunkedSize,pChunkedStart,iChunkedSize);
			memcpy(m_sBodyPureInfo+m_iBodyLen,pChunkedStart,iChunkedSize);
			
			
			m_iBodyLen += iChunkedSize;
			pChunkedStart = pChunkedStart + (iChunkedSize+2);
		}
		while( pChunkedStart < pChunkedEnd );
	}		


	//start to unzip if need

	if( GetHeaderContentEncodingType() == NW_CE_TYPE_GZIP ){
	
		//unsigned char * pTmp = (unsigned char *)(sAllContent + strHeaderInfo.length());
		//int iBodyLen = iTotalReadedBytes - strHeaderInfo.length();
		char sOutBuffer[NW_HTTP_CONTENT_MAX_SIZE] = {0};
		//char sErrMsg[1024] = {0};
		
		//printf("--------------------------------MakeBodyInfo:%d\n",m_iBodyLen);
		m_iRetCode = CWZipOp::InFlateHttpGZip( (unsigned char *)m_sBodyPureInfo, m_iBodyLen, 
											   (unsigned char *)sOutBuffer, sizeof(sOutBuffer),
											   m_sErrMsg,sizeof(m_sErrMsg) );
		if( m_iRetCode != 0 ){					
			CW_LOG("InFlateHttpGZip Failed RetCode:%d ErrMsg:%s",m_iRetCode,m_sErrMsg);
			return m_iRetCode;
		}

		m_strBodyInfo = sOutBuffer;

		//printf("OUTBUFFER Len:%u Content:%s\n",strlen( (char *) sOutBuffer),sOutBuffer);				
	}
	else{
		m_strBodyInfo = m_sBodyPureInfo;
	}

	return 0;			
}



bool CWHttpResp::LoadHeaderInfo(){
	const char * pHeaderEnd = CWTools::strstrBuffer(m_sAllContentBuffer,m_iAllContentLen,NW_HTTP_HEADER_DIVIDER, strlen(NW_HTTP_HEADER_DIVIDER) ); // not include the "\0".
	if(!pHeaderEnd){
		return false;
	}

	m_strHeaderContent.assign( m_sAllContentBuffer, pHeaderEnd - m_sAllContentBuffer + strlen(NW_HTTP_HEADER_DIVIDER) ); // 2 for one "\r\n";

	ConverRespHeaderIntoStruct(m_strHeaderContent,m_stRespHttpHeader);
	return true;
}






const char * CWHttpResp::GetNextChunkSize(const char * pStart, int iSize, int & iChunkedSize ){
	const char * pTmp = CWTools::strstrBuffer(pStart, iSize, "\r\n", 2);
	if(pTmp && pTmp!=pStart ){ // not empty.
		char sTmp[10] = {0};
		memcpy(sTmp,pStart,pTmp-pStart);
		iChunkedSize = (int)strtol(sTmp, NULL, 16);
		return pTmp+2;
	}
	iChunkedSize = 0;
	return 0;
}



void CWHttpResp::ConverRespHeaderIntoStruct(const std::string & strHeaderInfo,STRespHttpHeader & stRespHttpHeaderInfo){

	std::vector<std::string> vecResult;

	CWRegex oWRegex;
	int iRetCode = oWRegex.FindRegSubMatchFull("Content-Length: ([0-9]+)",strHeaderInfo.c_str(), vecResult);
	if( iRetCode != 0 && iRetCode != REG_NOMATCH ){
		CW_LOG("%s",oWRegex.GetLastErrMsg());
		return;
	}

	//printf("vecResult Size:%d HeaderInfo:%s\n ",vecResult.size(),strHeaderInfo.c_str());
	
	std::vector<std::string>::const_iterator it    = vecResult.begin();
	if(it!=vecResult.end()){
		stRespHttpHeaderInfo.iContentLen = CWTools::SafeAtoi(it->c_str());
	}

	if( strHeaderInfo.find("Content-Encoding: gzip") != std::string::npos ){
		stRespHttpHeaderInfo.iContentEncodingType  = NW_CE_TYPE_GZIP;
	}

	if( strHeaderInfo.find("Transfer-Encoding: chunked") != std::string::npos ){
		stRespHttpHeaderInfo.iTransferEncodingType = NW_TE_TYPE_CHUNCKED;
	}
	
}




