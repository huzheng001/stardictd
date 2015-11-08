<?php
header("Content-type:text/html; charset=utf-8");
$parent = isset($_REQUEST['parent'])?$_REQUEST['parent']:"";
$showadddict = isset($_REQUEST['showadddict'])?$_REQUEST['showadddict']:"1";
include_once 'arg.php';
$equeryword = encode_command_arg($parent);
if ($showadddict == "0")
	$args = "-ird ".$equeryword;
else
	$args = "-ir ".$equeryword;
include_once 'backend.php';
echo call_stardict_client($args);
?>
