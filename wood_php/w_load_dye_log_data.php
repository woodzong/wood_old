<?php
require_once("./class/Smarty.class.php");
require_once("./comm/wc_comm.php");

set_time_limit(60*60*2); //2 hours...

$G_TARGET_DATE = "2010-01-16";


$G_BASE_DATA_FILE_PATH = __BASE_CODE_PATH."data";

$G_WC_LOG = new CWoodLog(__LOG_BASE_PATH,__SYS_NAME,__WOOD_LOG_ROOL_TYPE_BY_SELF,__WOOD_LOG_NEED_ECHO);


WC_LOG("Base XXO Config File Path:%s",$G_BASE_DATA_FILE_PATH);

function GetInfoFromOneLine($strLineInfo){
	//[2009-12-25 09:33:43] DYE|0x27051801|AO_AUCTION_FIXUP|AO_AUCTION_EVENT|4
	$aInfo = explode("|", $strLineInfo);
	if( count($aInfo) != 5 ){
		return 0;
	}
	//$aInfo: 0:[2009-12-25 09:33:43] DYE 1:0x27051801 2:AO_AUCTION_FIXUP 3:AO_AUCTION_EVENT 4:4
	$strTime = $aInfo[0];
	$strTime = str_replace("[","",$strTime);
	$strTime = str_replace("]","",$strTime);
	$aTime = explode(" ", $strTime);

	$aRet["DATE"]        = $aTime[0];
	$aRet["TIME"]        = $aTime[1];
	$aRet["CMDID"]       = $aInfo[1];
	$aRet["CALLER"]      = $aInfo[2];		
	$aRet["NAME"]        = $aInfo[3];				
	$aRet["COUNT"]       = $aInfo[4];	
	$aRet["KEY"]         = $aRet["CALLER"]."-".$aRet["NAME"]."-".$aRet["CMDID"]."-".$aRet["DATE"];
	return $aRet;
}



function LoadOneXXODyeLog($strPath,$strFileName){
	$bIsAllEmpty = true;
	
	$fp = fopen($strPath.DIRECTORY_SEPARATOR.$strFileName, "r");
	if ( !$fp ) {
		WC_LOG_ERR("could not open ReadOneProcessFile input,File:%s",$strFileName);
		return;	
	}
	
	$iIndex = 0;
	$iRetIndex = 0;
		
	$strFileInSql = str_replace("_pl_key.log","",GetFileNameFromFullPath($strFileName));
	
	$aMergedRet = array();
	
	while (!feof($fp)) {
		$iIndex ++ ;
		$strLine = fgets($fp);
		
		$aRet = array();
		$aRet = GetInfoFromOneLine($strLine);
		if( $aRet == 0){ // error data; continue;
			continue;
		}
		
		$aRet["KEY"] = $aRet["KEY"]."-".$strFileInSql;
		
		//merge arrayinfo...
		if( array_key_exists($aRet["KEY"],$aMergedRet) ){
			$aMergedRet[$aRet["KEY"]]["COUNT"]+=$aRet["COUNT"];
		}
		else{
			$aMergedRet[$aRet["KEY"]] = $aRet;
		}
	}
	fclose($fp);
	//$str = DumpArray($aMergedRet);
	//echo $str;
	
	while ( $aRetInfo = each($aMergedRet) ) {
		global $G_TARGET_DATE;
		if($aRetInfo[1]["DATE"]!=$G_TARGET_DATE){
			continue;
		}
		
		$bIsAllEmpty = false;
		$iRetIndex ++;
		$strInsert = sprintf("INSERT INTO wood_dye.t_dye_detail SET `Fdatetime`='%s',`Fcount`=%s,`Fcaller`='%s',`Fname`='%s',`Fcmdid`=%s,`Ffilename`='%s';",
								 $aRetInfo[1]["DATE"]." ".$aRetInfo[1]["TIME"],
								 $aRetInfo[1]["COUNT"],
								 $aRetInfo[1]["CALLER"],
								 $aRetInfo[1]["NAME"],
								 $aRetInfo[1]["CMDID"],
								 $strFileInSql);
		
		global $G_DB_CONN;		
		mysql_query($strInsert,$G_DB_CONN) or die("Query failed ".$G_DB_CONN.mysql_error()." SQL:".$strInsert); 
	}	
	
	WC_LOG("Load One File Finished:%s Line:%d Inserted:%d",$strFileName,$iIndex,$iRetIndex);
	
	if($bIsAllEmpty){
		WC_LOG_DEBUG("ALL EMPTY LOG:%s",$strFileName);
	}
}


$aFileNameList = GetDirFileName($G_BASE_DATA_FILE_PATH);
foreach( $aFileNameList as $strTargetFileName ){
	if(strstr($strTargetFileName,"_pl_key")){
		//echo $strTargetFileName."</br>";
		LoadOneXXODyeLog($G_BASE_DATA_FILE_PATH,$strTargetFileName);		
	}
}



?>
