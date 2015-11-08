<?php
$username = isset($_REQUEST['username'])?$_REQUEST['username']:"";
$password = isset($_REQUEST['password'])?$_REQUEST['password']:"";
$password2 = isset($_REQUEST['password2'])?$_REQUEST['password2']:"";
$email = isset($_REQUEST['email'])?$_REQUEST['email']:"";
if ($username=="" || $password=="" || $password2=="" || $password!=$password2 || strpos($email, "@")===false) {
	include 'register-body.php';
} else {
	include_once 'arg.php';
	$eusername = encode_command_arg($username);
	$md5password = md5($password);
	$epassword = encode_command_arg($md5password);
	$eemail = encode_command_arg($email);
	$args = "-rr ".$eusername." ".$epassword." ".$eemail;
	include_once 'backend.php';
	$content = call_stardict_client($args);
	if ($content=="") {
		echo "<center>Register success!<br><a href=\"login.php\">Log in</a> now.</center>";
	} else {
		echo $content;
	}
}
