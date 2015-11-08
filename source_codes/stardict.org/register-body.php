<?php ob_start(); ?>
<html><head><meta http-equiv="content-type" content="text/html; charset=UTF-8"><title>StarDict:: Register</title>
<link rel="shortcut icon" href="images/stardict.ico" />
<body><center>
<?php
session_start();
include 'head.php';
?>
Register a new account.<br><br>
<form action="register.php" method="post">
<table>
<tr><td>Username:</td><td><input type="text" name="username" size="20" maxlength="16" /></td></tr>
<tr><td>Password:</td><td><input type="password" name="password" size="20" maxlength="32" /></td></tr>
<tr><td>Password again:</td><td><input type="password" name="password2" size="20" maxlength="32" /></td></tr>
<tr><td>Email:</td><td><input type="text" name="email" size="20" maxlength="32" /><td></tr>
<tr><td colspan="2"><input type="submit" value="Register" /></td></tr>
</table>
</form>
</center></body></html>
<?php
ob_end_flush();
?>
