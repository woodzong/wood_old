<?php
//author: woodzong
//date:   2008-12-07
//desc:   base configuration for wood php

//base code path
define("__BASE_CODE_PATH","C:\\APMServ5.2.6\\www\\htdocs\\woodphp\\");

//check is windows or linux like os
define("__IS_WINDOWS",(DIRECTORY_SEPARATOR=='\\')?1:0);

//smarty template base configuration
define("__TEMPLATE_BASE_PATH",__BASE_CODE_PATH."demo"); 

//log base path
define("__LOG_BASE_PATH",__BASE_CODE_PATH."log"); 

//upfile base path
define("__UP_BASE_PATH",__BASE_CODE_PATH."up"); 

//mysql link configuration
define("__DB_HOST","localhost");
define("__DB_USER","root");
define("__DB_PASSWD","");

//contrl the debug show info is on or not... just for using in developing...
define("__IS_DEBUGING", 1);

//is for bussiness configuration;
define("__COR_NAME_TITLE","wood php tool");
define("__SYS_NAME","php tool");

define("__MAX_READ_FILE_SIZE",1000*1024); // 1M

?>