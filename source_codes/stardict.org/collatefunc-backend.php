<?php
header("Content-type:text/html; charset=utf-8");
session_start();
$username = isset($_SESSION['username'])?$_SESSION['username']:"";
$password = isset($_SESSION['password'])?$_SESSION['password']:"";
if ($username!="" && $password!="") {
	$func = isset($_REQUEST['f'])?$_REQUEST['f']:"";
	$args;
	include_once 'arg.php';
        $eusername = encode_command_arg($username);
        $epassword = encode_command_arg($password);
	$efunc = encode_command_arg($func);
        $args = "-ar ".$eusername." ".$epassword." -sf ".$efunc;
	include_once 'backend.php';
	echo call_stardict_client($args);
} else {
	echo "Error, didn't login!";
}
?>
