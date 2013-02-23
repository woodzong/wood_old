/*
Manual FROM POSIX:




Within  a bracket expression, the name of a character class enclosed in
       `[:' and `:]' stands for the list of all characters belonging  to  that
       class.  Standard character class names are:

              alnum       digit       punct
              alpha       graph       space
              blank       lower       upper
              cntrl       print       xdigit
              
              
       isalnum()
              checks for an alphanumeric character; it is equivalent to (isalpha(c) || isdigit(c)).

       isalpha()
              checks  for  an  alphabetic  character;  in  the standard "C" locale, it is equivalent to (isupper(c) ||
              islower(c)).  In some locales, there may be additional characters for which isalpha()  is  true--letters
              which are neither upper case nor lower case.

       isascii()
              checks  whether  c is a 7-bit unsigned char value that fits into the ASCII character set.  This function
              is a BSD extension and is also an SVID extension.

       isblank()
              checks for a blank character; that is, a space or a tab.  This function is a GNU extension.

       iscntrl()
              checks for a control character.

       isdigit()
              checks for a digit (0 through 9).

       isgraph()
              checks for any printable character except space.

       islower()
              checks for a lower-case character.

       isprint()
              checks for any printable character including space.

       ispunct()
              checks for any printable character which is not a space or an alphanumeric character.

       isspace()
              checks for white-space characters.  In the "C" and "POSIX" locales, these are: space, form-feed  ('\f'),
              newline ('\n'), carriage return ('\r'), horizontal tab ('\t'), and vertical tab ('\v').

       isupper()
              checks for an uppercase letter.

       isxdigit()
              checks for a hexadecimal digits, i.e. one of 0 1 2 3 4 5 6 7 8 9 a b c d e f A B C D E F.
              
*/


/*
Author: woodzong@qq.com
Date: 2009-07-05
*/

#ifndef _CWREGEX_WWW_DDEEE1238178648714671234
#define _CWREGEX_WWW_DDEEE1238178648714671234

#include <stdio.h>
#include <sys/types.h>
#include <regex.h>
#include <string>
#include <vector>


namespace wood{

#define CW_REGEX_MAX_SUB_LEN 255

#define CW_REGEX_URL "http[s]{0,1}://[a-zA-Z.0-9-]+(/[a-zA-Z.0-9\\-\\?\\&/=]+)"
#define CW_REGEX_HOST "http[s]{0,1}://([a-zA-Z.0-9-]+)[/]{0,1}"

    class CWRegex{
        public:
            CWRegex(){ 
                m_iRetCode = 0;
                memset(m_sBuffer,0,sizeof(m_sBuffer));
                memset(m_sErrMsg,0,sizeof(m_sErrMsg));
                memset(&m_stRe,0,sizeof(m_stRe));
                m_bIsCompiled = false;                 
            }

            ~CWRegex(){
                if( m_bIsCompiled ){
                    regfree(&m_stRe);
                    m_bIsCompiled = false;
                }
            }

            int Init(const std::string & strRegex,int cflags = REG_EXTENDED);

            int FindRegSubMatchFull( const::string & strRegex, const std::string & strTargetContent, std::vector<std::string> &vecMatches , 
                                    int cRegFlags = REG_EXTENDED, int cExecFlags = 0 );

            int FindRegSubMatchFullGlobalAll( const::string & strRegex, const std::string & strTargetContent, std::vector<std::string> &vecMatches , 
                                    int cRegFlags = REG_EXTENDED, int cExecFlags = 0 );

            int FindRegOneMatchFull( const::string & strRegex, const std::string & strTargetContent,std::string & strMatchedStr,
	                              int cRegFlags = REG_EXTENDED, int cExecFlags = 0);

            int FindRegSubMatch(const std::string & strTargetContent, std::vector<std::string> &vecMatches , int cflags = 0, int * pPosNext = NULL );
            
            

            int GetSubExpressNum() {
                if( m_bIsCompiled ){
                    return m_stRe.re_nsub;
                }
                return -1;
            }

            const std::string & GetRegex(){
                return m_strRegex;
            }

            const char * GetLastErrMsg(){
                return m_sErrMsg;
            }

            int GetLastRetCode(){
                return m_iRetCode;
            }

            

        private:
            int m_iRetCode;
            bool m_bIsCompiled;
            regex_t m_stRe;
            char m_sBuffer[1024];
            char m_sErrMsg[1024];
            std::string m_strRegex;
    };
};

#endif


