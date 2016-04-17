<?php ob_start(); ?>
<html><head><meta http-equiv="content-type" content="text/html; charset=UTF-8"><title>StarDict:: Dict Manage</title>
<link rel="shortcut icon" href="images/stardict.ico" />
<link rel="stylesheet" href="css/tree.css">
<script language="javascript" type="text/javascript" src="js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="js/showhide.js"></script>
<script language="javascript" type="text/javascript" src="js/dictmanage.js"></script>
</head>
<body topmargin="3" alink="#ff0000" bgcolor="#ffffff" link="#0000cc" marginheight="3" text="#000000" vlink="#551a8b"><center>
<?php
session_start();
include 'head.php';
$username = isset($_SESSION['username'])?$_SESSION['username']:"";
$password = isset($_SESSION['password'])?$_SESSION['password']:"";
if ($username!="" && $password!="") {
	include_once 'arg.php';
	$eusername = encode_command_arg($username);
	$epassword = encode_command_arg($password);
	$edir = encode_command_arg("/");
	$args = "-ar ".$eusername." ".$epassword." +ir ".$edir." +gm -gd";
	include_once 'backend.php';
	$content = call_stardict_client($args);
	$result = explode("", $content);
	echo "<table width=\"100%\"><tr><td width=\"45%\"><div style='overflow-x:auto; height:420px;'><ul class=\"tree\">";
	echo $result[0];
	echo "</ul></div></td><td width=\"5%\"><div id=\"load\" style=\"display:none\"><img src=\"images/loading.gif\"></div></td><td width=\"50%\" valign=\"top\" align=\"right\"><table width=\"100%\"><tr><td><table><tr><td><div>";
	printf(_L('You can choose <b><span id="maxdictcount">%s</span></b> dictionaries.'), $result[1]);
	echo "</div><select id=\"dictlist\" size=\"8\">";
	echo $result[2];
	echo "</select></td><td><input type=\"button\" value=\""._L("Show Info")."\" onClick=\"showDictInfo2()\" /><br><input type=\"button\" value=\""._L("Move Up")."\" onClick=\"moveDictUp()\" /><br><input type=\"button\" value=\""._L("Move Down")."\" onClick=\"moveDictDown()\" /><br><input type=\"button\" value=\""._L("Delete")."\" onClick=\"deleteDict()\" /></td><td width=\"50\"></td><td><form action=\"setdictmask.php\" method=\"post\"><input id=\"setdictmask\" type=\"hidden\" name=\"dictmask\"><input type=\"submit\" value=\""._L("Apply")."\" onClick=\"submitDictMask()\" /></form></td></tr></table><tr><td><hr><div id=\"dictinfo\"></div></td></tr></table></td></tr></table>";
} else {
	echo "You need to <a href=\"login.php\">Sign in</a> first!";
}
include 'end.inc.php';
ob_end_flush();
?>
