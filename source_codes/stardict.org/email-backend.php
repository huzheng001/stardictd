<?php
header("Content-type:text/html; charset=utf-8");
session_start();
$username = isset($_SESSION['username'])?$_SESSION['username']:"";
$password = isset($_SESSION['password'])?$_SESSION['password']:"";
if ($username!="" && $password!="") {
	$email = isset($_REQUEST['e'])?$_REQUEST['e']:"";
	$args;
	include_once 'arg.php';
        $eusername = encode_command_arg($username);
        $epassword = encode_command_arg($password);
	$eemail = encode_command_arg($email);
        $args = "-ar ".$eusername." ".$epassword." -se ".$eemail;
	include_once 'backend.php';
	echo call_stardict_client($args);
} else {
	echo "Error, didn't login!";
}
?>
