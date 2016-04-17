<?php
include 'mydomain.inc';
?>
<table border="0" cellpadding="0" cellspacing="0" width="100%"><tbody><tr><td align="left"><a href="index.php"><?php echo $mydomain_www_StarDict_org; ?></a></td><td align="center"><span id="dict_status"></span></td><td align="right" nowrap="nowrap"><font size="-1">
<?php
$username = isset($_SESSION['username'])?$_SESSION['username']:"";
$password = isset($_SESSION['password'])?$_SESSION['password']:"";
$logged_in;
if ($username!="" && $password!="") {
	$logged_in = TRUE;
	echo "<b>".$username."</b>";
} else {
	$cookie_username = isset($_COOKIE["StarDict_username"])?$_COOKIE["StarDict_username"]:"";
	$cookie_md5saltpassword = isset($_COOKIE["StarDict_password"])?$_COOKIE["StarDict_password"]:"";
	if ($cookie_username!="" && $cookie_md5saltpassword!="") {
		$_SESSION['username'] = $cookie_username;
		$_SESSION['password'] = $cookie_md5saltpassword;
		$logged_in = TRUE;
		echo "<b>".$cookie_username."</b>";
	} else {
		$logged_in = FALSE;
		echo "<a href=\"login.php\">Sign in</a> <a href=\"register.php\">Register</a>";
	}
}
$language = isset($_SESSION['language'])?$_SESSION['language']:"";
if ($language == "") {
	$cookie_language = isset($_COOKIE["StarDict_language"])?$_COOKIE["StarDict_language"]:"";
	if ($cookie_language!="") {
		$_SESSION['language'] = $cookie_language;
	} else {
		if ($logged_in) {
			include_once 'arg.php';
			$eusername = encode_command_arg($_SESSION['username']);
			$epassword = encode_command_arg($_SESSION['password']);
			$args = "-ar ".$eusername." ".$epassword." -gl";
			include_once 'backend.php';
			$content = call_stardict_client($args);
			if ($content == "") {
				$_SESSION['language'] = "default";
			} else {
				$_SESSION['language'] = $content;
			}
			setcookie("StarDict_language", $_SESSION['language'], time()+60*60*24*30, '/');
		} else {
			$_SESSION['language'] = "default";
		}
	}
}
include 'translate.php';

if ($logged_in) {
	echo "&nbsp;|&nbsp;<a href=\"finance.php\">"._L("Finance Center")."</a>&nbsp;|&nbsp;<a href=\"account.php\">"._L("My Account")."</a>&nbsp;|&nbsp;<a href=\"logout.php\">"._L("Sign out")."</a>";
}
?>
</font></td></tr><tr height="4"><td><img alt="" height="1" width="1"></td></tr></tbody></table>
<script type="text/javascript" src="js/stardict.js"></script>
<script type="text/javascript">dictInit();</script>
