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
	echo "You are guest user, try to <a href=\"register.php\">Register</a> an account and choose your custom dictionaries.<br><br>";
}
$equeryword = encode_command_arg($queryword);
$args.= "-l ".$equeryword;
include_once 'backend.php';
echo call_stardict_client($args);
?>
