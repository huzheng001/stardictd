<?php
header("Content-type:text/html; charset=utf-8");
$queryword = isset($_REQUEST['q'])?$_REQUEST['q']:"";
session_start();
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
}
$equeryword = encode_command_arg($queryword);
$args.= "-d ".$equeryword;
include_once 'backend.php';
echo call_stardict_client($args);
?>
