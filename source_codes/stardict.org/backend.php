<?php
function call_stardict_client($args)
{
	$cmd="../../cgi-bin/stardict-client ";
	$cmd .= $args;
	$handle=popen($cmd, "r");
	$content = "";
	while (!feof ($handle)) {
		$content.= fread ($handle, 2048);
	}
	pclose($handle);
	return $content;
}
?>
