<?php ob_start(); ?>
<html><head><meta http-equiv="content-type" content="text/html; charset=UTF-8"><title>StarDict Query</title>
<link rel="shortcut icon" href="images/stardict.ico" />
<script language="javascript" type="text/javascript" src="js/query.js"></script>
</head>
<body topmargin="3" alink="#ff0000" bgcolor="#ffffff" link="#0000cc" marginheight="3" text="#000000" vlink="#551a8b">
<?php
session_start();
include 'head.php';
include 'query-head.php';
?>
<table border="0" cellpadding="0" cellspacing="0" width="100%"><tbody><tr><td bgcolor="#3366cc"><img alt="" height="1" width="1"></td></tr></tbody></table>
<table bgcolor="#e5ecf9" border="0" cellpadding="0" cellspacing="0" width="100%"><tbody><tr><td bgcolor="#e5ecf9" nowrap="nowrap" width="1%"><font size="+1">&nbsp;<b>
<?php
$queryword = isset($_REQUEST['q'])?$_REQUEST['q']:"";
if ($queryword!="") {
	$charset = isset($_REQUEST['cs'])?$_REQUEST['cs']:"";
	if ($charset!="" && strcasecmp($charset, "UTF-8")!=0) {
		$queryword = iconv($charset, "UTF-8", $queryword);
		if ($queryword == FALSE) {
			echo "Error when converting word.";
			echo "</b></font>&nbsp;</td></tr></tbody></table><br>";
			include 'query-end.php';
			ob_end_flush();
			exit;
		}
	}
	echo $queryword;
} else {
	echo "Please input some words.";
}
echo "</b></font>&nbsp;</td></tr></tbody></table><table width=\"100%\"><tr><td valign=\"top\"><br>";
if ($queryword=="") {
	include 'query-end.php';
	echo "<script language='javascript' type='text/javascript'>document.getElementById(\"end_entry\").focus()</script>";
	ob_end_flush();
	exit;
}
$username = isset($_SESSION['username'])?$_SESSION['username']:"";
$password = isset($_SESSION['password'])?$_SESSION['password']:"";
$args;
include_once 'arg.php';
if ($username!="" && $password!="") {
	$eusername = encode_command_arg($username);
	$epassword = encode_command_arg($password);
	$args = "-ar ".$eusername." ".$epassword." ";
} else {
	$args = "";
	echo "You are guest user, try to <a href=\"register.php\">Register</a> an account and choose your custom dictionaries.<br><br>";
}
$equeryword = encode_command_arg($queryword);
$args.= "-q ".$equeryword;
include_once 'backend.php';
echo call_stardict_client($args);
echo "</td><td align=\"right\">";
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
echo "</td></tr></table>";
include 'query-end.php';
ob_end_flush();
?>
