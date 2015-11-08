<?php
session_start();
unset($_SESSION['username']);
unset($_SESSION['password']);
unset($_SESSION['language']);
setcookie("StarDict_username", "", time()-3600, '/');
setcookie("StarDict_password", "", time()-3600, '/');
setcookie("StarDict_language", "", time()-3600, '/');
setcookie("StarDict_win_x", "", time()-3600, '/');
setcookie("StarDict_win_y", "", time()-3600, '/');
setcookie("StarDict_win_w", "", time()-3600, '/');
setcookie("StarDict_win_h", "", time()-3600, '/');
$dir = dirname($_SERVER['PHP_SELF']);
if ($dir == '/')
	$dir = "";
header("Location: http://".$_SERVER['HTTP_HOST'].$dir."/index.php");
?>
