<?php ob_start(); ?>
<html><head><meta http-equiv="content-type" content="text/html; charset=UTF-8"><title>StarDict:: News</title>
<link rel="shortcut icon" href="images/stardict.ico" />
</head>
<body><center>
<?php
session_start();
include 'head.php';
?>
<br>
Thank you for your payment. Your transaction has been completed, and a receipt for your purchase has been emailed to you.<br>We will upgrade your account in three days. Any problem, please mail to <a href="mailto:huzheng001@gmail.com">huzheng001@gmail.com</a>.
<?php
include 'end.inc.php';
ob_end_flush();
?>
