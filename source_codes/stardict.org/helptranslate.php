<?php ob_start(); ?>
<html><head><meta http-equiv="content-type" content="text/html; charset=UTF-8"><title>StarDict:: Help us to translate</title>
<link rel="shortcut icon" href="images/stardict.ico" />
</head>
<body><center>
<?php
session_start();
include 'head.php';
echo "<br>You can download this <a href=\"lang.zh_CN.inc\">file</a>, it is in UTF-8 encoding.<br>Replace the Chinese strings with your translation, then send the modified file to <a href=\"mailto:huzheng001@gmail.com\">huzheng001@gmail.com</a>.<br>You can tell me your StarDict account at the same time, so I will upgrade your account to level 1 user for your work!";
include 'end.inc.php';
ob_end_flush();
?>
