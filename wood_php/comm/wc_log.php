<?php
define("__WOOD_LOG_ROOL_TYPE_BY_DAY",1);  //1 for one log file per day
define("__WOOD_LOG_ROOL_TYPE_BY_HOUR",2); //2 for one log file per hour
define("__WOOD_LOG_ROOL_TYPE_BY_SELF",3); //3 for log file with bin file's self name...

define("__WOOD_LOG_NEED_ECHO",1);
define("__WOOD_LOG_NONEED_ECHO",0);

//here only 1+3 is use now...

require_once("./comm/wc_base_fun.php");

class CWoodLog{
	private $m_strBasePath;
	private $m_dwRollType; 
	private $m_strLogFileName;
	private $m_strBinName;
	private $m_dwIsNeedEcho;
	
	public function CWoodLog($_strBasePath,$_strLogFileName,$_dwRollType=__WOOD_LOG_ROOL_TYPE_BY_DAY,$_dwIsNeedEcho=__WOOD_LOG_NONEED_ECHO){
		$this->m_strBasePath    = $_strBasePath;
		$this->m_dwRollType     = $_dwRollType;
		$this->m_strLogFileName = $_strLogFileName;
		$this->m_strBinName     = basename($_SERVER['PHP_SELF']);
		$this->m_strBinName     = str_replace(".php","",$this->m_strBinName);
		$this->m_dwIsNeedEcho   = $_dwIsNeedEcho;
	}
	
	public function LogWebErr(){
		$arg_list = func_get_args();
		$m_strContent = call_user_func_array("sprintf",$arg_list);
		$m_strAll     = "[".WC_TimeNow()." ERR ]"
		                ."[".$this->m_strBinName."] "
						."[".$_SERVER['REMOTE_ADDR'].":".$_SERVER['REMOTE_PORT'].":".($_SERVER['REQUEST_TIME']%10000)."] "
						.$m_strContent."\n";
		$this->WriteIntoFile($m_strAll);
	}
	
	public function LogWebPerform(){
		$arg_list = func_get_args();
		$m_strContent = call_user_func_array("sprintf",$arg_list);
		$m_strAll     = "[".WC_TimeNow()." PER ]"
		                ."[".$this->m_strBinName."] "
						."[".$_SERVER['REMOTE_ADDR'].":".$_SERVER['REMOTE_PORT'].":".($_SERVER['REQUEST_TIME']%10000)."] "
						.$m_strContent."\n";
		$this->WriteIntoFile($m_strAll);
	}
	
	public function LogWebDebug(){
		$arg_list = func_get_args();
		$m_strContent = call_user_func_array("sprintf",$arg_list);
		$m_strAll     = "[".WC_TimeNow()." DEG ]"
		                ."[".$this->m_strBinName."] "
						."[".$_SERVER['REMOTE_ADDR'].":".$_SERVER['REMOTE_PORT'].":".($_SERVER['REQUEST_TIME']%10000)."] "
						.$m_strContent."\n";
		$this->WriteIntoFile($m_strAll);
	}				
	
	public function LogWeb(){
		$arg_list = func_get_args();
		$m_strContent = call_user_func_array("sprintf",$arg_list);
		$m_strAll     = "[".WC_TimeNow()." RUN ]"
		                ."[".$this->m_strBinName."] "
						."[".$_SERVER['REMOTE_ADDR'].":".$_SERVER['REMOTE_PORT'].":".($_SERVER['REQUEST_TIME']%10000)."] "
						.$m_strContent."\n";
		$this->WriteIntoFile($m_strAll);
	}
	
	public function Log(){
		$arg_list = func_get_args();
		$m_strContent = call_user_func_array("sprintf",$arg_list);
		$m_strAll     = "[".WC_TimeNow()."]".$m_strContent."\n";
		$this->WriteIntoFile($m_strAll);
	}
	
	private function WriteIntoFile($strContent){

		if($this->m_dwIsNeedEcho){
			echo $strContent."</BR>\n";
		}
	
		$strLogFile = $this->m_strBasePath.DIRECTORY_SEPARATOR
		              .WC_MonthNow().DIRECTORY_SEPARATOR;
					  
		if( $this->m_dwRollType == __WOOD_LOG_ROOL_TYPE_BY_SELF ){
			$strLogFile.=basename($this->m_strBinName);
		}			
		else{
			$strLogFile.=$this->m_strLogFileName;
		}		  
		
		$strLogFile.=".".WC_DateNow().".log";
					  
					  
		$strPath = $this->m_strBasePath.DIRECTORY_SEPARATOR.WC_MonthNow();					  
		
		if(!__IS_WINDOWS){
			$strLogFile = str_replace( "\\", "/", $strLogFile);
			$strPath = str_replace( "\\", "/", $strPath);
		}
		else{
			$strLogFile = str_replace( "/", "\\", $strLogFile);
			$strPath = str_replace( "/", "\\", $strPath);
		}
		
		if( !file_exists( $strPath ) ){			
			WC_MakeDir( $strPath );
		}
		
		$iFh = fopen($strLogFile ,"a+");
		
		fwrite($iFh,$strContent);
	
		fclose($iFh);
	}

}

?>
