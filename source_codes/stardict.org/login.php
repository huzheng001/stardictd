<?php
$username = isset($_REQUEST['username'])?$_REQUEST['username']:"";
$password = isset($_REQUEST['password'])?$_REQUEST['password']:"";
if ($username=="" || $password=="") {
	include 'login-body.php';
} else {
	include_once 'arg.php';
	$eusername = encode_command_arg($username);
	$md5saltpassword = md5("LoveCher".$password."StarDict");
	$epassword = encode_command_arg($md5saltpassword);
	$args = "-ar ".$eusername." ".$epassword;
	include_once 'backend.php';
	$content = call_stardict_client($args);
	if ($content=="") {
		$saveinfo = isset($_REQUEST['saveinfo'])?$_REQUEST['saveinfo']:"";
		session_start();
		$_SESSION['username'] = $username;
		$_SESSION['password'] = $md5saltpassword;
		unset($_SESSION['language']);
		if ($saveinfo=="on") {
			setcookie("StarDict_username", $username, time()+60*60*24*30, '/');
			setcookie("StarDict_password", $md5saltpassword, time()+60*60*24*30, '/');
		}
		$dir = dirname($_SERVER['PHP_SELF']);
		if ($dir == '/')
			$dir = "";
		header("Location: http://".$_SERVER['HTTP_HOST'].$dir."/index.php");
	} else {
		echo $content;
	}
}
?>
