<?php ob_start(); ?>
<html><head><meta http-equiv="content-type" content="text/html; charset=UTF-8"><title>StarDict</title>
<link rel="shortcut icon" href="images/stardict.ico" />
<script src="http://www.google-analytics.com/urchin.js" type="text/javascript">
</script>
<script type="text/javascript">
_uacct = "UA-949160-1";
urchinTracker();
</script>
<script>
<!--
function sf(){document.f.q.focus();}
// -->
</script></head>
<body onload="sf()" topmargin="3" alink="#ff0000" bgcolor="#ffffff" link="#0000cc" marginheight="3" text="#000000" vlink="#551a8b"><center>
<?php
$cookindex = isset($_COOKIE["StarDict_index"])?$_COOKIE["StarDict_index"]:"";
if ($cookindex!="web") {
	setcookie("StarDict_index", "web", time()+60*60*24*30, '/');
}
session_start();
include 'head.php';
?>
<img src="images/logo.gif" alt="StarDict" height="110" width="286">

<object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=6,0,29,0" width="1024" height="100"><param name="movie" value="./stardict_zh_CN.swf"><br><param name="quality" value="high"><embed src="./stardict_zh_CN.swf" quality="high" pluginspage="http://www.macromedia.com/go/getflashplayer" type="application/x-shockwave-flash" width="1024" height="100"></embed></object>

<br><br><br>
<form action="query.php" name="f">
<?php
	echo "<table border=\"0\" cellpadding=\"4\" cellspacing=\"0\"><tbody><tr><td nowrap=\"nowrap\"><font size=\"-1\"><b>Web</b>&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"ajax.php\">Ajax</a>&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"news.php\">"._L("News")."</a>&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"forum/\">"._L("Forum")."</a>&nbsp;&nbsp;&nbsp;&nbsp;<b><a href=\"more.php\">"._L("more")."&nbsp;»</a></b></font></td></tr></tbody></table>
<table cellpadding=\"0\" cellspacing=\"0\"><tbody><tr><td width=\"25%\">&nbsp;</td><td align=\"center\"><input maxlength=\"256\" size=\"55\" name=\"q\" value=\"\" title=\""._L("Query")."\"><br><input value=\""._L("StarDict Query")."\" type=\"submit\"></td><td nowrap=\"nowrap\" valign=\"top\" width=\"25%\"><font size=\"-1\">&nbsp;&nbsp;<a href=\"preferences.php\">"._L("Preferences")."</a><br>&nbsp;&nbsp;<a href=\"dictmanage.php\">"._L("Dict Manage")."</a></font></td></tr></tbody></table>
</form><br><a href=\"http://stardictd.sourceforge.net\" target=\"_blank\">"._L("StarDictd Project")."</a>&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"selectandtrans.php\">"._L("Select and Translate")."</a>&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"browserplugin.php\">"._L("Browser Plug-in")."</a><br><br>Sponsor: <a href=\"http://www.minigui.com\" target=\"_blank\">www.MiniGUI.com</a>";
include 'end.inc.php';
ob_end_flush();
?>
