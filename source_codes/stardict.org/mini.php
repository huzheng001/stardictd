<?php ob_start(); ?>
<html><head><meta http-equiv="content-type" content="text/html; charset=UTF-8"></head><body>
<?php
$queryword = isset($_REQUEST['q'])?$_REQUEST['q']:"";
$charset = isset($_REQUEST['cs'])?$_REQUEST['cs']:"";
if ($charset!="" && strcasecmp($charset, "UTF-8")!=0) {
	$queryword = iconv($charset, "UTF-8", $queryword);
	if ($queryword == FALSE) {
		echo "Error when converting word.";
		echo "</body></html>";
		ob_end_flush();
		exit;
	}
}
$username = isset($_REQUEST['user'])?$_REQUEST['user']:"";
$password = isset($_REQUEST['passwd'])?$_REQUEST['passwd']:"";
if ($username=="" || $password=="") {
	session_start();
	$username = isset($_SESSION['username'])?$_SESSION['username']:"";
	$password = isset($_SESSION['password'])?$_SESSION['password']:"";
	if ($username=="" || $password=="") {
		$username = isset($_COOKIE["StarDict_username"])?$_COOKIE["StarDict_username"]:"";
		$password = isset($_COOKIE["StarDict_password"])?$_COOKIE["StarDict_password"]:"";
		if ($username!="" && $password!="") {
			$_SESSION['username'] = $username;
			$_SESSION['password'] = $password;
		}
	}
}
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
$args.= "-qs ".$equeryword;
include_once 'backend.php';
echo call_stardict_client($args);
if ($username=="" || $password=="") {
	echo "<br>You are guest user, try to <a href=\"register.php\" target=\"_blank\">Register</a> an account and choose your custom dictionaries.";
}
echo "</body></html>";
ob_end_flush();
?>
