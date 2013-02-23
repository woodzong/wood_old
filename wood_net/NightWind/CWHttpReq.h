/*
Author: woodzong@qq.com
Date: 2009-07-12
*/

#ifndef _CWHTTPREQ_WWW_DDEEE1238178648714671234
#define _CWHTTPREQ_WWW_DDEEE1238178648714671234


#include <stdio.h>
#include <string>
#include <stdint.h>
#include "CWRegex.h"

namespace wood{


class CWHttpReq{
	public:
        CWHttpReq(){
            CWHttpReq(0,"0.0.0.0","http://localhost","cookie:");
        };
        
		CWHttpReq( uint32_t dwPort,const std::string & strIP, 
				  const std::string & strFullURL,const std::string & strLocalCookie ):
		m_dwPort(dwPort),
		m_strIP(strIP),
		m_strLocalCookie(strLocalCookie)
		{   
		    GetHostAndURL(strFullURL, m_strHost, m_strURL);
			m_strAllContent = "";
			m_strAllContent+="GET "+GetURL()+" HTTP/1.1\r\n";
			m_strAllContent+="Accept: */* \r\n";
			m_strAllContent+="Referer: http://"+ GetHost() +"\r\n";
			m_strAllContent+="Accept-Language: zh-cn \r\n";
			m_strAllContent+="Accept-Encoding: gzip, deflate\r\n";
			m_strAllContent+="User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727;)\r\n";
			m_strAllContent+="Host: "+GetHost()+"\r\n";
			m_strAllContent+="Connection: Keep-Alive\r\n";
			m_strAllContent+=strLocalCookie+"\r\n";
			m_strAllContent+="\r\n";
		}


        CWHttpReq& operator = (const CWHttpReq& oWHttpReq){
            this->m_dwPort = oWHttpReq.GetReqPort();
            this->m_strIP = oWHttpReq.GetReqIP();
            this->m_strHost = oWHttpReq.GetHost();
            this->m_strURL = oWHttpReq.GetURL();
            this->m_strLocalCookie = oWHttpReq.GetLocalCookie();
            this->m_strAllContent  = oWHttpReq.GetReqAllContent();           
        	return *this;
        }

        const std::string & GetLocalCookie() const{
            return m_strLocalCookie;
        }
        

		const std::string & GetReqAllContent() const{
			return m_strAllContent;
		}
		
		
		int GetReqPort() const{ 
			return m_dwPort; 
		};
		
		const std::string & GetReqIP() const{ 
			return m_strIP;
		};

		const std::string & GetHost() const{ 
			return m_strHost;
		};

		const std::string & GetURL() const{ 
			return m_strURL; 
		};	

    private:
        void GetHostAndURL(const std::string & strURLFull, std::string & strHost, std::string & strURL){
            CWRegex oWRegex;
        	int iRetCode = oWRegex.FindRegOneMatchFull(CW_REGEX_URL,strURLFull.c_str(),strURL);
        	if( iRetCode != 0 ){
        		printf("%s \n",oWRegex.GetLastErrMsg());                
                if(strURL.empty()){
                    strURL = "/";
                }
        	}

            iRetCode = oWRegex.FindRegOneMatchFull(CW_REGEX_HOST,strURLFull.c_str(),strHost);
        	if( iRetCode != 0 ){
        		printf("%s \n",oWRegex.GetLastErrMsg());
        	}
        }

	private:
		uint32_t m_dwPort;
		std::string m_strIP;
		std::string m_strHost;
		std::string m_strURL;
		std::string m_strLocalCookie;
		std::string m_strAllContent;
};

};

#endif


