<?php
header("Content-type:text/html; charset=utf-8");
$dictmask = isset($_REQUEST['dictmask'])?$_REQUEST['dictmask']:"";
session_start();
$username = isset($_SESSION['username'])?$_SESSION['username']:"";
$password = isset($_SESSION['password'])?$_SESSION['password']:"";
include_once 'arg.php';
$equeryword = encode_command_arg($dictmask);
$eusername = encode_command_arg($username);
$epassword = encode_command_arg($password);
$args = "-ar ".$eusername." ".$epassword." -sd ".$equeryword;
include_once 'backend.php';
$content = call_stardict_client($args);
if ($content=="") {
	$dir = dirname($_SERVER['PHP_SELF']);
	if ($dir == '/')
		$dir = "";
	header("Location: http://".$_SERVER['HTTP_HOST'].$dir."/index.php");
} else {
	echo $content;
}
?>
