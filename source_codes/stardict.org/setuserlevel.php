<?php
header("Content-type:text/html; charset=utf-8");
$rootpassword = isset($_REQUEST['rootpassword'])?$_REQUEST['rootpassword']:"";
$user = isset($_REQUEST['user'])?$_REQUEST['user']:"";
$fromlevel = isset($_REQUEST['fromlevel'])?$_REQUEST['fromlevel']:"";
$tolevel = isset($_REQUEST['tolevel'])?$_REQUEST['tolevel']:"";
if ($rootpassword=="" || $user=="" || $fromlevel=="" || $tolevel=="") {
	echo "Invalid parameters.";
} else {
	include_once 'arg.php';
	$erootpassword = encode_command_arg(md5($rootpassword."StarDict"));
	$euser = encode_command_arg($user);
	$efromlevel = encode_command_arg($fromlevel);
	$etolevel = encode_command_arg($tolevel);
	$args = "-ar root ".$erootpassword." -u ".$euser." ".$efromlevel." ".$etolevel;
	include_once 'backend.php';
	echo call_stardict_client($args);
}
?>
