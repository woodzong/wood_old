<?php	
	function WC_TimeNow(){
		return date("Y-m-d H:i:s", time());
	}
	
	function WC_SecNow(){
		return date("U", time());
	}
	
	function WC_DateNow(){
		return date("Ymd", time());
	}	
	
	function WC_MonthNow(){
		return date("Ym", time());
	}
	
	function WC_MakeDir( $dir, $mode = "0777" ){
		if( !$dir ){
			return 0;
		}			
		
		$mdir = "";
		
		foreach( explode( DIRECTORY_SEPARATOR, $dir ) as $val ) {
		
			$mdir .= $val.DIRECTORY_SEPARATOR;
			if( $val == ".." || $val == "." )
				continue;
			
			if( ! file_exists( $mdir ) ) {
				if(!@mkdir( $mdir, $mode )){
						echo "创建目录[".$mdir."]失败.";
						exit;
				}
			}
		}
		return true;
	}	
	
	function GetDirFileName($dir){ 
		$aFileName = array();
		if(is_dir($dir)){ 
			$dh = opendir($dir);
			if ( $dh ) { 
				while (($file= readdir($dh)) !== false){ 
					if((is_dir($dir.DIRECTORY_SEPARATOR.$file)) && $file!="." && $file!=".."){ 
						//echo "<b><font color='red'>文件名：</font></b>",$file,"<br><hr>"; 
						//listDir($dir."/".$file."/"); 
						// ignore dir file
						//continue;
						foreach( GetDirFileName($dir.DIRECTORY_SEPARATOR.$file) as $strFileName ){
							array_push($aFileName,$file.DIRECTORY_SEPARATOR.$strFileName);
						}
						//$aFileName = array_merge( $aFileName, GetDirFileName($dir.DIRECTORY_SEPARATOR.$file) );
						//echo "DIR:".$file."</br>";
					}else{ 
						if($file!="." && $file!=".."){ 
							array_push($aFileName,$file);
						} 
					} 
				} 
				closedir($dh); 
			} 
			return $aFileName;
		} 
	} 	
?>