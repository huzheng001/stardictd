<?php
$cookindex = isset($_COOKIE["StarDict_index"])?$_COOKIE["StarDict_index"]:"";
if ($cookindex == "web") {
	include 'web.php';
} else {
	include 'ajax.php';
}
?>
