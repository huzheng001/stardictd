<?php ob_start(); ?>
<html><head><meta http-equiv="content-type" content="text/html; charset=UTF-8"><title>StarDict:: Select and Translate</title>
<link rel="shortcut icon" href="images/stardict.ico" />
<script src="js/search-plugin.js" type="text/javascript"></script>
<body><center>
<?php
session_start();
include 'head.php';
echo "<br><table border=\"1\"><tr><td><b>"._L('How to add StarDict to your firefox search bar?')."</b><br>"._L('Click the next link is OK.')."<br><center><img src=\"files/stardict.png\" /><a href=\"javascript:addEngine('stardict','png','Dictionary','0')\">StarDict.org</a></center></td></tr></table>";
echo "<br><table border=\"1\"><tr><td><b>Install Oak Vocabulary for Firefox.</b><br>Oak Vocabulary let you check up new words from the webpage, including StarDict.org.<br>You can install it from <a href=\"https://addons.mozilla.org/firefox/2830/\" target=\"_blank\">here</a>.<br>You can add StarDict.org to it:<br>Dictionary: <b>StarDict</b><br>Encoding: <b>UTF-8</b><br>Query URL: <b>http://www.stardict.org/mini.php?q=$$$$</b></td></tr></table>";
include 'end.inc.php';
ob_end_flush();
?>
