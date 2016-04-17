<?php ob_start(); ?>
<html><head><meta http-equiv="content-type" content="text/html; charset=UTF-8"><title>StarDict:: Select and Translate</title>
<link rel="shortcut icon" href="images/stardict.ico" />
<body><center>
<?php
session_start();
include 'head.php';
include 'mydomain.inc';

echo "<br><table border=\"1\"><tr><td><b>"._L('How to use "Select and Translate" in other web page?')."</b><br>"._L('Bookmark the next link, or just drag it to your toolbar:')."<br>";
echo "<a href=\"javascript:void((function() {var element=document.createElement('script'); element.setAttribute('src', 'http://".$mydomain_www_stardict_org."/js/initselect.js";
$username = isset($_SESSION['username'])?$_SESSION['username']:"";
$password = isset($_SESSION['password'])?$_SESSION['password']:"";
if ($username!="" && $password!="") {
	echo "?user=";
	echo $username;
	echo "&passwd=";
	echo $password;
}
echo "'); document.body.appendChild(element);})())\"><font color=red><b>StarDict</b></font></a>";
if ($username=="" || $password=="") {
	echo "<br><font color=red>Notice:</font> You are guest user, you'd better <a href=\"login.php\">login</a> first, so this link will contain your user info and you can use your custom dictionaries by it.<br>";
}
echo "<br>"._L("Then open any web page, and click the StarDict link on your toolbar.")."</td></tr></table>";
?>
<br><br><table border="1">
<tr><td><b>How to add "Select and Translate" in your website?</b><br>
Put these codes in your website:<br><i>
&lt;script src=&quot;<?php echo "http://".$mydomain_www_stardict_org."/js/stardict.js" ?>&quot; type=&quot;text/javascript&quot;&gt;&lt;/script&gt;<br>
&lt;script type=&quot;text/javascript&quot;&gt;<br>
dictInit();<br>
&lt;/script&gt;</i><hr>
You can add &quot;dict_username&quot; and &quot;dict_md5saltpasswd&quot; into this script, so you can use custom dictionaries.<br>
<form action="genselectjs.php" method="post"><table><tr><td>Username:</td><td><input type="text" name="username" size="20" maxlength="16" /></td></tr><tr><td>Password:</td><td><input type="password" name="password" size="20" maxlength="32" /></td></tr><tr><td colspan="2"><input type="submit" value="Generate JavaScript code" /></td></tr></table></form>
</td></tr>
<tr><td><b>Enable translate opinion:</b><br>
If you want to show a "Enable translate opinion" in your webpage, put these codes in suitable place:<br><i>&lt;span id=&quot;dict_status&quot;&gt;&lt;/span&gt;</i>
</td></tr>
<tr><td><small>* If you want to disable "Select and Translate" by default, add <b><i>dict_enable=false;</i></b> before <b><i>dictInit();</i></b></small></td></tr></table>
<br><b>Provide Dictionary Lookup Service in your website.</b><table border="1"><tr><td>Insert these codes in your web page:</td><td>Preview:</td></tr><tr><td><p><i>&lt;form method=&quot;post&quot; target=&quot;_blank&quot;<br>
action=&quot;<?php echo "http://".$mydomain_www_stardict_org."/query.php" ?>&quot;&gt;<br>
&lt;input maxlength=256 size=21 name=q /&gt;&lt;br&gt;<br>
&lt;a href=&quot;<?php echo "http://".$mydomain_www_stardict_org ?>&quot; target=&quot;_blank&quot;&gt;StarDict&lt;/a&gt;<br>
&lt;input type=&quot;hidden&quot; name=&quot;cs&quot; value=&quot;UTF-8&quot; /&gt;<br>
&lt;input type=&quot;submit&quot; value=&quot;Look Up&quot; /&gt;<br>
&lt;/form&gt;</i></p></td><td>
<form method="post" target="_blank" 
action="query.php">
<input maxlength=256 size=21 name=q /><br>
<a href="http://<?php echo $mydomain_www_stardict_org ?>" target="_blank">StarDict</a>
<input type="hidden" name="cs" value="UTF-8" />
<input type="submit" value="Look Up" />
</form>
</td></tr><tr><td colspan="2"><small>* You may need to change &quot;UTF-8&quot; to the charset that you are using in your web page.</td></tr></table>
<?php
include 'end.inc.php';
ob_end_flush();
?>
