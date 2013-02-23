<?php
require_once("./class/Smarty.class.php");
require_once("./comm/wc_comm.php");
require_once("./comm/wc_img_v2.php");

set_time_limit(60*60*2); //2 hours...

$G_WC_LOG = new CWoodLog(__LOG_BASE_PATH,__SYS_NAME,__WOOD_LOG_ROOL_TYPE_BY_SELF,__WOOD_LOG_NONEED_ECHO);
//$G_WC_LOG = new CWoodLog(__LOG_BASE_PATH,__SYS_NAME,__WOOD_LOG_ROOL_TYPE_BY_SELF,__WOOD_LOG_NEED_ECHO);


$G_TARGET_DATE = "2010-01-16";
$G_TARGET_DATE_END = "2010-01-17";
//$G_TARGET_NAME = "AO_ITEM";
$G_TARGET_NAME = "";

$G_BASE_DATA_FILE_PATH = __BASE_CODE_PATH."data";

WC_LOG("Base XXO Config File Path:%s",$G_BASE_DATA_FILE_PATH);

//SELECT COUNT(1) FROM t_dye_detail WHERE Ffilename!='back_netio' AND Ffilename!='netio' AND Fdatetime>'2010-01-16' AND Fdatetime<'2010-01-17';
//SELECT DISTINCT `Fname` FROM t_dye_detail WHERE Ffilename!='back_netio' AND Ffilename!='netio' AND Fdatetime>'2010-01-16' AND Fdatetime<'2010-01-17';

$strWhereCondition = "";
if($G_TARGET_NAME){
	$strWhereCondition = sprintf(" AND ( Fcaller='%s' OR Fcaller='%s' ) ",$G_TARGET_NAME, $G_TARGET_NAME);
}

//find distinct becalled name list
$g_aAllInfoList = array();
$strSelectSQL = sprintf("SELECT Fcaller,Fname,SUM(Fcount) AS FTotalCallNum, CONCAT(Fname,'-',Fcaller) AS TMP_GROUP,COUNT(1) AS FNUM FROM wood_dye.t_dye_detail  WHERE Ffilename!='back_netio' AND Ffilename!='netio' AND Fdatetime>'%s' AND Fdatetime<'%s' %s GROUP BY TMP_GROUP ORDER BY FNUM ASC;",
						$G_TARGET_DATE,$G_TARGET_DATE_END,$strWhereCondition );
$resList = mysql_query($strSelectSQL,$G_DB_CONN) or die("Query failed ".$G_DB_CONN.mysql_error()." SQL:".$strSelectSQL); 
while($aRes = mysql_fetch_array($resList)){
	$aTmp = array();
	$aTmp["CALLER"] = $aRes["Fcaller"];
	$aTmp["SERVER"] = $aRes["Fname"];
	$aTmp["CALL_TIMES"] = $aRes["FTotalCallNum"];
	array_push($g_aAllInfoList,$aTmp);
}
//WC_LOG_DEBUG("g_aAllInfoList:%s",DumpArray($g_aAllInfoList));


//make all level...
$g_aList_Level_All = array();
foreach( $g_aAllInfoList as $aTmpRelationOuter ){
	  $strTmpName = $aTmpRelationOuter["CALLER"];
	  if( in_array($strTmpName,$g_aList_Level_All) == false ){
		  array_push($g_aList_Level_All,$strTmpName);
	  }
	  $strTmpName = $aTmpRelationOuter["SERVER"];
	  if( in_array($strTmpName,$g_aList_Level_All) == false ){
		  array_push($g_aList_Level_All,$strTmpName);
	  }	  
}
WC_LOG_DEBUG("g_aList_Level_All:%s",DumpArray($g_aList_Level_All));


//find Level 0 xxo
$g_aList_Level_0 = array();
foreach( $g_aAllInfoList as $aTmpRelationOuter ){
	$bIsPureServer = true;
	foreach( $g_aAllInfoList as $aTmpRelationInner ){
		if( $aTmpRelationInner["CALLER"] == $aTmpRelationOuter["SERVER"] ){
			//this "SERVER" Is not in level 0
			$bIsPureServer = false;
			break;
		}
	}
	if($bIsPureServer){
		$strTmpName = $aTmpRelationOuter["SERVER"];
		if( in_array($strTmpName,$g_aList_Level_0) == false ){
			array_push($g_aList_Level_0,$strTmpName);
		}
	}
}
WC_LOG_DEBUG("g_aList_Level_0:%s",DumpArray($g_aList_Level_0));


//find level 1 xxo
$g_aList_Level_1 = array();
foreach( $g_aAllInfoList as $aTmpRelationOuter ){
	  $strTmpName = $aTmpRelationOuter["CALLER"];
	  if( in_array($strTmpName,$g_aList_Level_1) == false ){
		  array_push($g_aList_Level_1,$strTmpName);
	  }
}
WC_LOG_DEBUG("g_aList_Level_1:%s",DumpArray($g_aList_Level_1));


//find level 2 xxo
$g_aList_Level_2 = array();
foreach( $g_aList_Level_1 as $strTmpNameLevel1 ){
	foreach( $g_aAllInfoList as $aTmpRelationOuter ){
		$strTmpName = $aTmpRelationOuter["CALLER"];
		if( $strTmpNameLevel1 == $aTmpRelationOuter["SERVER"]){
			if( in_array($strTmpName,$g_aList_Level_2) == false ){
			  array_push($g_aList_Level_2,$strTmpName);
			}
		}	  
	}		
}
WC_LOG_DEBUG("g_aList_Level_2:%s",DumpArray($g_aList_Level_2));


//find level 3 xxo
$g_aList_Level_3 = array();
foreach( $g_aList_Level_2 as $strTmpNameLevel2 ){
	foreach( $g_aAllInfoList as $aTmpRelationOuter ){
		$strTmpName = $aTmpRelationOuter["CALLER"];
		if( $strTmpNameLevel2 == $aTmpRelationOuter["SERVER"]){
			if( in_array($strTmpName,$g_aList_Level_3) == false ){
			  array_push($g_aList_Level_3,$strTmpName);
			}
		}	  
	}		
}
WC_LOG_DEBUG("g_aList_Level_3:%s",DumpArray($g_aList_Level_3));


//find level 4 xxo
$g_aList_Level_4 = array();
foreach( $g_aList_Level_3 as $strTmpNameLevel3 ){
	foreach( $g_aAllInfoList as $aTmpRelationOuter ){
		$strTmpName = $aTmpRelationOuter["CALLER"];
		if( $strTmpNameLevel3 == $aTmpRelationOuter["SERVER"]){
			if( in_array($strTmpName,$g_aList_Level_4) == false ){
			  array_push($g_aList_Level_4,$strTmpName);
			}
		}	  
	}		
}
WC_LOG_DEBUG("g_aList_Level_4:%s",DumpArray($g_aList_Level_4));


//find level 5 xxo
$g_aList_Level_5 = array();
foreach( $g_aList_Level_4 as $strTmpNameLevel4 ){
	foreach( $g_aAllInfoList as $aTmpRelationOuter ){
		$strTmpName = $aTmpRelationOuter["CALLER"];
		if( $strTmpNameLevel4 == $aTmpRelationOuter["SERVER"]){
			if( in_array($strTmpName,$g_aList_Level_5) == false ){
			  array_push($g_aList_Level_5,$strTmpName);
			}
		}	  
	}		
}
WC_LOG_DEBUG("g_aList_Level_5:%s",DumpArray($g_aList_Level_5));

function ArrayMinus($aA,$aB){ //$aA - $aB
	$i = 0; 
	for( $i=0;$i<count($aB);$i++ ){
	  for( $j=0;$j<count($aA);$j++ ){
		if( $aB[$i] == $aA[$j] ){
			array_splice($aA,$j,1);
		}
	  }	
	}
	return $aA;
}

WC_LOG_DEBUG("g_aList_Level_0:%s",DumpArray($g_aList_Level_0));

$g_aList_Level_1 = ArrayMinus($g_aList_Level_1,$g_aList_Level_2);
WC_LOG_DEBUG("g_aList_Level_1:%s",DumpArray($g_aList_Level_1));

$g_aList_Level_2 = ArrayMinus($g_aList_Level_2,$g_aList_Level_3);
WC_LOG_DEBUG("g_aList_Level_2:%s",DumpArray($g_aList_Level_2));

$g_aList_Level_3 = ArrayMinus($g_aList_Level_3,$g_aList_Level_4);
WC_LOG_DEBUG("g_aList_Level_3:%s",DumpArray($g_aList_Level_3));

$g_aList_Level_4 = ArrayMinus($g_aList_Level_4,$g_aList_Level_5);
WC_LOG_DEBUG("g_aList_Level_4:%s",DumpArray($g_aList_Level_4));


$g_aList_Level_All = array();
if( count($g_aList_Level_0) != 0){
	array_push($g_aList_Level_All,$g_aList_Level_0);
}
if( count($g_aList_Level_1) != 0){
	array_push($g_aList_Level_All,$g_aList_Level_1);
}
if( count($g_aList_Level_2) != 0){
	array_push($g_aList_Level_All,$g_aList_Level_2);
}
if( count($g_aList_Level_3) != 0){
	array_push($g_aList_Level_All,$g_aList_Level_3);
}
if( count($g_aList_Level_4) != 0){
	array_push($g_aList_Level_All,$g_aList_Level_4);
}
if( count($g_aList_Level_5) != 0){
	array_push($g_aList_Level_All,$g_aList_Level_5);
}




header('Content-type: image/png');
$G_WIDTH  = 1920*3;
$G_HEIGHT = 1024;
$G_HORIZONTAL_MARGIN = 10;
$G_VERTICAL_MARGIN   = 30;
$image = imagecreatetruecolor($G_WIDTH,$G_HEIGHT);

//draw ground
$colorBack = imagecolorallocate($image, 255, 255,255);
imagefill($image, 0, 0, $colorBack);
imagecolordeallocate($image,$colorBack);

$colorFont = imagecolorallocate($image, 255, 0,0);
imagestring($image, 2, 20, $G_HEIGHT-30, "XXO Relation Map. Gen Date: ".$G_TARGET_DATE, $colorFont);
imagecolordeallocate($image,$colorFont);

$g_iLevelNum = count($g_aList_Level_All);
$g_aImgListAll = array();

$iYOffSet = $G_HEIGHT/$g_iLevelNum;


/* 左侧对齐的算法
for( $iLevel=0; $iLevel<count($g_aList_Level_All); $iLevel++){
	$iX = $G_HORIZONTAL_MARGIN;
	$iY = $G_VERTICAL_MARGIN;
	foreach( $g_aList_Level_All[$iLevel] as $strNameXXO ){		
		$oImgTmp = new CObjectXXOImg($strNameXXO,0,$iX,$iYOffSet*( $g_iLevelNum - $iLevel - 1) + $iY);
		$g_aImgListAll[$strNameXXO] = $oImgTmp;
		$iX += $oImgTmp->GetWidth() + $G_HORIZONTAL_MARGIN;
		if( $iX + 250 > $G_WIDTH ){
			$iY += $oImgTmp->GetHeight() + $G_VERTICAL_MARGIN;
			$iX = $G_HORIZONTAL_MARGIN;
		}		
	}
}
*/
// 中间对齐的算法
for( $iLevel=0; $iLevel<count($g_aList_Level_All); $iLevel++){
	$iX = $G_HORIZONTAL_MARGIN;
	$iY = $G_VERTICAL_MARGIN;
	foreach( $g_aList_Level_All[$iLevel] as $strNameXXO ){		
		$oImgTmp = new CObjectXXOImg($strNameXXO,0,$iX,$iYOffSet*( $g_iLevelNum - $iLevel - 1) + $iY);
		$g_aImgListAll[$strNameXXO] = $oImgTmp;
		$iX += $oImgTmp->GetWidth() + $G_HORIZONTAL_MARGIN;
		if( $iX + 250 > $G_WIDTH ){
			$iY += $oImgTmp->GetHeight() + $G_VERTICAL_MARGIN;
			$iX = $G_HORIZONTAL_MARGIN;			
		}
	}
	
	$iXNeedMoved = ($G_WIDTH - ($iX + 250)) / 2;
	
	foreach( $g_aList_Level_All[$iLevel] as $strNameXXO ){	
		$g_aImgListAll[$strNameXXO] -> RelocationOffSetX( $iXNeedMoved ,0 );
	}
	
	
}


function FindImageObject($aList,$strName){
	if( array_key_exists($strName,$aList) ){
		return $aList[$strName];
	}
	return 0;
}

function ShowImgList( $image,$aList ){
	reset($aList);
	while ( list($strName, $oImg) = each($aList) ) {
		$oImg->ShowOut($image);
	}	
}


foreach( $g_aAllInfoList as $aTmpRelationOuter ){
	$strServer = $aTmpRelationOuter["SERVER"];
	$strCaller = $aTmpRelationOuter["CALLER"];
	
	$oImgCaller = FindImageObject($g_aImgListAll, $strCaller);
	$oImgServer = FindImageObject($g_aImgListAll, $strServer);
	
	if( $oImgCaller != 0 && $oImgServer != 0 ){
		CConnector::ConnectAToB($image,$oImgCaller,$oImgServer);
	}
}

ShowImgList($image,$g_aImgListAll);

imagepng($image);
imagedestroy($image);










?>
