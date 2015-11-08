<?php ob_start(); ?>
<html><head><meta http-equiv="content-type" content="text/html; charset=UTF-8"><title>StarDict:: Business</title>
<link rel="shortcut icon" href="images/stardict.ico" />
</head>
<body><center>
<?php
session_start();
include 'head.php';
?>
<br>
www.stardict.org use stardictd as the server back end.<br>We will only provide stardictd to advance user.<br>You can go to <a href="finance.php">Finance</a> page to upgrade your account first, then contact us, if you want to get a copy of it.
<?php
include 'end.inc.php';
ob_end_flush();
?>
