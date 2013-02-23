/*
Author: woodzong@qq.com
Date: 2009-07-12
*/

#ifndef _CWTOOLS_WWW_DDEEE1238178648714671234
#define _CWTOOLS_WWW_DDEEE1238178648714671234

#include <stdlib.h>
#include <sys/time.h>
#include "CWCommInclude.h"

namespace wood{

class CWTools{
	public:
        
    static int SafeAtoi(const char * sNumber){
        if(sNumber == NULL){
            return 0;
        }
        return atoi(sNumber);
    }
        
    static const char * strstrBuffer(const char * pContentBuffer,  int iContentSize,  const char * pDividerBuffer, int iDividerSize ){
    	const char * pTarget = 0;
    	const char * pTmp    = pContentBuffer;
    	int i = 0;

    	for( i=0; i<iContentSize; i++){
    		pTmp=pContentBuffer+i;
    		const char * pDivder = pDividerBuffer;
    		while( *pDivder == *pTmp 
    				&& pTmp< (pContentBuffer+iContentSize)  //keep it not overflow..
    				&& pDivder< ( pDividerBuffer+iDividerSize ) ) // keep it not overflow...
    		{
    			pDivder ++;
    			pTmp ++;

    			if( pDivder == pDividerBuffer+iDividerSize ){ // get it, break;
    				pTarget = pContentBuffer+i;
    				break;
    			}			
    		}	

    		if(pTarget){
    			break;
    		}		
    	}

    	return pTarget;	
    }


    static long long GetCurrTimeUs(){
    	struct timeval stTime;
    	gettimeofday(&stTime,NULL);		
    	long long tmpTime = stTime.tv_sec * 1000000;
    	return tmpTime + stTime.tv_usec;
    }

};

};

#endif


