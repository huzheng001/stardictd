<?php
$file = isset($_REQUEST['file'])?$_REQUEST['file']:"";
header("Location: http://prdownloads.sourceforge.net/stardict/".$file."?download");
/*
include 'downloadlevel.inc.php';
$level = isset($DownloadLevel[$file])?$DownloadLevel[$file]:1;
$tmpdir;
if ($handle = opendir('download')) {
	while (false !== ($tmpdir = readdir($handle))) {
		if ($tmpdir != "." && $tmpdir!= ".." && $tmpdir != "index.php")
			break;
	}
	closedir($handle);
}
if ($level == 0) {
	header("Location: download/".$tmpdir."/".$file);
} else {
	$cookie_username = isset($_COOKIE["StarDict_username"])?$_COOKIE["StarDict_username"]:"";
	$cookie_md5password = isset($_COOKIE["StarDict_password"])?$_COOKIE["StarDict_password"]:"";
	if ($cookie_username == "" || $cookie_md5password == "") {
		echo "<center>Only Level ".$level." user can download this file.<br>Please <a href=\"login.php\">Sign in</a> first!</center>";
	} else {
		include_once 'arg.php';
		$eusername = encode_command_arg($cookie_username);
		$epassword = encode_command_arg($cookie_md5password);
		$args = "-ar ".$eusername." ".$epassword." -gu";
		include_once 'backend.php';
		$content = call_stardict_client($args);
		if ($content < $level) {
			echo "<center>Only Level ".$level." user can download this file.<br><a href=\"finance.php\">Upgrade Now!</a></center>";
		} else {
			header("Location: download/".$tmpdir."/".$file);
		}
	}
}
*/
?>
