<?php
header("Content-type:text/html; charset=utf-8");
$username = isset($_REQUEST['username'])?$_REQUEST['username']:"";
$oldpassword = isset($_REQUEST['oldpassword'])?$_REQUEST['oldpassword']:"";
$newpassword = isset($_REQUEST['newpassword'])?$_REQUEST['newpassword']:"";
$newpassword2 = isset($_REQUEST['newpassword2'])?$_REQUEST['newpassword2']:"";
if ($username=="" || $oldpassword=="" || $newpassword=="" || $newpassword!=$newpassword2) {
	echo "Invalid parameters.";
} else {
	include_once 'arg.php';
	$eusername = encode_command_arg($username);
	$eoldpassword =  encode_command_arg($oldpassword);
	$md5saltnewpassword = md5("LoveCher".$newpassword."StarDict");
	$enewpassword = encode_command_arg($md5saltnewpassword);
	$args = "-cr ".$eusername." ".$eoldpassword." ".$enewpassword;
	include_once 'backend.php';
	$content = call_stardict_client($args);
	if ($content=="") {
		session_start();
		$_SESSION['password'] = $md5saltnewpassword;
		$cookpassword = isset($_COOKIE["StarDict_password"])?$_COOKIE["StarDict_password"]:"";
		if ($cookpassword!="")
			setcookie("StarDict_password", $md5saltnewpassword, time()+60*60*24*30, '/');
		echo "<center>Change password success!<br>Back to <a href=\"index.php\">Home</a> page.</center>";
	} else {
		echo $content;
	}
}
?>
