<?php ob_start(); ?>
<html><head><meta http-equiv="content-type" content="text/html; charset=UTF-8"><title>StarDict:: News</title>
<link rel="shortcut icon" href="images/stardict.ico" />
</head>
<body><center>
<?php
session_start();
include 'head.php';
echo "<br>"._L("If you want to add advertisement in StarDict website, contact")." <a href=\"mailto:huzheng001@gmail.com\">huzheng001@gmail.com</a>.";
include 'end.inc.php';
ob_end_flush();
?>
