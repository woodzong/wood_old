#if 0
#include <stdio.h>
#include <sys/types.h>
#include <regex.h>
#include <string>

/* 取子串的函数 */ 
static char* substr(const char*str, unsigned start, unsigned end) 
{ 
	unsigned n = end - start; 
	static char stbuf[256]; 
	strncpy(stbuf, str + start, n); 
	stbuf[n] = 0; 
	return stbuf; 
} 
/* 主程序 */ 
int main(int argc, char** argv) 
{ 
	char * pattern; 
	int x, z, lno = 0, cflags = 0; 
	char ebuf[128], lbuf[256]; 
	regex_t reg; 
	regmatch_t pm[10]; 
	const size_t nmatch = 10; 
	/* 编译正则表达式*/ 
	pattern = argv[1]; 
	z = regcomp(&reg, pattern, cflags); 
	if (z != 0){ 
		regerror(z, &reg, ebuf, sizeof(ebuf)); 
		fprintf(stderr, "%s: pattern '%s' \n", ebuf, pattern); 
		return 1; 
	} 
	/*  逐行处理输入的数据 */ 
	while(fgets(lbuf, sizeof(lbuf), stdin)) { 
		++lno; 
		if ((z = strlen(lbuf)) > 0 && lbuf[z-1] == '\n') 
			lbuf[z - 1] = 0; 
		/* 对每一行应用正则表达式进行匹配 */ 
		z = regexec(&reg, lbuf, nmatch, pm, 0); 
		if (z == REG_NOMATCH) continue; 
		else if (z != 0) { 
			regerror(z, &reg, ebuf, sizeof(ebuf)); 
			fprintf(stderr, "%s: regcom('%s')\n", ebuf, lbuf); 
			return 2; 
		} 
		/* 输出处理结果 */ 
		for (x = 0; x < nmatch && pm[x].rm_so != -1; ++ x) { 
			if (!x) printf("%04d: %s\n", lno, lbuf); 
			printf("  $%d='%s'\n", x, substr(lbuf, pm[x].rm_so, pm[x].rm_eo)); 
		} 
	} 
	/* 释放正则表达式  */ 
	regfree(&reg); 
	return 0; 
} 



#include <stdio.h>
#include <sys/types.h>
#include <regex.h>
#include <string>

#define SUBSLEN 10  
#define EBUFLEN 128 /* error buffer length */  
#define BUFLEN 1024 /* matched buffer length */  

int  
main (int argc, char **argv)  
{  
	FILE *fp;  
	size_t len; /* store error message length */  
	regex_t re; /* store compilned regular expression */  
	regmatch_t subs[SUBSLEN]; /* store matched string position */  
	char matched[BUFLEN]; /* store matched strings */  
	char errbuf[EBUFLEN]; /* store error message */  
	int err, i;  

//	char string[] = "AAAAabaaababAbAbCdCd123123 11(123){12}";  
//	char pattern[] = "(\\([0-9]+\\))(\\{[0-9]+\\}{1})$";  

	char string[]  = "Content-Length: 43894";  
	char pattern[] = "Content-Length: ([0-9]+)"; 


	printf ("String : %s\n", string);  
	printf ("Pattern: \"%s\"\n", pattern);  

	/* compile regular expression */  
	err = regcomp (&re, pattern, REG_EXTENDED);  

	if (err)  
	{  
		len = regerror (err, &re, errbuf, sizeof (errbuf));  
		fprintf (stderr, "error: regcomp: %s\n", errbuf);  
		exit (1);  
	}  
	printf ("Total has subexpression: %d\n", re.re_nsub);  

	/* execute pattern match */  
	err = regexec (&re, string, (size_t)SUBSLEN, subs, 0);  

	if (err == REG_NOMATCH)  
	{  
		fprintf (stderr, "Sorry, no match ...\n");  
		regfree (&re);  
		exit (0);  
	}  
	else if (err)  
	{  
		len = regerror (err, &re, errbuf, sizeof (errbuf));  
		fprintf (stderr, "error: regexec: %s\n", errbuf);  
		exit (1);  
	}  

	/* if no REG_NOMATCH and no error, then pattern matched */  
	printf ("\nOK, has matched ...\n\n");  
	for (i = 0; i <= re.re_nsub; i++)  
	{  
		if (i == 0)  
		{  
			printf ("begin: %d, end: %d, ", subs[i].rm_so, subs[i].rm_eo);  
		}  
		else  
		{  
			printf ("subexpression %d begin: %d, end: %d, ", i, subs[i].rm_so, subs[i].rm_eo);  
		}  
		len = subs[i].rm_eo - subs[i].rm_so;  
		memcpy (matched, string + subs[i].rm_so, len);  
		matched[len] = '\0';  
		printf ("match: %s\n", matched);  
	}  

	regfree(&re);  
	exit(0);  
}


#include <unistd.h>
#include <sys/time.h>

#include "CWRegex.h"

using namespace wood;
using namespace std;

//char string[]  = "Content-Length: 43894";  
//char pattern[] = "Content-Length: ([0-9]+)"; 


long long GetCurrTimeUs(){
	struct timeval stTime;
	gettimeofday(&stTime,NULL);		
	long long tmpTime = stTime.tv_sec * 1000000;
	return tmpTime + stTime.tv_usec;
}



void Test1(){

	std::vector<std::string> vecResult;

	CWRegex oWRegex;
	int iRetCode = oWRegex.Init("Content-Length: ([0-9]+)");
	if( iRetCode != 0 ){
		printf("%s \n",oWRegex.GetLastErrMsg());
		return;
	}

	iRetCode = oWRegex.FindRegSubMatch("Content-Length: 43894",vecResult);
	if( iRetCode != 0 ){
		printf("%s \n",oWRegex.GetLastErrMsg());
		return;
	}	

	std::vector<std::string>::const_iterator it    = vecResult.begin();
	std::vector<std::string>::const_iterator itEnd = vecResult.end();	
	for(;it!=itEnd;++it){
		printf("Machthed:%s\n",it->c_str());
	}
}



void Test2(){

	std::vector<std::string> vecResult;

	CWRegex oWRegex;
	int iRetCode = oWRegex.FindRegSubMatchFull("Content-Length: ([0-9]+)","Content-Length: 43894", vecResult);
	if( iRetCode != 0 ){
		printf("%s \n",oWRegex.GetLastErrMsg());
		return;
	}
	
	std::vector<std::string>::const_iterator it    = vecResult.begin();
	std::vector<std::string>::const_iterator itEnd = vecResult.end();	
	for(;it!=itEnd;++it){
		printf("Machthed:%s\n",it->c_str());
	}
}


void TestLeakMemOne(CWRegex & oWRegex){
	std::vector<std::string> vecResult;
	int iRetCode = oWRegex.FindRegSubMatchFull("Content-Length: ([0-9]+)","Content-Length: 43894", vecResult);
	if( iRetCode != 0 ){
		printf("%s \n",oWRegex.GetLastErrMsg());
		return;
	}

	std::vector<std::string>::const_iterator it    = vecResult.begin();
	std::vector<std::string>::const_iterator itEnd = vecResult.end();	
	for(;it!=itEnd;++it){
		printf("Machthed:%s\n",it->c_str());
	}
}


void TestGetHostAndURL(const std::string & strURLFull, std::string & strHost, std::string & strURL){
    CWRegex oWRegex;
    strHost = "";
    strURL  = "";
	int iRetCode = oWRegex.FindRegOneMatchFull("http[s]{0,1}://[a-zA-Z.0-9-]+(/[a-zA-Z.0-9\\-\\?\\&/=]+)",strURLFull.c_str(),strURL);
	if( iRetCode != 0 ){
		printf("%s \n",oWRegex.GetLastErrMsg());
	}

    iRetCode = oWRegex.FindRegOneMatchFull("http[s]{0,1}://([a-zA-Z.0-9-]+)/*",strURLFull.c_str(),strHost);
	if( iRetCode != 0 ){
		printf("%s \n",oWRegex.GetLastErrMsg());
	}           
}



int main (int argc, char **argv){

	//Test1();

	//Test2();

	/*
	std::string strUrl;
	std::string strHost;
	TestGetHostAndURL("http://www.amazon.cn/mn/detailApp?&prodid=zjbk050159",strHost,strUrl);
	std::cout<<"URL:"<<strUrl<<std::endl;
	std::cout<<"HOST:"<<strHost<<std::endl;	


	TestGetHostAndURL("http://www.baidu.cn",strHost,strUrl);
	std::cout<<"URL:"<<strUrl<<std::endl;
	std::cout<<"HOST:"<<strHost<<std::endl;	
*/

	CWRegex oWRegex;
	std::string strProductId;
	int iRetCode = oWRegex.FindRegOneMatchFull("href=\"http://www.amazon.cn/mn/[[:graph:]]+prodid=([[:alnum:]]+)",
					"href=\"http://www.amazon.cn/mn/detailApp/479-0290071-3759446?ref=BR_TS&uid=479-0290071-3759446&prodid=bkbk955974",strProductId);
	if( iRetCode != 0 ){
		printf("%s \n",oWRegex.GetLastErrMsg());
	}
	std::cout<<"strProductId:"<<strProductId<<std::endl;	
	

	/*
	long long llStart = GetCurrTimeUs();
	CWRegex oWRegex;
	int i=0 ;
	for( i =0 ;i<10000000; i++ ){
		if( i%10000 == 0 ){
			printf("%d\n",i);
		}
		TestLeakMemOne(oWRegex);
	}
	long long llUsed = GetCurrTimeUs()-llStart;

	printf("Count:%d UsedTime: %lld ms %lld s \n",i,llUsed/1000,llUsed/1000000);
*/
	return 0;
}

#endif



/* regex_test.c
* regular expression test in GNU C
*
* tested on redhat6.1
* gcc regex_test.c -o regex_test
*/
#include <stdio.h>
#include <sys/types.h>
#include <regex.h>
#define SUBSLEN 10
#define EBUFLEN 128 /* error buffer length */
#define BUFLEN 1024 /* matched buffer length */
int
main (int argc, char **argv)
{
	FILE *fp;
	size_t len; /* store error message length */
	regex_t re; /* store compilned regular expression */
	regmatch_t subs[SUBSLEN]; /* store matched string position */
	char matched[BUFLEN]; /* store matched strings */
	char errbuf[EBUFLEN]; /* store error message */
	int err, i;
	char string[] = "AAAAabaaababAbAbCdCd123123 11(123){12}(22){33}";
	char pattern[] = "(\\([0-9]+\\))(\\{[0-9]+\\}{1})";
	printf ("String : %s\n", string);
	printf ("Pattern: %s\n", pattern);
	/* compile regular expression */
	err = regcomp (&re, pattern, REG_EXTENDED);
	if (err)
	{
		len = regerror (err, &re, errbuf, sizeof (errbuf));
		fprintf (stderr, "error: regcomp: %s\n", errbuf);
		exit (1);
	}
	printf ("Total has subexpression: %d\n", re.re_nsub);
	/* execute pattern match */
	err = regexec (&re, string, (size_t)SUBSLEN, subs, 0);
	if (err == REG_NOMATCH)
	{
		fprintf (stderr, "Sorry, no match ...\n");
		regfree (&re);
		exit (0);
	}
	else if (err)
	{
		len = regerror (err, &re, errbuf, sizeof (errbuf));
		fprintf (stderr, "error: regexec: %s\n", errbuf);
		exit (1);
	}
	/* if no REG_NOMATCH and no error, then pattern matched */
	printf ("\nOK, has matched ...\n\n");
	for (i = 0; i <= re.re_nsub; i++)
	{
		if (i == 0)
		{
			printf ("begin: %d, end: %d, ",
				subs[i].rm_so, subs[i].rm_eo);
		}
		else
		{
			printf ("subexpression %d begin: %d, end: %d, ",
				i, subs[i].rm_so, subs[i].rm_eo);
		}
		len = subs[i].rm_eo - subs[i].rm_so;
		memcpy (matched, string + subs[i].rm_so, len);
		matched[len] = '\0';
		printf ("match: %s\n", matched);
	}
	regfree(&re);
	exit(0);
} 



