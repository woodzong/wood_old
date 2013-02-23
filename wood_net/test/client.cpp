#include <string.h>
#include <errno.h>
#include <stdio.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>


inline int SafeAtoi(const char * sNumber){
    if(sNumber == NULL){
        return 0;
    }
    return atoi(sNumber);
}

int main(int argc, char ** argv){

	//start to check param
	if( argc!=3 ){
		printf("Param Error, Usage: client IP PORT \n");
		printf("Example: client 211.66.6.86 53412 \n");
		return -1;
	}

	//start to set the server address
	int iRetCode = 0;
	
	struct sockaddr_in stServerAddr;
	memset(&stServerAddr,0,sizeof(stServerAddr));	
	
	char sServerIp[25]={0};
	snprintf(sServerIp,sizeof(sServerIp),"%s",argv[1]);
	
	int iPort = SafeAtoi(argv[2]);
	stServerAddr.sin_port  = htons(iPort);
	stServerAddr.sin_family = AF_INET;
	
	iRetCode = inet_pton(AF_INET, sServerIp, &(stServerAddr.sin_addr));
	if(iRetCode<0){
		printf("[File:%s Line:%d Fun:%s] IP:%s Format Error!! \n",__FILE__,__LINE__,__FUNCTION__,sServerIp);
		return -1;
	}
	
	printf("[File:%s Line:%d Fun:%s] Connecting Ip:%s Port:%d \n",
			__FILE__,__LINE__,__FUNCTION__,inet_ntoa(stServerAddr.sin_addr), iPort);


	//start to do somthing for the socket ...
	int iConnectedFd = 0;	
	
	// start to prepare the socket
	// SOCK_STREAM or SOCK_DGRAM (TCP or UDP sockets)
	iConnectedFd = socket(AF_INET, SOCK_STREAM, 0);  
	if ( iConnectedFd < 0 ) {
		printf("[File:%s Line:%d Fun:%s] socket IP:%s Port:%d  Failed ErrorListenFd:%d ErrNo:%d ErrMsg:%s \n",
				__FILE__,__LINE__,__FUNCTION__,
				sServerIp,iPort, iConnectedFd,errno,strerror(errno) );
		return -1;
	}

	iRetCode = connect(iConnectedFd,(struct sockaddr *)&stServerAddr, sizeof(stServerAddr) );
	if(	iRetCode < 0 ){
		printf("[File:%s Line:%d Fun:%s] connect IP:%s Port:%d  Failed ErrorListenFd:%d ErrNo:%d ErrMsg:%s \n",
				__FILE__,__LINE__,__FUNCTION__,
				sServerIp,iPort, iConnectedFd,errno,strerror(errno) );
		return -1;		
	}
	
	printf("[File:%s Line:%d Fun:%s] ConnectedIp:%s Port:%d \n",__FILE__,__LINE__,__FUNCTION__,sServerIp, iPort);

	int iReadedBytes = 0;
	char sReadBuffer[255] = {0};
	while( (iReadedBytes = read(iConnectedFd,sReadBuffer,sizeof(sReadBuffer)-1))>0 ){
		printf("%s \n",sReadBuffer);
	}

	close(iConnectedFd);
	
	return 0;
}

