//#include "zlib.h" // for gzip decode....
#include "CWCommInclude.h"
#include "CWLoadOneHttp.h"
#include "CWRegex.h"
#include <set>

using namespace std;
using namespace wood;



int main(int argc, char ** argv){

	//start to set the server address
	
	
	          
	//CWHttpReq oReq(80,"121.14.88.14", "http://www.baidu.com","");
	//CWHttpReq oReq(80,"124.42.86.135", "product.dangdang.com","/product.aspx?product_id=20610769","");
	//CWHttpReq oReq(80,"192.168.0.1", "192.168.0.1", "/index2.html","");
	//CWHttpReq oReq(80,"121.14.91.142","auction1.paipai.com" ,"/FFF3620A00000000007F385504B2A1FC","");

	/*
	CWHttpReq oReq(80,"203.81.17.195", "http://www.amazon.cn/mn/browseApp?nodeid=51080",
		          "Cookie: session-id-time=1247958000l; session-id=475-8470161-1159534; ubid-acbcn=477-6855675-9539217; session-token=KmJS6VyoG5FQoFrZYSFLJUj22QTS1vd6lOJYrtDH8rMyYJfEWC6+KyBfVXlpEni/i6GYLap4OHc90nv1G3cAba+7ZIYhlB++t+Ja1NFBmpopUhone/pKCmtEWrpUaCYhi9xOu7SWXCdPkUu8wsmafJGp7yM9D7PejnHJU5d2J6pvIReTrDdh0kfXvYByKpiQSq+RYTGTCNg=");
*/

	std::set<std::string> setProductIds;

	for(int iPageIndex = 1; iPageIndex<=100; iPageIndex++){
		char sUrl[1024] = {0};
		snprintf(sUrl,sizeof(sUrl),"http://www.amazon.cn/mn/browseApp?nodeid=51080&sortType=market&pageNow=%d",iPageIndex);
		CWHttpReq oReq(80,"203.81.17.195", sUrl,"");
		CWLoadOneHttp oWLoadOneHttp;
		CWHttpResp oWHttpResp;
		int iRetCode = oWLoadOneHttp.Load(oReq, oWHttpResp);
		if( iRetCode != 0 ){
			CW_LOG_ERR(iRetCode,"oWLoadOneHttp.Load Failed:%s",oWHttpResp.GetLastErrMsg());
			return iRetCode;
		}
		//printf("%s",oWHttpResp.GetBodyInfo().c_str());

		if(oWHttpResp.GetBodyInfo().empty()!=true){
			CWRegex oWRegex;
			std::vector<std::string> vecPorductIds;
			iRetCode = oWRegex.FindRegSubMatchFullGlobalAll("href=\"http://www.amazon.cn/mn/[[:graph:]]+prodid=([[:alnum:]]+)",
													oWHttpResp.GetBodyInfo().c_str(),vecPorductIds);
			if( iRetCode != 0 ){
				CW_LOG_DEBUG("%s \n",oWRegex.GetLastErrMsg());
			}
			CW_LOG_DEBUG("vecPorductIds Size:%d",vecPorductIds.size());

			std::vector<std::string>::iterator it    = vecPorductIds.begin();
			std::vector<std::string>::iterator itEnd = vecPorductIds.end();
			for(;it!=itEnd; ++it){
				setProductIds.insert(*it);
			}			
			//CW_LOG("Content:%s",oWHttpResp.GetBodyInfo().c_str());
		}
		CW_LOG_DEBUG("=======================================================setProductIds Size:%d",setProductIds.size());
	}

	
	

	/*
	char sBuffer[1024];
	SNPRINTF_DEBUG(sBuffer, sizeof(sBuffer), "hahahah:%d",122);
	std::cout<<sBuffer<<std::endl;

	SNPRINTF_ERR(100,sBuffer, sizeof(sBuffer), "hahahah:%d",122);
	std::cout<<sBuffer<<std::endl;	
*/

	/*
	long long llStart = GetCurrTimeUs();

	int iTmp = 1;
	for(;iTmp>0; iTmp--){
		int iConnectedFd = Connect(oReq);
		SendAndRead(iConnectedFd, oReq);
		close(iConnectedFd);
	}

	long long llUsed = GetCurrTimeUs()-llStart;

	printf("UsedTime: %lld ms %lld s \n",llUsed/1000,llUsed/1000000);
*/
	/*

	int iReadedBytes = 0;
	char sReadBuffer[255] = {0};
	while( (iReadedBytes = read(iConnectedFd,sReadBuffer,sizeof(sReadBuffer)-1))>0 ){
		printf("%s \n",sReadBuffer);
	}
	*/

	
	
	return 0;
}

