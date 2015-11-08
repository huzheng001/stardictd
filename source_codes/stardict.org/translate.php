<?php
$language = isset($_SESSION['language'])?$_SESSION['language']:"";
if ($language != "" && $language != "default") {
	include "lang.".$language.".inc";
} else {
	$Translations = array ();
}
function _L($s) {
	global $Translations;
	if (isset($Translations[$s]) && $Translations[$s] != '')
		return $Translations[$s];
	else
		return $s;
}
?>
