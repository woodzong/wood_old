#include <string.h>
#include <errno.h>
#include <stdio.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


#include <sys/types.h>
#include <sys/socket.h>



const int WOOD_SERVER_PORT    = 53412;
const int WOOD_LISTEN_BACKLOG = 1024;


void DoSomthing(int iClientFd, struct sockaddr_in * pClientAddr){
	printf("Client Connect OK IP:%s:%d \n",inet_ntoa(pClientAddr->sin_addr), ntohs(pClientAddr->sin_port) );
	char sWriteBuffer[255]={0};
	snprintf(sWriteBuffer,sizeof(sWriteBuffer),"%s","hahahhahahahahah");
	write(iClientFd,sWriteBuffer,sizeof(sWriteBuffer));
	close(iClientFd);
}

int main(){
	int iRetCode  = 0;
	
	int iListenFd  = 0;

	//Start to Init The Server Address
	struct sockaddr_in stServerAddr;
	memset(&stServerAddr,0,sizeof(stServerAddr));
	stServerAddr.sin_family = AF_INET;
	stServerAddr.sin_port   = htons(WOOD_SERVER_PORT);
	stServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// start to prepare the socket
	// SOCK_STREAM or SOCK_DGRAM (TCP or UDP sockets)
	iListenFd = socket(AF_INET, SOCK_STREAM, 0);  
	if ( iListenFd < 0 ) {
		printf("[File:%s Line:%d Fun:%s] socket Failed ErrorListenFd:%d ErrNo:%d ErrMsg:%s \n", 
		        __FILE__,__LINE__,__FUNCTION__,
		        iListenFd,errno,strerror(errno) );
		return -1;
	}

	//set socket into can reuse...
	int reuse_addr = 1;
	iRetCode = setsockopt(iListenFd, SOL_SOCKET, SO_REUSEADDR, (void*)(&(reuse_addr)),sizeof(reuse_addr));
	if( iRetCode < 0 ){
		printf("[File:%s Line:%d Fun:%s] setsockopt Failed ErrorListenFd:%d ErrNo:%d ErrMsg:%s \n", 
				__FILE__,__LINE__,__FUNCTION__,
				iListenFd,errno,strerror(errno) );
		return -1;		
	}

	//start to bind
	struct sockaddr * pSockAddrTmp = (struct sockaddr *) &stServerAddr;
	iRetCode = bind(iListenFd, pSockAddrTmp, sizeof(stServerAddr));
	if ( iRetCode < 0) {
		char sBuffer[14+1] = {0}; // only 14.. because sa_data in sockaddr is 14 len...
		snprintf(sBuffer,sizeof(sBuffer),"%s",pSockAddrTmp->sa_data);
		printf("[File:%s Line:%d Fun:%s] bind Failed sa_family:%d port:%d Ip:%s RetCode:%d ErrorListenFd:%d ErrNo:%d ErrMsg:%s \n",
				__FILE__,__LINE__,__FUNCTION__,
				stServerAddr.sin_family, ntohs(stServerAddr.sin_port),inet_ntoa(stServerAddr.sin_addr),
		        iRetCode,iListenFd,errno,strerror(errno) );
		close(iListenFd);
		return -1;
	}

	//start to listen
	iRetCode = listen(iListenFd, WOOD_LISTEN_BACKLOG);
	if( iRetCode < 0 ){
		printf("[File:%s Line:%d Fun:%s] listen Failed sa_family:%d Port:%d Ip:%s RetCode:%d ErrorListenFd:%d ErrNo:%d ErrMsg:%s \n", 
				__FILE__,__LINE__,__FUNCTION__,
				stServerAddr.sin_family, ntohs(stServerAddr.sin_port),inet_ntoa(stServerAddr.sin_addr),
				iRetCode,iListenFd,errno,strerror(errno) );		
		return -1;		
	}

	printf("[File:%s Line:%d Fun:%s] listening sa_family:%d Ip:%s Port:%d\n", 
			__FILE__,__LINE__,__FUNCTION__,
			stServerAddr.sin_family, inet_ntoa(stServerAddr.sin_addr),ntohs(stServerAddr.sin_port));	

	//start the main logic...
	
	int iClientFd = 0;
	struct sockaddr_in stClientAddr;
	memset(&stClientAddr,0,sizeof(stClientAddr));
	socklen_t lentClient = sizeof(stClientAddr);
	
	while(1){		
		memset(&stClientAddr,0,sizeof(stClientAddr));
		iClientFd = accept( iListenFd,(struct sockaddr *)&stClientAddr,&lentClient);
		if ( iClientFd < 0 ){
			if (errno == ECONNABORTED){
				printf("[File:%s Line:%d Fun:%s] ECONNABORTED accept Failed sa_family:%d port:%d Ip:%s RetCode:%d ErrorListenFd:%d ErrNo:%d ErrMsg:%s \n", 
						__FILE__,__LINE__,__FUNCTION__,
						stServerAddr.sin_family, ntohs(stServerAddr.sin_port),inet_ntoa(stServerAddr.sin_addr),
						iRetCode,iListenFd,errno,strerror(errno) );	
				continue;
			}
			else{
				printf("[File:%s Line:%d Fun:%s] accept Failed sa_family:%d port:%d Ip:%s RetCode:%d ErrorListenFd:%d ErrNo:%d ErrMsg:%s \n", 
						__FILE__,__LINE__,__FUNCTION__,
						stServerAddr.sin_family, ntohs(stServerAddr.sin_port),inet_ntoa(stServerAddr.sin_addr),
						iRetCode,iListenFd,errno,strerror(errno) );	
				continue;
			}
		}
		DoSomthing(iClientFd,&stClientAddr);
	}

	
}
