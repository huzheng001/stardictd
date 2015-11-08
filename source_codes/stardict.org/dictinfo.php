<?php
header("Content-type:text/html; charset=utf-8");
$uid = isset($_REQUEST['uid'])?$_REQUEST['uid']:"";
include_once 'arg.php';
$equeryword = encode_command_arg($uid);
$args = "-id ".$equeryword;
include_once 'backend.php';
echo call_stardict_client($args);
?>
