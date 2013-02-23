<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<title>{{$TITLE}}--{{$CUR_PAGE}}</title>
<script language="javascript">
function CheckSexOption(iSex){
	if(iSex == 1){
		document.getElementById("h_sex_man").checked = true;
	}
	else if(iSex == 2){
		document.getElementById("h_sex_man").checked = true;
	}
}

function ShowPhotoPreview(sId,oFile){
	var oImgPreview = document.getElementById(sId);
	if(oImgPreview){
		oImgPreview.src = oFile.value;
	}
}
</script>