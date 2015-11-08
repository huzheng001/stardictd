<?php
header("Content-type:text/html; charset=utf-8");
session_start();
$username = isset($_SESSION['username'])?$_SESSION['username']:"";
$password = isset($_SESSION['password'])?$_SESSION['password']:"";
if ($username!="" && $password!="") {
	$lang = isset($_REQUEST['l'])?$_REQUEST['l']:"";
	$args;
	include_once 'arg.php';
        $eusername = encode_command_arg($username);
        $epassword = encode_command_arg($password);
	$elang = encode_command_arg($lang);
        $args = "-ar ".$eusername." ".$epassword." -sl ".$elang;
	include_once 'backend.php';
	$content = call_stardict_client($args);
	if ($content == "") {
		$_SESSION['language'] = $lang;
		setcookie("StarDict_language", $lang, time()+60*60*24*30, '/');
	} else {
		echo $content;
	}
} else {
	echo "Error, didn't login!";
}
?>
