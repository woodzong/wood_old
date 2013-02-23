#include <stdio.h>
#include <string.h>

#include "CWCommInclude.h"
#include "CWZipOp.h"

using namespace wood;


/* file operation...
FILE * fp = fopen("/tmp/zlib.tmp","w");
if(fp == NULL){
	printf("Err In Fopen!!\n");
	return 1;
}
if(fwrite(pTmp,1,iBodyLen,fp) != iBodyLen){
	printf("Err In Fwrite!!\n");
	return 2;
}
fclose(fp); 				

char pchData[10240]; 
gzFile fData = gzopen("/tmp/zlib.tmp","rb"); 
int ret = gzread(fData,pchData,10240); 
gzclose(fData);
printf("Ret:%d Out:%s inlen:%d\n",ret,pchData,iBodyLen);
*/


int CWZipOp::InFlateHttpGZip(unsigned char * pInContent, int iInContentSize, 
  						     unsigned char * pOutBuffer, int iOutBufferSize, 
  						     char * sErrMsg, int iErrMsgSize){

	bool bIsWithGZipHead = false;	
	const int iGZipHeadLen = 10; // magic num;
	int iRetCode = 0;

	//printf("======================iInContentSize:%d\n",iInContentSize);

	memset(sErrMsg,0,iErrMsgSize);

	if( iInContentSize <= iGZipHeadLen 
		|| iOutBufferSize < (iInContentSize - iGZipHeadLen)
	   	|| pInContent == NULL || pOutBuffer == NULL 
	   	|| pInContent == pOutBuffer){
		SNPRINTF_ERR(-1,sErrMsg,iErrMsgSize,"ContentSizeError InContentSize:%d OutBufferSize:%d InContent:%p OutBuffer:%p",
				iInContentSize,iOutBufferSize,pInContent,pOutBuffer);
		return -1;
	}

	/* gzip file format...
	+---+---+---+---+---+---+---+---+---+---+========//========+===========//==========+---+---+---+---+---+---+---+---+
	|ID1|ID2| CM|FLG|     MTIME     |XFL| OS|   额外的头字段   |       压缩的数据      |     CRC32     |     ISIZE     |
	+---+---+---+---+---+---+---+---+---+---+========//========+===========//==========+---+---+---+---+---+---+---+---+	
	ID1与ID2：各1字节。固定值，ID1 = 31 (0x1F)，ID2 = 139(0x8B)，指示GZIP格式。
	CM：1字节。压缩方法。目前只有一种：CM = 8，指示DEFLATE方法。
	*/

	if( (*pInContent) == 31 && (*(pInContent+1)) == 139  &&  (*(pInContent+2)) == 8 ){
		bIsWithGZipHead = true;
	}
	else{
		//not support yet...
		SNPRINTF_ERR(-2,sErrMsg,iErrMsgSize,"Content Is Not GZip InContentSize:%d OutBufferSize:%d InContent:%p OutBuffer:%p",
					  iInContentSize,iOutBufferSize,pInContent,pOutBuffer);
		return -2;
	}

	
	z_stream strm;	
	strm.zalloc = Z_NULL;
	strm.zfree 	= Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in 	= iInContentSize-iGZipHeadLen;
	strm.next_in 	= pInContent+iGZipHeadLen;
	strm.avail_out  = iOutBufferSize;
    strm.next_out   = pOutBuffer;	
	iRetCode = inflateInit2(&strm,-MAX_WBITS);
	if( iRetCode != Z_OK ){
	    SNPRINTF_ERR(iRetCode,sErrMsg,iErrMsgSize,"inflateInit2 Failed InContentSize:%d OutBufferSize:%d InContent:%p OutBuffer:%p",
	    			iInContentSize,iOutBufferSize,pInContent,pOutBuffer);
		return iRetCode;
	}

    iRetCode = inflate(&strm, Z_SYNC_FLUSH);
    if( iRetCode == Z_DATA_ERROR || iRetCode == Z_MEM_ERROR || iRetCode == Z_STREAM_ERROR || iRetCode == Z_NEED_DICT){
    	SNPRINTF_ERR(iRetCode,sErrMsg,iErrMsgSize,"inflate Failed InContentSize:%d OutBufferSize:%d InContent:%p OutBuffer:%p",
    				 iInContentSize,iOutBufferSize,pInContent,pOutBuffer);
		(void)deflateEnd(&strm);	
		return iRetCode;
    }

	(void)deflateEnd(&strm);
	return 0;
}

