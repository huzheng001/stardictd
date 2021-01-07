<?php
include 'mydomain.inc';

$username = isset($_REQUEST['username'])?$_REQUEST['username']:"";
$password = isset($_REQUEST['password'])?$_REQUEST['password']:"";
echo "&lt;script src=&quot;http://".$mydomain_www_stardict_org."/js/stardict.js&quot; type=&quot;text/javascript&quot;&gt;&lt;/script&gt;<br>
&lt;script type=&quot;text/javascript&quot;&gt;<br>dict_username=&quot;";
echo $username;
echo "&quot;;<br>dict_md5saltpasswd=&quot;";
echo md5("LoveCher".$password."StarDict");
echo "&quot;;<br>dictInit();<br>&lt;/script&gt;"
?>
