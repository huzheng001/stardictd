<?php ob_start(); ?>
<html><head><meta http-equiv="content-type" content="text/html; charset=UTF-8"><title>StarDict:: News</title>
<link rel="shortcut icon" href="images/stardict.ico" />
</head>
<body><center>
<?php
session_start();
include 'head.php';
echo "<br>"._L("This website is maintained by these people:")."<br><br>
<table border=\"1\"><tr><td><b>Hu Zheng</b></td><td><a href=\"mailto:huzheng001@gmail.com\">huzheng001@gmail.com</a></td><td><a href=\"http://www.huzheng.org\" target=\"_blank\">http://www.huzheng.org</a></td></tr></table>";
include 'end.inc.php';
ob_end_flush();
?>
