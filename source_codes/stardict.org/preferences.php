<?php ob_start(); ?>
<html><head><meta http-equiv="content-type" content="text/html; charset=UTF-8"><title>StarDict:: Preferences</title>
<link rel="shortcut icon" href="images/stardict.ico" />
<script language="javascript" type="text/javascript" src="js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="js/preferences.js"></script>
</head>
<body><center>
<?php
session_start();
include 'head.php';
$username = isset($_SESSION['username'])?$_SESSION['username']:"";
$password = isset($_SESSION['password'])?$_SESSION['password']:"";
if ($username!="" && $password!="") {
	echo "<br>"._L("Language:")." <select id=\"language\"><option value=\"default\">English</option><option value=\"zh_CN\">简体中文</option><option value=\"zh_TW\">繁體中文</option><option value=\"ko\">Korean</option><option value=\"sk\">Slovak</option></select> <input type=\"button\" value=\""._L("Apply")."\" onClick=\"apply_language()\" /> &nbsp;&nbsp; <a href=\"helptranslate.php\">Help us to translate</a><br>";
	echo "<br>"._L("Sort word list by collate function:")." <select id=\"collatefunc\"><option value=\"0\">Default</option><option value=\"1\">utf8_general_ci</option><option value=\"2\">utf8_unicode_ci</option><option value=\"3\">utf8_bin</option><option value=\"4\">utf8_czech_ci</option><option value=\"5\">utf8_danish_ci</option><option value=\"6\">utf8_esperanto_ci</option><option value=\"7\">utf8_estonian_ci</option><option value=\"8\">utf8_hungarian_ci</option><option value=\"9\">utf8_icelandic_ci</option><option value=\"10\">utf8_latvian_ci</option><option value=\"11\">utf8_lithuanian_ci</option><option value=\"12\">utf8_persian_ci</option><option value=\"13\">utf8_polish_ci</option><option value=\"14\">utf8_roman_ci</option><option value=\"15\">utf8_romanian_ci</option><option value=\"16\">utf8_slovak_ci</option><option value=\"17\">utf8_slovenian_ci</option><option value=\"18\">utf8_spanish_ci</option><option value=\"19\">utf8_spanish2_ci</option><option value=\"20\">utf8_swedish_ci</option><option value=\"21\">utf8_turkish_ci</option></select> <input type=\"button\" value=\""._L("Apply")."\" onClick=\"apply_collate_func()\" /><br><span id=\"collatefunc_info\"></span>";
	include_once 'arg.php';
	$eusername = encode_command_arg($username);
	$epassword = encode_command_arg($password);
	$args = "-ar ".$eusername." ".$epassword." +gl -gf";
	include_once 'backend.php';
	$content = call_stardict_client($args);
	$result = explode("", $content);
	echo "<script language='javascript' type='text/javascript'>document.getElementById(\"language\").value=\"";
	echo $result[0];
	echo "\";document.getElementById(\"collatefunc\").value=\"";
	echo $result[1];
	echo "\";</script>";
} else {
        echo "You need to <a href=\"login.php\">Sign in</a> first!";
}
include 'end.inc.php';
ob_end_flush();
?>
