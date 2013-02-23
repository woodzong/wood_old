<?php
require_once("./comm/wc_config.php");
require_once("./class/Smarty.class.php");
require_once("./comm/wc_log.php");

$G_DB_CONN= mysql_pconnect(__DB_HOST,__DB_USER,__DB_PASSWD)or die(mysql_error());

//$G_WC_LOG = new CWoodLog(__LOG_BASE_PATH,__SYS_NAME,__WOOD_LOG_ROOL_TYPE_BY_SELF);

function COPY_ROW(&$arrayTmp,$row,$aIndex){
    foreach($aIndex as $index){
        $arrayTmp[$index] = $row[$index];
    }
}

function DumpArray($aTarget){
	$strTmp = "";
	reset($aTarget);
	while ( $aValue = each($aTarget) ) {
		$strTmp.=" ".$aValue[0].":".$aValue[1];
		if( gettype( $aValue[1]) == "array" ){
			$strTmp.=" In ".$aValue[0]." => ";
			$strTmp.= DumpArray($aValue[1]);
		}
	}
	reset($aTarget);
	return $strTmp;	
}


function DEBUG(){
	if(__IS_DEBUGING){
		$arg_list = func_get_args();
		$m_strContent = call_user_func_array("sprintf",$arg_list);
		echo "<div> ". $m_strContent ."<div> </br>";
	}
}

function WC_LOG(){
	global $G_WC_LOG;
	$arg_list = func_get_args();	 
	call_user_func_array(array(&$G_WC_LOG, "LogWeb"),$arg_list);
}

function WC_LOG_ERR(){
	global $G_WC_LOG;
	$arg_list = func_get_args();
	call_user_func_array(array(&$G_WC_LOG, "LogWebErr"),$arg_list);
}

function WC_LOG_PER(){
	global $G_WC_LOG;
	$arg_list = func_get_args();
	call_user_func_array(array(&$G_WC_LOG, "LogWebPerform"),$arg_list);
}

function WC_LOG_DEBUG(){
	global $G_WC_LOG;
	$arg_list = func_get_args();
	call_user_func_array(array(&$G_WC_LOG, "LogWebDebug"),$arg_list);
}

function HTML_STR($str){
	return htmlentities($str,ENT_COMPAT,GB2312);
}


function LogReqDetail(){
	global $G_WC_LOG;
	
	//$_SERVER["REQUEST_METHOD"]
	
	$strGetReqTmp="GET:".DumpArray($_GET);
	WC_LOG_DEBUG("%s",$strGetReqTmp);
	
	$strPostReqTmp="POST:".DumpArray($_POST);
	WC_LOG_DEBUG("%s",$strPostReqTmp);
	
	$strCookieReqTmp="COOKIE:".DumpArray($_COOKIE);
	WC_LOG_DEBUG("%s",$strCookieReqTmp);
	
	$strFileReqTmp="FILE:".DumpArray($_FILES);
	WC_LOG_DEBUG("%s",$strFileReqTmp);			

	/*
	$strENVReqTmp="ENV:".DumpArray($_ENV);
	$G_WC_LOG->LogWeb("%s",$strENVReqTmp);	
	*/
	
}

function ReplaceMoreSpace($sData){
	return preg_replace("/[\s]+/",' ', $sData);	
}


function ReplaceChnWords($sData){
	$low=chr(0xa1);    
  	$high=chr(0xff); 
    return preg_replace("/[$low-$high]/",'', $sData);	
}

function FixBadXmlWords($sData){
	$sData2 = str_replace(">M",">",$sData);
	$sData2 = str_replace("<xml","<?xml",$sData2);
	return $sData2;
}

function GetIpFromString($strContent){
	if (preg_match("/(\d+.\d+.\d+.\d+)/", $strContent, $result)) {
		return $result[1];
	}
	return "0.0.0.0";
}

function GetFileNameFromFullPath($strFullPath){
	if(__IS_WINDOWS){
		if (preg_match("/\\\\?([^\\\\]*)$/", $strFullPath, $result)){
			return $result[1];
		}
	}
	else{
		if (preg_match("/\/?([^\/]*)$/", $strFullPath, $result)){
			return $result[1];
		}		
	}
	return "";
}


$G_SM_TPL = new Smarty();
$G_SM_TPL->template_dir = __TEMPLATE_BASE_PATH . "/templates/";
$G_SM_TPL->compile_dir = __TEMPLATE_BASE_PATH . "/templates_c/";
$G_SM_TPL->config_dir = __TEMPLATE_BASE_PATH . "/configs/";
$G_SM_TPL->cache_dir = __TEMPLATE_BASE_PATH . "/cache/";
$G_SM_TPL->left_delimiter = '{{';
$G_SM_TPL->right_delimiter = '}}';

?>