<?php ob_start(); ?>
<html><head><meta http-equiv="content-type" content="text/html; charset=UTF-8"><title>StarDict:: Admin</title>
<link rel="shortcut icon" href="images/stardict.ico" />
</head><body><center>
<?php
session_start();
include 'head.php';
echo "<br><table border=\"1\"><tr><td><center>Set user level.</center>";
echo "<form action=\"setuserlevel.php\" method=\"post\"><table>";
echo "<tr><td>root password:</td><td><input type=\"password\" name=\"rootpassword\" size=\"20\" maxlength=\"32\" /><td></tr><tr><td>User:</td><td><input type=\"text\" name=\"user\" size=\"20\" maxlength=\"16\" /><td></tr><tr><td>From level:</td><td><input type=\"text\" name=\"fromlevel\" size=\"5\" maxlength=\"1\" /><td></tr><tr><td>To level:</td><td><input type=\"text\" name=\"tolevel\" size=\"5\" maxlength=\"1\" /><td></tr><tr><td colspan=\"2\"><input type=\"submit\" value=\"Save\" /></td></tr></table></form></td></tr></table>";
include 'end.inc.php';
ob_end_flush();
?>
