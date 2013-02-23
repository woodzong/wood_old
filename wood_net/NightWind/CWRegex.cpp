#include "CWRegex.h"
#include "CWCommInclude.h"

using namespace wood;

int CWRegex::Init(const std::string & strRegex,int cflags){
	if( m_bIsCompiled ){
		regfree(&m_stRe);
		m_bIsCompiled = false;
	}
	
	CWRegex();

	m_strRegex = strRegex;

	m_iRetCode = regcomp(&m_stRe, m_strRegex.c_str(), cflags);
	if( m_iRetCode ){
		regerror (m_iRetCode, &m_stRe, m_sBuffer, sizeof (m_sBuffer));
		SNPRINTF_ERR(m_iRetCode,m_sErrMsg,sizeof(m_sErrMsg),"regcomp Error Regex:%s ErrMsg:%s",
					 m_strRegex.c_str(),m_sBuffer);
		return m_iRetCode;
	}

	m_bIsCompiled = true;
	return 0;
}

int CWRegex::FindRegSubMatchFull( const::string & strRegex, const std::string & strTargetContent, 
								  std::vector<std::string> &vecMatches , 
								  int cRegFlags, int cExecFlags){
	m_iRetCode = Init(strRegex,cRegFlags);
	if( m_iRetCode != 0 ){
		return m_iRetCode;
	}

	return FindRegSubMatch(strTargetContent,vecMatches,cExecFlags);
}


int CWRegex::FindRegSubMatchFullGlobalAll( const::string & strRegex, const std::string & strTargetContent, std::vector<std::string> &vecMatches , 
                                           int cRegFlags = REG_EXTENDED, int cExecFlags = 0 ){

	bool bIsOneMatched = false;
	m_iRetCode = Init(strRegex,cRegFlags);
	if( m_iRetCode != 0 ){
		return m_iRetCode;
	}

	std::string strTmp = strTargetContent;
	int iMatchedPos = 0;
	do{
		m_iRetCode = FindRegSubMatch(strTmp,vecMatches,cExecFlags,&iMatchedPos);
		if( m_iRetCode != 0 ){
			break;
		}
		bIsOneMatched = true;
		strTmp.erase(0,iMatchedPos);
		iMatchedPos = 0;
	}while(1);

	if(bIsOneMatched){
		return 0;
	}
	return m_iRetCode; // all no matched...
}




int CWRegex::FindRegOneMatchFull( const::string & strRegex, const std::string & strTargetContent,std::string & strMatchedStr,
	                              int cRegFlags, int cExecFlags){
	strMatchedStr = "";
	std::vector<std::string> vecMatches;
	m_iRetCode = Init(strRegex,cRegFlags);
	if( m_iRetCode != 0 ){
		return m_iRetCode;
	}

	m_iRetCode = FindRegSubMatch(strTargetContent,vecMatches,cExecFlags);
	if( m_iRetCode != 0){
		return m_iRetCode;
	}

	std::vector<std::string>::iterator it = vecMatches.begin();
	if( it!=vecMatches.end() ){
		strMatchedStr = *it;
	}
	return 0;
}






int CWRegex::FindRegSubMatch(const std::string & strTargetContent, std::vector<std::string> &vecMatches , int cflags, int * pPosNext ){
	
	regmatch_t stSubs[CW_REGEX_MAX_SUB_LEN];
	memset(&stSubs,0,sizeof(stSubs));
	
	m_iRetCode = regexec (&m_stRe, strTargetContent.c_str(), (size_t)CW_REGEX_MAX_SUB_LEN, stSubs, cflags);  
	if (m_iRetCode == REG_NOMATCH){  
		SNPRINTF_ERR(m_iRetCode,m_sErrMsg,sizeof(m_sErrMsg),"regexec NoMatched Regex:%s",
				     m_strRegex.c_str()); 
		return m_iRetCode;
	}  
	else if (m_iRetCode){
		regerror (m_iRetCode, &m_stRe, m_sBuffer, sizeof (m_sBuffer));
		SNPRINTF_ERR(m_iRetCode,m_sErrMsg,sizeof(m_sErrMsg),"regexec Error Regex:%s ErrMsg:%s ",
					 m_strRegex.c_str(),m_sBuffer);
		return m_iRetCode;
	}


	//matched... find the sub matches...
	//printf("m_stRe.re_nsub:%d \n",m_stRe.re_nsub);
	//
	if( m_stRe.re_nsub <= 0 ){
		m_iRetCode = REG_NOMATCH; 
		SNPRINTF_ERR(m_iRetCode,m_sErrMsg,sizeof(m_sErrMsg),"regexec NoMatched Regex:%s m_stRe.re_nsub:%d ",
					 m_strRegex.c_str(),m_stRe.re_nsub); 
		return m_iRetCode;
	}
	
	for ( unsigned int i = 0; i <= m_stRe.re_nsub; i++){
		if( i==0 ){
			continue;
		}
		std::string strTmp;
		int iLen = stSubs[i].rm_eo - stSubs[i].rm_so; 
		strTmp.assign( strTargetContent.c_str()+stSubs[i].rm_so,iLen );
		if( pPosNext != NULL ){
			*pPosNext = stSubs[i].rm_so+iLen;
		}
		vecMatches.push_back(strTmp);
	}

	return 0;				 
}



