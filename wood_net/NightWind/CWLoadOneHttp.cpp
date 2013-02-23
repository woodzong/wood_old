/*
Author: woodzong@qq.com
Date: 2009-07-12
*/

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>

#include "CWLoadOneHttp.h"


using namespace std;
using namespace wood;


CWLoadOneHttp::CWLoadOneHttp(){
	m_iRetCode = 0;
	memset(m_sErrMsg,0,sizeof(m_sErrMsg));
	m_iSocketFd = 0;
}


int CWLoadOneHttp::Load(const wood::CWHttpReq& oWHttpReq, CWHttpResp & oWHttpResp){
	m_oWHttpReq = oWHttpReq;
	m_iSocketFd= Connect(m_oWHttpReq);
	if( m_iSocketFd < 0 ){
		m_iRetCode = m_iSocketFd;
		CW_LOG_ERR(m_iRetCode,"Connect Failed!! ErrMsg:%s",m_sErrMsg);
		return m_iRetCode;
	}

	m_iRetCode = SendAndRead(m_iSocketFd, m_oWHttpReq,oWHttpResp);
	if(m_iRetCode!=0){
		close(m_iSocketFd);
		CW_LOG_ERR(m_iRetCode,"SendAndRead Failed!! ErrMsg:%s",m_sErrMsg);
		return m_iRetCode;
	}

	m_oWHttpResp = oWHttpResp;
	close(m_iSocketFd);
	return 0;
	
}


int CWLoadOneHttp::Connect(const CWHttpReq & oReq){
	int iRetCode = 0;

	struct sockaddr_in stServerAddr;
	memset(&stServerAddr,0,sizeof(stServerAddr));		

	stServerAddr.sin_port  = htons( oReq.GetReqPort() );
	stServerAddr.sin_family = AF_INET;
	
	m_iRetCode = inet_pton(AF_INET, oReq.GetReqIP().c_str(), &(stServerAddr.sin_addr));
	if(iRetCode<0){
		SNPRINTF_ERR(iRetCode, m_sErrMsg, sizeof(m_sErrMsg), "IP:%s Format Error", oReq.GetReqIP().c_str());
		return -1;
	}

	
	CW_LOG_DEBUG("Connecting Ip:%s Port:%d",inet_ntoa(stServerAddr.sin_addr), oReq.GetReqPort());


	//start to do somthing for the socket ...
	int iConnectedFd = 0;	
	
	// start to prepare the socket
	// SOCK_STREAM or SOCK_DGRAM (TCP or UDP sockets)
	iConnectedFd = socket(AF_INET, SOCK_STREAM, 0);  
	if ( iConnectedFd < 0 ) {
		SNPRINTF_ERR(iConnectedFd, m_sErrMsg, sizeof(m_sErrMsg), "socket IP:%s Port:%d socket Failed Fd:%d ErrNo:%d ErrMsg:%s",
					oReq.GetReqIP().c_str(),oReq.GetReqPort(), iConnectedFd,errno,strerror(errno));
		return -1;
	}

	struct timeval tmout;
	tmout.tv_sec = 10;
	tmout.tv_usec = 0;
	setsockopt(iConnectedFd, SOL_SOCKET, SO_RCVTIMEO, (void*)&tmout, sizeof(tmout));
	setsockopt(iConnectedFd, SOL_SOCKET, SO_SNDTIMEO, (void*)&tmout, sizeof(tmout));	

	iRetCode = connect(iConnectedFd,(struct sockaddr *)&stServerAddr, sizeof(stServerAddr) );
	if(	iRetCode < 0 ){
		SNPRINTF_ERR(iConnectedFd, m_sErrMsg, sizeof(m_sErrMsg), "socket IP:%s Port:%d connect Failed Fd:%d ErrNo:%d ErrMsg:%s",
					oReq.GetReqIP().c_str(),oReq.GetReqPort(), iConnectedFd,errno,strerror(errno));
		return -1;		
	}
	
	CW_LOG_DEBUG("ConnectedIp:%s Port:%d",oReq.GetReqIP().c_str(), oReq.GetReqPort());
	return iConnectedFd;
	
}




int CWLoadOneHttp::SendAndRead(int iConnectedFd, const CWHttpReq & oReq, CWHttpResp & oWHttpResp){

	int iRetCode = 0;

	std::string strTmpBuffer = oReq.GetReqAllContent();

	CW_LOG_DEBUG("SendContent:%s",strTmpBuffer.c_str());
	CW_LOG_DEBUG("Sending:%s Port:%d Len:%d",oReq.GetReqIP().c_str(), oReq.GetReqPort(),strTmpBuffer.length());
	
	iRetCode = write( iConnectedFd, strTmpBuffer.c_str(), strTmpBuffer.length() );
	if( iRetCode < 0 ){
		SNPRINTF_ERR(iRetCode,m_sErrMsg,sizeof(m_sErrMsg),"write Failed IP:%s Port:%d  Failed Fd:%d ContentLen:%d ErrNo:%d ErrMsg:%s",
				oReq.GetReqIP().c_str(),oReq.GetReqPort(), iConnectedFd,strTmpBuffer.length(),errno,strerror(errno) );
		return iRetCode;	
	}
	//printf("[File:%s Line:%d Fun:%s] Sended:%s Port:%d Len:%d\n",
	//		__FILE__,__LINE__,__FUNCTION__,oReq.GetReqIP().c_str(), oReq.GetReqPort(),strTmpBuffer.length());
	

	struct timeval stTimeVal;
    stTimeVal.tv_sec  = 10;
    stTimeVal.tv_usec = 0;      
    
	int iTmpReadedBytes = 0;
	char sReadBuffer[NW_HTTP_CONTENT_MAX_SIZE] = {0}; // 1 MB,
	bool bGoOn = true;
	
	while( bGoOn && ( iTmpReadedBytes = read(iConnectedFd,sReadBuffer,sizeof(sReadBuffer)) ) >0 ){
		oWHttpResp.AddReadBuffer(sReadBuffer,iTmpReadedBytes);
		memset(sReadBuffer,0,sizeof(sReadBuffer));	
		if( oWHttpResp.IsLoadedHeaderInfo() ){
		
			std::stringstream oss;
			oWHttpResp.DebugDumpHeaderInfo(oss);
			std::cout<<oss.str()<<std::endl;			
			
			while( oWHttpResp.IsStillNeedToRead() ){
				memset(sReadBuffer,0,sizeof(sReadBuffer));
				iTmpReadedBytes = read(iConnectedFd,sReadBuffer,sizeof(sReadBuffer));
				if( iTmpReadedBytes<=0 ){
					SNPRINTF_ERR(-2,m_sErrMsg,sizeof(m_sErrMsg),"Error Data In ReadContentLen info iTmpReadedBytes:%d",iTmpReadedBytes);
					return -2;
				}
				oWHttpResp.AddReadBuffer(sReadBuffer,iTmpReadedBytes);
			}
			bGoOn = false;			
		}
	}

	iRetCode = oWHttpResp.MakeBodyInfo();
	if( iRetCode != 0 ){
		SNPRINTF_ERR(iRetCode,m_sErrMsg,sizeof(m_sErrMsg),"oWHttpResp.MakeBodyInfo Failed ErrMsg:%s",oWHttpResp.GetLastErrMsg());
		return iRetCode;							
	}

	CW_LOG_DEBUG("Load Fin RespBodyInfoLen:%d",oWHttpResp.GetBodyInfo().length());

	//std::cout<<oWHttpResp.GetBodyInfo()<<std::endl;

	
	return iRetCode;
	
}





