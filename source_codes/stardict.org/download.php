<?php ob_start(); ?>
<html><head><meta http-equiv="content-type" content="text/html; charset=UTF-8"><title>StarDict:: Download</title>
<link rel="shortcut icon" href="images/stardict.ico" />
<link rel="stylesheet" href="css/tree.css">
<script language="javascript" type="text/javascript" src="js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="js/showhide.js"></script>
<script language="javascript" type="text/javascript" src="js/download.js"></script>
</head>
<body topmargin="3" alink="#ff0000" bgcolor="#ffffff" link="#0000cc" marginheight="3" text="#000000" vlink="#551a8b"><center>
<?php
session_start();
include 'head.php';
include_once 'arg.php';
$args = "-ird /";
include_once 'backend.php';
$content = call_stardict_client($args);
echo "<br>Visit <a href=\"http://stardict.sourceforge.net\">StarDict Project Home</a> to download the main program.<br><table width=\"100%\"><tr><td width=\"45%\"><div style='overflow-x:auto; height:380px;'><ul class=\"tree\">";
echo $content;
echo "</ul></div></td><td width=\"5%\"><div id=\"load\" style=\"display:none\"><img src=\"images/loading.gif\"></div></td><td width=\"50%\" valign=\"top\" align=\"right\"><table width=\"100%\"><tr><td>";
echo "<font color=\"red\"><big>Install Dictionaries in Linux:</big></font><br>To install these tarball dictionaries, do this:<br>tar -xjvf a.tar.bz2<br>mv a /usr/share/stardict/dic<br><font color=\"red\"><big>Install Dictionaries in Win32:</big></font><br>Use WinRAR to extract the .tar.bz2 file, which you get a directory, then move this directory to X:\Program files\stardict\dic\<br>";
echo "</td></tr><tr><td><hr><div id=\"dictinfo\"></div></td></tr></table></td></tr></table>";
include 'end.inc.php';
ob_end_flush();
?>
