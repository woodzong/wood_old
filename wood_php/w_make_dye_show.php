<?php
require_once("./class/Smarty.class.php");
require_once("./comm/wc_comm.php");
require_once("./comm/wc_img.php");

set_time_limit(60*60*2); //2 hours...

$G_WC_LOG = new CWoodLog(__LOG_BASE_PATH,__SYS_NAME,__WOOD_LOG_ROOL_TYPE_BY_SELF,__WOOD_LOG_NEED_ECHO);


$G_TARGET_DATE = "2010-01-16";
$G_TARGET_DATE_END = "2010-01-17";

$G_BASE_DATA_FILE_PATH = __BASE_CODE_PATH."data";


WC_LOG("Base XXO Config File Path:%s",$G_BASE_DATA_FILE_PATH);


//SELECT COUNT(1) FROM t_dye_detail WHERE Ffilename!='back_netio' AND Ffilename!='netio' AND Fdatetime>'2010-01-16' AND Fdatetime<'2010-01-17';
//SELECT DISTINCT `Fname` FROM t_dye_detail WHERE Ffilename!='back_netio' AND Ffilename!='netio' AND Fdatetime>'2010-01-16' AND Fdatetime<'2010-01-17';


//find distinct becalled name list
$aResInfoList = array();
$strSelectSQL = sprintf("SELECT DISTINCT `Fname` FROM wood_dye.t_dye_detail WHERE Ffilename!='back_netio' AND Ffilename!='netio' AND Fdatetime>'%s' AND Fdatetime<'%s' ORDER BY Fname;",
						$G_TARGET_DATE,$G_TARGET_DATE_END );
$resList = mysql_query($strSelectSQL,$G_DB_CONN) or die("Query failed ".$G_DB_CONN.mysql_error()." SQL:".$strSelectSQL); 
while($aRes = mysql_fetch_array($resList)){
	array_push($aResInfoList,$aRes["Fname"]);
}
$aBeCalledNameList = $aResInfoList;
//WC_LOG_DEBUG("CurCount:%d SQL:%s",count($aResInfoList),$strSelectSQL);



//find distinct caller name list
$aResInfoList = array();
$strSelectSQL = sprintf("SELECT DISTINCT `Fcaller` FROM wood_dye.t_dye_detail WHERE Ffilename!='back_netio' AND Ffilename!='netio' AND Fdatetime>'%s' AND Fdatetime<'%s' ORDER BY Fcaller;",
						$G_TARGET_DATE,$G_TARGET_DATE_END );
$resList = mysql_query($strSelectSQL,$G_DB_CONN) or die("Query failed ".$G_DB_CONN.mysql_error()." SQL:".$strSelectSQL); 
while($aRes = mysql_fetch_array($resList)){
	array_push($aResInfoList,$aRes["Fcaller"]);
}
$aCallerNameList = $aResInfoList;
//WC_LOG_DEBUG("CurCount:%d SQL:%s",count($aResInfoList),$strSelectSQL);




//find the relation from caller to becalled;
$aAllNameInfoList = array();
foreach( $aBeCalledNameList as $strBeCalledName ){
	$aResInfoList = array();
	$strSelectSQL = sprintf("SELECT DISTINCT `Fcaller` FROM wood_dye.t_dye_detail WHERE Ffilename!='back_netio' AND Ffilename!='netio' AND Fdatetime>'%s' AND Fdatetime<'%s' AND Fname='%s' ORDER BY Fcaller;",
							$G_TARGET_DATE,$G_TARGET_DATE_END,$strBeCalledName );
	//WC_LOG_DEBUG("SelectSQL:%s",$strSelectSQL);	 
	
	$resList = mysql_query($strSelectSQL,$G_DB_CONN) or die("Query failed ".$G_DB_CONN.mysql_error()." SQL:".$strSelectSQL); 
	while($aRes = mysql_fetch_array($resList)){
		array_push($aResInfoList,$aRes["Fcaller"]);
	}
	//WC_LOG_DEBUG("CurCount:%d SQL:%s",count($aResInfoList),$strSelectSQL);
	if(count($aResInfoList)){
		$aAllNameInfoList[$strBeCalledName] = $aResInfoList;
	}
}

//find the base xxo; which is only becalled and not a caller;
$aBaseNameList = array();
foreach( $aBeCalledNameList as $strBeCalledName ){
	$bIsBeCalled = false;
	foreach( $aCallerNameList as $strCallerName ){
		if( $strBeCalledName == $strCallerName ){
			$bIsBeCalled = true;
			break;
		}
	}
	if( $bIsBeCalled == false ){
		array_push($aBaseNameList,$strBeCalledName);
	}
}


WC_LOG_DEBUG("CallerNum:%d BeCalledNum:%d BaseNum:%d RelationNum:%d",count($aCallerNameList),count($aBeCalledNameList),count($aBaseNameList),count($aAllNameInfoList)  );


//$im = imagecreatefrompng($G_BASE_DATA_FILE_PATH."\\001.png");
//$im = imagecreate(100, 300);
//$bg = imagecolorallocate($im, 255, 255, 255);
//$lineColor = imagecolorallocate($im,0,0,0); 
//imageline($im,0,0,50,50,$lineColor);
//imagestring($im, 5, 0, 0, "Hello world!", $lineColor);
// create image
$G_WIDTH = 1920;
$image = imagecreatetruecolor($G_WIDTH,1024);
imagecolorallocate($image, 255, 255, 255);


$aImgBaseNameList = array();
$iX  = 10;
$iY  = 700;
foreach( $aBaseNameList as $strBaseName){
	$oImgTmp = new CObjectXXOImg($strBaseName,0,$iX,$iY);	
	
	if($iX > $G_WIDTH || ($iX + 250) > $G_WIDTH){
		$iX = 10;
		$iY += 30;
	}
	else{
		$iX += ($oImgTmp->GetWidth()+20);
	}
	
	$aImgBaseNameList[$strBaseName] = $oImgTmp;
	$oImgTmp->ShowOut($image);
}








$iX  = 10;
$iY  = 100;
foreach( $aCallerNameList as $strCallerName){
	$oImgTmp = new CObjectXXOImg($strCallerName,0,$iX,$iY);	
	
	if($iX > $G_WIDTH || ($iX + 250) > $G_WIDTH){
		$iX = 10;
		$iY += 30;
	}
	else{
		$iX += ($oImgTmp->GetWidth()+20);
	}
	$aImgBaseNameList[$strCallerName] = $oImgTmp;
	
	$oImgTmp->ShowOut($image);
}




function FindImageObject($aList,$strName){
	if( array_key_exists($strName,$aList) ){
		return $aList[$strName];
	}
	return 0;
}


$aCalledName = array_keys($aAllNameInfoList);
//WC_LOG("CalledName:%s",DumpArray($aCalledName));
foreach( $aCalledName as $strBeCalledName ){	
	$oImgBeCalled = FindImageObject($aImgBaseNameList,$strBeCalledName);	
	if(  $oImgBeCalled != 0 ){		
		$aCaller = array();
		$aCaller = $aAllNameInfoList[$strBeCalledName];	
		foreach( $aCaller as $strCallerName ){
			$oImgCaller = FindImageObject($aImgBaseNameList,$strCallerName);
			//WC_LOG("strCallerName:%s :%d",$strCallerName,$oImgCaller);
			if( $oImgCaller != 0 ){				
				//echo gettype($oImgBeCalled);
				//WC_LOG("Caller:%s BeCalled:%s",$oImgCaller->GetName(),$oImgBeCalled->GetName());
				CConnector::ConnectAToB($image,$oImgCaller,$oImgBeCalled);
			}
		}
	}
}


imagepng($image,$G_BASE_DATA_FILE_PATH."\\002.png"); //把刚才创建的图像保存为aaa.png图片文件
imagedestroy($image); //销毁图像handel   


?>
