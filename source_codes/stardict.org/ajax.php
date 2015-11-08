<?php
ob_start();
session_start();
if (!isset($_SESSION['browser_ie'])) {
	if (false !== strpos (strtolower ($_SERVER['HTTP_USER_AGENT']), 'msie'))
		$_SESSION['browser_ie'] = 1;
	else
		$_SESSION['browser_ie'] = 0;
}
?>
<html>
<head>
<title>StarDict</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="shortcut icon" href="images/stardict.ico" />
<script src="http://www.google-analytics.com/urchin.js" type="text/javascript">
</script>
<script type="text/javascript">
_uacct = "UA-949160-1";
urchinTracker();
</script>
<script language="javascript" type="text/javascript" src="js/x_core.js"></script>
<script language="javascript" type="text/javascript" src="js/x_drag.js"></script>
<script language="javascript" type="text/javascript" src="js/x_event.js"></script>
<script language="javascript" type="text/javascript" src="js/x_StarDictwin.js"></script>
<script language="javascript" type="text/javascript" src="js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="js/lookup.js"></script>
<script language="javascript" type="text/javascript" src="js/showhide.js"></script>
<link rel='stylesheet' type='text/css' href='css/window.css' />
<?php
if ($_SESSION['browser_ie']==1)
	echo "<link rel='stylesheet' type='text/css' href='css/IE/window.css' />";
?>
<link rel='stylesheet' type='text/css' href='css/wordlist.css' />
</head>
<body bgcolor="white" onload="set_focus()">
<center>
<?php
$cookindex = isset($_COOKIE["StarDict_index"])?$_COOKIE["StarDict_index"]:"";
if ($cookindex!="ajax") {
	setcookie("StarDict_index", "ajax", time()+60*60*24*30, '/');
}
include 'head.php';
echo "<font size=\"-1\"><a href=\"web.php\">Web</a>&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"news.php\">"._L("News")."</a>&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"forum/\">"._L("Forum")."</a>&nbsp;&nbsp;&nbsp;&nbsp;<b><a href=\"more.php\">"._L("more")."&nbsp;»</a></b></font><br><br><table width=\"100%\"><tr><td width=\"80%\">";
?>
<script type="text/javascript"><!--
google_ad_client = "pub-7493326633307702";
google_ad_width = 120;
google_ad_height = 600;
google_ad_format = "120x600_as";
google_ad_type = "text_image";
google_ad_channel ="";
google_color_border = "336699";
google_color_bg = "FFFFFF";
google_color_link = "0000FF";
google_color_url = "008000";
google_color_text = "000000";
//--></script>
<script type="text/javascript"
  src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
</script>
<?php
echo "</td><td width=\"20%\" valign=\"top\"><a href=\"preferences.php\">"._L("Preferences")."</a><br><a href=\"dictmanage.php\">"._L("Dict Manage")."</a><br><br><a href=\"http://stardict.sourceforge.net\" target=\"_blank\">"._L("StarDict Project")."</a><br><a href=\"selectandtrans.php\">"._L("Select and Translate")."</a><br><a href=\"browserplugin.php\">"._L("Browser Plug-in")."</a><br><br>";
?>
<script type="text/javascript"><!--
google_ad_client = "pub-7493326633307702";
google_ad_width = 110;
google_ad_height = 32;
google_ad_format = "110x32_as_rimg";
google_cpa_choice = "CAAQga7zzwEaCAqINcPi102eKLPGvnU";
//--></script>
<script type="text/javascript" src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
</script>
<?php
echo "</td></tr></table></center>";
$nom = "StarDict.mainwin";
static $zindex = 99;
++$zindex;
$win_x = isset($_COOKIE["StarDict_win_x"])?$_COOKIE["StarDict_win_x"]:150;
$win_y = isset($_COOKIE["StarDict_win_y"])?$_COOKIE["StarDict_win_y"]:70;
$win_w = isset($_COOKIE["StarDict_win_w"])?$_COOKIE["StarDict_win_w"]:600;
$win_h = isset($_COOKIE["StarDict_win_h"])?$_COOKIE["StarDict_win_h"]:450;
echo "<div id='$nom' class='eyeapp' style='position:absolute; overflow:hidden; width:".$win_w."px; height:".$win_h."px; left:".$win_x."px; top:".$win_y."px; text-align: left; z-index: $zindex;";
echo "overflow:auto;' onMouseDown=' if (topapp != this) { topapp = this; this.style.zIndex = ++maxZ; sendWindowPos (this.id, xLeft (this), xTop (this), xWidth (this), xHeight (this), xZIndex(this)); }'><div id='${nom}DBar' style='position: relative; cursor: move; margin:0px; padding:0px; color:#ccc; height: 21px; text-align: left; '><div class='bsupesq'></div><div class='bsupdre'></div><div id='${nom}DTop' class='bsupcen'><div align='right'>"." <img id='{$nom}MBtn' style='cursor:pointer;' border='0' alt='Maximize window' title='Maximize window' src='images/gfxwin/max.png' />"."<img border='0' title='Close window' alt='Close window' src='images/gfxwin/close.png' style='cursor:pointer;' onclick = 'closeApp (this)' /></div></div><div class='captitol'>"._L("StarDict")."&nbsp; <a href='help.php'><img border='0' title='"._L("Help")."' src='images/btn/help.png' /></a>&nbsp;<a href='dictmanage.php'><img border='0' style='cursor:pointer;' title='"._L("Dict Manage")."' src='images/btn/config.png' /></a></div></div><div class='txt' id='${nom}txt'>";
echo "<div class='actionbar' style='text-align: center;'><div class='barr'></div><div class='barl'></div><span style='float:left; padding-left:5px; padding-top:5px;'><input type=\"button\" onclick=\"clear_entry()\" value=\""._L("Clear")."\" /> <input type=\"text\" autocomplete=off size=35 id=\"entry\" onkeyup=\"on_entry_changed(event)\" /> &nbsp;<a href='#' onClick=\"do_fuzzy_query()\"><img border='0' title='"._L("Fuzzy query")."' src='images/search.png'></a> &nbsp;<a href='#' onClick=\"do_go_back()\"><img border='0' title='"._L("Go back")."' src='images/undo.png'></a> &nbsp;<a href='#' onClick=\"showInfo()\"><img border='0' title='"._L("Show Information")."' src='images/info.png'></a></span><span style='float:right;padding-right:10px; padding-top:5px;'><a href='#' onClick=\"javascript:document.getElementById('main_menu').style.display='block';\"><img border='0' alt='Main Menu' title='"._L("Main Menu")."' src='images/home.png'></a></span></div>";
echo "<div id='main_menu' class='bubble' style='left:420px; top: 40px; width: 120px; height:150px;'><div class='bubbletitle' >"._L("Main Menu")."</div><div><img src='images/preferences.png'> <a href=\"preferences.php\">"._L("Preferences")."</a><br><img src='images/properties.png'> <a href=\"dictmanage.php\">"._L("Dict Manage")."</a><br><img src='images/exec.png'> <a href=\"account.php\">"._L("Account")."</a><br><img src='images/help.png'> <a href=\"help.php\">"._L("Help")."</a><br><img src='images/about.png'> <a href=\"about.php\">"._L("About")."</a></div><div class='bubblecancel'><a href='#' onClick=\"javascript:document.getElementById('main_menu').style.display='none';\"><img border='0' alt='Cancel' title='Cancel' src='images/btn/cancel.png' /></a></div></div>";
echo "<div class='interior'>";
if ($_SESSION['browser_ie']==1)
	include 'lookup-body-ie.php';
else
	include 'lookup-body.php';
echo "</div></div><div class='peu' id='${nom}DDow' ><div class='binfesq'></div><div class='binfdre'></div><div class='binfcen'>";
echo "<div id='${nom}RBtn' align='right' class='botobaix' ></div></div></div></div>";
echo "<script language='javascript' type='text/javascript'> Setup ('$nom'); maxZ = $zindex; </script></body></html>";
ob_end_flush();
?>
