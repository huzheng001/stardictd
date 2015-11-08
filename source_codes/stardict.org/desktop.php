<?php
if (isset($_REQUEST['pos'])) {
	setcookie("StarDict_win_x", $_REQUEST['x'], time()+60*60*24*30, '/');
	setcookie("StarDict_win_y", $_REQUEST['y'], time()+60*60*24*30, '/');
	setcookie("StarDict_win_w", $_REQUEST['w'], time()+60*60*24*30, '/');
	setcookie("StarDict_win_h", $_REQUEST['h'], time()+60*60*24*30, '/');
}
?>
