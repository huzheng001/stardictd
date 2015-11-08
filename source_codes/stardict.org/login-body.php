<?php ob_start(); ?>
<html><head><meta http-equiv="content-type" content="text/html; charset=UTF-8"><title>StarDict:: Log in</title>
<link rel="shortcut icon" href="images/stardict.ico" />
<body><center>
<?php
session_start();
include 'head.php';
?>
Please enter your username and password to log in.<br><br>
<form action="login.php" method="post">
<table>
<tr><td>Username:</td><td><input type="text" name="username" size="20" maxlength="16" /></td></tr>
<tr><td>Password:</td><td><input type="password" name="password" size="20" maxlength="32" /></td></tr>
<tr><td colspan="2">Remember me:<input type="checkbox" name="saveinfo" checked /></td></tr>
<tr><td colspan="2"><input type="submit" value="Log in" /></td></tr>
</table>
</form>
<br>No account? <a href="register.php">Register</a> now!
</center></body></html>
<?php
ob_end_flush();
?>
