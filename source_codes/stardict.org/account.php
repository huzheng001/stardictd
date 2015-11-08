<?php ob_start(); ?>
<html><head><meta http-equiv="content-type" content="text/html; charset=UTF-8"><title>StarDict:: Account</title>
<link rel="shortcut icon" href="images/stardict.ico" />
<script language="javascript" type="text/javascript" src="js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="js/account.js"></script>
</head><body><center>
<?php
session_start();
include 'head.php';
$username = isset($_SESSION['username'])?$_SESSION['username']:"";
$password = isset($_SESSION['password'])?$_SESSION['password']:"";
if ($username!="" && $password!="") {
	echo "<br><a href=\"preferences.php\">"._L("Preferences")."</a> &nbsp;&nbsp; <a href=\"dictmanage.php\">"._L("Dict Manage")."</a><br>";
	echo "<br><table><tr><td><table border=\"1\"><tr><td><center>";
	printf(_L('Change <b>%s</b>\'s password.'), $username);
	echo "</center>";
	echo "<form action=\"changepassword.php\" method=\"post\"><table><input type=\"hidden\" name=\"username\" value=\"";
	echo $username;
	echo "\" /><tr><td>"._L("Current password:")."</td><td><input type=\"password\" name=\"oldpassword\" size=\"20\" maxlength=\"32\" /><td></tr><tr><td>"._L("New password:")."</td><td><input type=\"password\" name=\"newpassword\" size=\"20\" maxlength=\"32\" /><td></tr><tr><td>"._L("Confirm new password:")."</td><td><input type=\"password\" name=\"newpassword2\" size=\"20\" maxlength=\"32\" /><td></tr><tr><td colspan=\"2\"><input type=\"submit\" value=\""._L("Save")."\" /></td></tr></table></form></td></tr></table></td><td><table border=\"1\"><tr><td><center>"._L("Other Information:")."</center>"._L("Email:")." <input type=\"text\" size=\"20\" id=\"email\" value=\"";
	include_once 'arg.php';
	$eusername = encode_command_arg($username);
	$epassword = encode_command_arg($password);
	$args = "-ar ".$eusername." ".$epassword." -ge";
	include_once 'backend.php';
	echo call_stardict_client($args);
	echo "\" /> <input type=\"button\" value=\""._L("Update")."\" onClick=\"update_email()\" /><br><center><span id=\"update_info\"></span></center>";
	echo "</td></tr></table></td></tr></table>";
} else {
	echo "You need to <a href=\"login.php\">Sign in</a> first!";
}
include 'end.inc.php';
ob_end_flush();
?>
