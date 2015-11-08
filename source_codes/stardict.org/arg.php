<?php
function encode_command_arg($arg)
{
	$earg="\"";
	$len = strlen($arg);
	for ($i=0;$i<$len;$i++) {
		if ($arg[$i]=="\"")
			$earg.="\\\"";
		else if ($arg[$i]=="\\")
			$earg.="\\\\";
		else
			$earg.=$arg[$i];
	}
	$earg.="\"";
	return $earg;
}
?>
