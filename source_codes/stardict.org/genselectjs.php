<?php
$username = isset($_REQUEST['username'])?$_REQUEST['username']:"";
$password = isset($_REQUEST['password'])?$_REQUEST['password']:"";
echo "&lt;script src=&quot;http://www.stardict.org/js/stardict.js&quot; type=&quot;text/javascript&quot;&gt;&lt;/script&gt;<br>
&lt;script type=&quot;text/javascript&quot;&gt;<br>dict_username=&quot;";
echo $username;
echo "&quot;;<br>dict_md5passwd=&quot;";
echo md5($password);
echo "&quot;;<br>dictInit();<br>&lt;/script&gt;"
?>
