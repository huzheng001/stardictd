<?php ob_start(); ?>
<html><head><meta http-equiv="content-type" content="text/html; charset=UTF-8"><title>StarDict:: News</title>
<link rel="shortcut icon" href="images/stardict.ico" />
</head>
<body><center>
<?php
session_start();
include 'head.php';
?>
<br></center>
StarDict.org frequently asked questions!<br>
----------------------------------------
<p>
Q) What's the use of collate function?<br>
A) Collate function make the word list be sorted in special language, not just in English. When a word is not found, StarDict will show the first following word, if you enabled collate function, the result will be better, such as input "Francais" will find "Français".
</p>
<p>
Q) How can I upgrade my account level?<br>
A) You can upgrade your account level in <a href="finance.php">Finance Center</a> by pay some money. We can upgrade your account level too if you do some contribution to StarDict, such as <a href="helptranslate.php">help us translate</a> the website, help us add more dictionaries, join StarDict development, and so on.<br>Each way can help StarDict develop further :)
</p>
<center>
<?php
include 'end.inc.php';
ob_end_flush();
?>
