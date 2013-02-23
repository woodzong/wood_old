<?php
define("__WOOD_DEFAULT_BORDER_COLOR",0x3169C6);
define("__WOOD_DEFAULT_FILLED_COLOR",0xC6D3EF);
define("__WOOD_DEFAULT_FONT_COLOR",0x000000);
define("__WOOD_DEFAULT_LINE_COLOR",0x000000);
define("__WOOD_DEFAULT_FONT_TYPE",3);
define("__WOOD_DEFAULT_FONT_WIDTH",7);
define("__WOOD_DEFAULT_FONT_HEIGHT",11);

define("__WOOD_DEFAULT_LEFT_RIGHT_MARGIN",10);
define("__WOOD_DEFAULT_TOP_BOTTOM_MARGIN",6);

$g_iLineColorIndex = 0;

function WCMakeColor($image,$iColor){
	$iRed   = ($iColor&0xFF0000) >> 16;
	$iGreen = ($iColor&0x00FF00) >> 8;
	$iBlue  = ($iColor&0x0000FF) >> 0;
	return imagecolorallocate($image, $iRed, $iGreen, $iBlue);
}


function imagelinethick($image, $x1, $y1, $x2, $y2, $color, $thick = 10)
{
    /* this way it works well only for orthogonal lines
    imagesetthickness($image, $thick);
    return imageline($image, $x1, $y1, $x2, $y2, $color);
    */
    if ($thick == 1) {
        return imageline($image, $x1, $y1, $x2, $y2, $color);
    }
    $t = $thick / 2 - 0.5;
    if ($x1 == $x2 || $y1 == $y2) {
        return imagefilledrectangle($image, round(min($x1, $x2) - $t), round(min($y1, $y2) - $t), round(max($x1, $x2) + $t), round(max($y1, $y2) + $t), $color);
    }
    $k = ($y2 - $y1) / ($x2 - $x1); //y = kx + q
    $a = $t / sqrt(1 + pow($k, 2));
    $points = array(
        round($x1 - (1+$k)*$a), round($y1 + (1-$k)*$a),
        round($x1 - (1-$k)*$a), round($y1 - (1+$k)*$a),
        round($x2 + (1+$k)*$a), round($y2 - (1-$k)*$a),
        round($x2 + (1-$k)*$a), round($y2 + (1+$k)*$a),
    );
    imagefilledpolygon($image, $points, 4, $color);
    return imagepolygon($image, $points, 4, $color);
}


class CObjectXXOImg{
	
	private $m_iX;
	private $m_iY;
	private $m_iWidth;
	private $m_iHeight;
	private $m_iBorderColor;
	private $m_iFilledColor;
	private $m_iFontColor;
	private $m_iFontType;
	
	public function CObjectXXOImg($_strName,$_iProperty, $_iX,$_iY,$_iWidth=0,$_iHeight=0,
								  $_iBorderColor=__WOOD_DEFAULT_BORDER_COLOR,$_iFilledColor=__WOOD_DEFAULT_FILLED_COLOR,
								  $_iFontColor=__WOOD_DEFAULT_FONT_COLOR,$_iFontType=__WOOD_DEFAULT_FONT_TYPE){
		$this->m_strName   = $_strName;
		$this->m_iProperty = $_iProperty;
		$this->m_iX     = $_iX;
		$this->m_iY     = $_iY;
		
		if($_iWidth == 0){
			$this->m_iWidth = $this->CalWidth($_strName);
		}
		else{
			$this->m_iWidth = $_iWidth;
		}
		
		if($_iHeight == 0){
			$this->m_iHeight = $this->CalHeight();
		}
		else{
			$this->m_iHeight = $_iHeight;
		}		
		
		$this->m_iBorderColor   = $_iBorderColor;
		$this->m_iFilledColor   = $_iFilledColor;
		$this->m_iFontColor     = $_iFontColor;
		$this->m_iFontType      = $_iFontType;		
	}
	
	public function GetName(){
		return $this->m_strName;
	}
	
	public function GetCenterPointX(){
		return $this->m_iX + $this->m_iWidth/2;
	}
	
	public function GetCenterPointY_UP(){
		return $this->m_iY;
	}
	
	public function GetCenterPointY_DOWN(){
		return $this->m_iY + $this->m_iHeight;
	}	
	
	public function ShowOut($image){
		$colorFilledColor = WCMakeColor($image, $this->m_iFilledColor);
		$colorBorderColor = WCMakeColor($image, $this->m_iBorderColor);
		$colorFontColor   = WCMakeColor($image, $this->m_iFontColor);
		
		$polygonArray = array ($this->m_iX,   $this->m_iY,
							   $this->m_iX,   $this->m_iY+$this->m_iHeight,
							   $this->m_iX+$this->m_iWidth, $this->m_iY+$this->m_iHeight,
							   $this->m_iX+$this->m_iWidth, $this->m_iY);
		
		imagefilledpolygon($image,$polygonArray,4,$colorFilledColor);
		imagepolygon($image,$polygonArray,4,$colorBorderColor);
		imagestring($image, $this->m_iFontType, $this->m_iX+__WOOD_DEFAULT_LEFT_RIGHT_MARGIN/2, $this->m_iY+__WOOD_DEFAULT_TOP_BOTTOM_MARGIN/2, $this->m_strName, $colorFontColor);

	}
	
	public function GetWidth(){
		return $this->m_iWidth;
	}
	
	public function GetHeight(){
		return $this->m_iHeight;
	}	
	
	private function CalHeight(){
		return __WOOD_DEFAULT_FONT_HEIGHT + __WOOD_DEFAULT_TOP_BOTTOM_MARGIN; // for margin;
	}
	
	private function CalWidth($strName){
		$iLen = strlen($strName);
		return $iLen*__WOOD_DEFAULT_FONT_WIDTH + __WOOD_DEFAULT_LEFT_RIGHT_MARGIN; // for margin;
	}
	
	
}


class CConnector{
	static public function ConnectAToB( $image, $oObjectXXOImg_A, $oObjectXXOImg_B ){
		//echo "GetName:".$oObjectXXOImg_A->GetName()." Color:".$_iLineColor."</br>";
		global $g_iLineColorIndex;
		$g_iLineColorIndex ++ ;
		$iLineColor = ( 0xFFFFFF>>($g_iLineColorIndex%16) );
		echo $g_iLineColorIndex." Color:".$iLineColor."</br>";
		$colorLine = WCMakeColor($image,$iLineColor);
		//imagestring($image,5,100,100,"hello!!!!!!",$colorLine);
		//$colorLine = imagecolorallocate($image,255,255,0); 
		//imagelinethick($image,0,0,500,500,$colorLine);
		imageline($image,$oObjectXXOImg_A->GetCenterPointX(),$oObjectXXOImg_A->GetCenterPointY_DOWN(),
						 $oObjectXXOImg_B->GetCenterPointX(),$oObjectXXOImg_B->GetCenterPointY_UP(),$colorLine);
	}
}
 
?>
