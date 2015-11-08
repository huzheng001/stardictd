<?php ob_start(); ?>
<html><head><meta http-equiv="content-type" content="text/html; charset=UTF-8"><title>StarDict:: Finance</title>
<link rel="shortcut icon" href="images/stardict.ico" />
</head>
<body><center>
<?php
session_start();
include 'head.php';
echo "<br><table border=\"1\"><tr><td>"._L('If you like StarDict, you can consider to upgrade to level 1 user, this will help StarDict develop further.<br>Upgrade to level 1 user can get better service when using StarDict too.<br><br>Normal registered user have these limits when using StarDict:<br>1.Can only choose 8 dictionaries.<br>2.Can\'t use some advance dictionaries.<br>3.Can\'t use collate function.<br>4.The usage of query, pattern match, fuzzy search, full-text search are limited.<br>You can only query for 50 times in one hour, use pattern match and fuzzy search for 5 times each in one hour, and 3 times for full-text search.')."<br><br>"._L('If you have done any contribution to StarDict ago, please <a href="mailto:huzheng001@gmail.com">mail me</a> and I will upgrade your account for Free!')."</td></tr></table><br>";
$username = isset($_SESSION['username'])?$_SESSION['username']:"";
$password = isset($_SESSION['password'])?$_SESSION['password']:"";
if ($username=="" || $password=="") {
	echo "You need to <a href=\"login.php\">Sign in</a> first!";
} else {
	include_once 'arg.php';
	$eusername = encode_command_arg($username);
	$epassword = encode_command_arg($password);
	$args = "-ar ".$eusername." ".$epassword." -gu";
	include_once 'backend.php';
	$content = call_stardict_client($args);
	if ($content=="") {
		echo "Error while getting user level information.";
	} else {
		printf(_L("You are level %d user currently."), $content);
		if ($content=="0") {
echo "<br><table border=\"1\"><tr><td>";
printf(_L("Upgrade to Level %d user:"), 1);
echo "<center><form action=\"https://www.paypal.com/cgi-bin/webscr\" method=\"post\">
<input type=\"hidden\" name=\"cmd\" value=\"_xclick\">
<input type=\"hidden\" name=\"business\" value=\"alantang@ieee.org\">
<input type=\"hidden\" name=\"item_name\" value=\"Upgrade from level 0 to level 1 in www.stardict.org\">
<input type=\"hidden\" name=\"item_number\" value=\"01\">
<input type=\"hidden\" name=\"amount\" value=\"5.00\">
<input type=\"hidden\" name=\"no_shipping\" value=\"2\">
<input type=\"hidden\" name=\"return\" value=\"http://www.stardict.org/paymentreceived.php\">
<input type=\"hidden\" name=\"cancel_return\" value=\"http://www.stardict.org/finance.php\">
<input type=\"hidden\" name=\"no_note\" value=\"1\">
<input type=\"hidden\" name=\"currency_code\" value=\"USD\">
<input type=\"hidden\" name=\"bn\" value=\"PP-BuyNowBF\">
<input type=\"hidden\" name=\"on0\" value=\"account\"><input type=\"hidden\" name=\"os0\" value=\"";
echo $username;
echo "\"><input type=\"image\" src=\"https://www.paypal.com/en_US/i/btn/x-click-but23.gif\" border=\"0\" name=\"submit\" alt=\"Buy it\">
<img alt=\"\" border=\"0\" src=\"https://www.paypal.com/en_US/i/scr/pixel.gif\" width=\"1\" height=\"1\">
</form></center>
<a href='https://login.taobao.com/auction/fast_buy_item.htm?auction_id=532034f765dbfa214ce156716961be58&x_id=null&key=7c5f69ca4036d6515f85c6058262f914'><img alt='网上汇款免手续费， 收货满意后卖家才能拿钱，货款都安全' src='http://img.alipay.com/pimg/button_alipaybutton_w.gif' align='absmiddle' border='0'/></a>
<a href='https://www.99bill.com/webapp/payAction.do?ad=88990610177124105&buttonid=47908&mac=1DA09AA7031F1EDF5FD9869B5C25307F' target='_blank'><img border=0 alt='通过快钱购买！热！' src='https://www.99bill.com/img_2/btnimg/button1.gif' /></a>
</td>
<td>"._L("Cost:")." $5.</td>
<td>";
printf(_L("Level %d user can choose %d dictionaries."), 1, 15);
echo "<br>"._L("Can use advance dictionaries.")."<br>"._L("Can use the collate function.")."<br>"._L("Use query, pattern match and fuzzy search without limit.")."<br>";
printf(_L("Use full-text search for %d times in one hour."), 10);
echo "</td></tr>
<tr><td>";
printf(_L("Upgrade to Level %d user:"), 2);
echo "<center><form action=\"https://www.paypal.com/cgi-bin/webscr\" method=\"post\">
<input type=\"hidden\" name=\"cmd\" value=\"_xclick\">
<input type=\"hidden\" name=\"business\" value=\"alantang@ieee.org\">
<input type=\"hidden\" name=\"item_name\" value=\"Upgrade from level 0 to level 2 in www.stardict.org\">
<input type=\"hidden\" name=\"item_number\" value=\"02\">
<input type=\"hidden\" name=\"amount\" value=\"10.00\">
<input type=\"hidden\" name=\"no_shipping\" value=\"2\">
<input type=\"hidden\" name=\"return\" value=\"http://www.stardict.org/paymentreceived.php\">
<input type=\"hidden\" name=\"cancel_return\" value=\"http://www.stardict.org/finance.php\">
<input type=\"hidden\" name=\"no_note\" value=\"1\">
<input type=\"hidden\" name=\"currency_code\" value=\"USD\">
<input type=\"hidden\" name=\"bn\" value=\"PP-BuyNowBF\">
<input type=\"hidden\" name=\"on0\" value=\"account\"><input type=\"hidden\" name=\"os0\" value=\"";
echo $username;
echo "\"><input type=\"image\" src=\"https://www.paypal.com/en_US/i/btn/x-click-but23.gif\" border=\"0\" name=\"submit\" alt=\"Buy it\">
<img alt=\"\" border=\"0\" src=\"https://www.paypal.com/en_US/i/scr/pixel.gif\" width=\"1\" height=\"1\">
</form></center>
<a href='https://login.taobao.com/auction/fast_buy_item.htm?auction_id=8eda82e01e18eb76761ecc2faa4bb1f6&x_id=null&key=d0602c8a9caef6a511aa236884e89889'><img alt='网上汇款免手续费， 收货满意后卖家才能拿钱，货款都安全' src='http://img.alipay.com/pimg/button_alipaybutton_w.gif' align='absmiddle' border='0'/></a>
<a href='https://www.99bill.com/webapp/payAction.do?ad=88990610177124105&buttonid=47917&mac=195FBDFAFDB9A148AE902F939496FCCD' target='_blank'><img border=0 alt='通过快钱购买！热！' src='https://www.99bill.com/img_2/btnimg/button1.gif' /></a>";
echo "</td><td>"._L("Cost:")." $10.</td>
<td>";
printf(_L("Level %d user can choose %d dictionaries."), 2, 25);
echo "<br>";
printf(_L("Use full-text search for %d times in one hour."), 20);
echo "</td></tr>
<tr><td>";
printf(_L("Upgrade to Level %d user:"), 3);
echo "<center><form action=\"https://www.paypal.com/cgi-bin/webscr\" method=\"post\">
<input type=\"hidden\" name=\"cmd\" value=\"_xclick\">
<input type=\"hidden\" name=\"business\" value=\"alantang@ieee.org\">
<input type=\"hidden\" name=\"item_name\" value=\"Upgrade from level 0 to level 3 in www.stardict.org\">
<input type=\"hidden\" name=\"item_number\" value=\"03\">
<input type=\"hidden\" name=\"amount\" value=\"20.00\">
<input type=\"hidden\" name=\"no_shipping\" value=\"2\">
<input type=\"hidden\" name=\"return\" value=\"http://www.stardict.org/paymentreceived.php\">
<input type=\"hidden\" name=\"cancel_return\" value=\"http://www.stardict.org/finance.php\">
<input type=\"hidden\" name=\"no_note\" value=\"1\">
<input type=\"hidden\" name=\"currency_code\" value=\"USD\">
<input type=\"hidden\" name=\"bn\" value=\"PP-BuyNowBF\">
<input type=\"hidden\" name=\"on0\" value=\"account\"><input type=\"hidden\" name=\"os0\" value=\"";
echo $username;
echo "\"><input type=\"image\" src=\"https://www.paypal.com/en_US/i/btn/x-click-but23.gif\" border=\"0\" name=\"submit\" alt=\"Buy it\">
<img alt=\"\" border=\"0\" src=\"https://www.paypal.com/en_US/i/scr/pixel.gif\" width=\"1\" height=\"1\">
</form></center>
<a href='https://login.taobao.com/auction/fast_buy_item.htm?auction_id=360927817f216234c3ace89c907fca7c&x_id=null&key=55eb67251538ef4592e3c33a96dbad07'><img alt='网上汇款免手续费， 收货满意后卖家才能拿钱，货款都安全' src='http://img.alipay.com/pimg/button_alipaybutton_w.gif' align='absmiddle' border='0'/></a>
<a href='https://www.99bill.com/webapp/payAction.do?ad=88990610177124105&buttonid=47920&mac=11BCFB4812469398F69F975DE3D7742A' target='_blank'><img border=0 alt='通过快钱购买！热！' src='https://www.99bill.com/img_2/btnimg/button1.gif' /></a>";
echo "</td><td>"._L("Cost:")." $20.</td>
<td>";
printf(_L("Level %d user can choose %d dictionaries."), 3, 35);
echo "<br>";
printf(_L("Use full-text search for %d times in one hour."), 30);
echo "</td></tr>
<tr><td>";
printf(_L("Upgrade to Level %d user:"), 4);
echo "<center><form action=\"https://www.paypal.com/cgi-bin/webscr\" method=\"post\">
<input type=\"hidden\" name=\"cmd\" value=\"_xclick\">
<input type=\"hidden\" name=\"business\" value=\"alantang@ieee.org\">
<input type=\"hidden\" name=\"item_name\" value=\"Upgrade from level 0 to level 4 in www.stardict.org\">
<input type=\"hidden\" name=\"item_number\" value=\"04\">
<input type=\"hidden\" name=\"amount\" value=\"50.00\">
<input type=\"hidden\" name=\"no_shipping\" value=\"2\">
<input type=\"hidden\" name=\"return\" value=\"http://www.stardict.org/paymentreceived.php\">
<input type=\"hidden\" name=\"cancel_return\" value=\"http://www.stardict.org/finance.php\">
<input type=\"hidden\" name=\"no_note\" value=\"1\">
<input type=\"hidden\" name=\"currency_code\" value=\"USD\">
<input type=\"hidden\" name=\"bn\" value=\"PP-BuyNowBF\">
<input type=\"hidden\" name=\"on0\" value=\"account\"><input type=\"hidden\" name=\"os0\" value=\"";
echo $username;
echo "\"><input type=\"image\" src=\"https://www.paypal.com/en_US/i/btn/x-click-but23.gif\" border=\"0\" name=\"submit\" alt=\"Buy it\">
<img alt=\"\" border=\"0\" src=\"https://www.paypal.com/en_US/i/scr/pixel.gif\" width=\"1\" height=\"1\">
</form></center>
<a href='https://login.taobao.com/auction/fast_buy_item.htm?auction_id=680637d1213b933173445e285fc3d924&x_id=null&key=e52b8b91d8c4b6e6e044810e544faf30'><img alt='网上汇款免手续费， 收货满意后卖家才能拿钱，货款都安全' src='http://img.alipay.com/pimg/button_alipaybutton_w.gif' align='absmiddle' border='0'/></a>
<a href='https://www.99bill.com/webapp/payAction.do?ad=88990610177124105&buttonid=47922&mac=E732893151B30D919D59FD137BD6F04B' target='_blank'><img border=0 alt='通过快钱购买！热！' src='https://www.99bill.com/img_2/btnimg/button1.gif' /></a>";
echo "</td><td>"._L("Cost:")." $50.</td>
<td>";
printf(_L("Level %d user can choose %d dictionaries."), 4, 45);
echo "<br>";
printf(_L("Use full-text search for %d times in one hour."), 40);
echo "</td></tr>
<tr><td>";
printf(_L("Upgrade to Level %d user:"), 5);
echo "<center><form action=\"https://www.paypal.com/cgi-bin/webscr\" method=\"post\">
<input type=\"hidden\" name=\"cmd\" value=\"_xclick\">
<input type=\"hidden\" name=\"business\" value=\"alantang@ieee.org\">
<input type=\"hidden\" name=\"item_name\" value=\"Upgrade from level 0 to level 5 in www.stardict.org\">
<input type=\"hidden\" name=\"item_number\" value=\"05\">
<input type=\"hidden\" name=\"amount\" value=\"100.00\">
<input type=\"hidden\" name=\"no_shipping\" value=\"2\">
<input type=\"hidden\" name=\"return\" value=\"http://www.stardict.org/paymentreceived.php\">
<input type=\"hidden\" name=\"cancel_return\" value=\"http://www.stardict.org/finance.php\">
<input type=\"hidden\" name=\"no_note\" value=\"1\">
<input type=\"hidden\" name=\"currency_code\" value=\"USD\">
<input type=\"hidden\" name=\"bn\" value=\"PP-BuyNowBF\">
<input type=\"hidden\" name=\"on0\" value=\"account\"><input type=\"hidden\" name=\"os0\" value=\"";
echo $username;
echo "\"><input type=\"image\" src=\"https://www.paypal.com/en_US/i/btn/x-click-but23.gif\" border=\"0\" name=\"submit\" alt=\"Buy it\">
<img alt=\"\" border=\"0\" src=\"https://www.paypal.com/en_US/i/scr/pixel.gif\" width=\"1\" height=\"1\">
</form></center>
<a href='https://login.taobao.com/auction/fast_buy_item.htm?auction_id=d80c75f72c0f0ab9a498af06935b2a07&x_id=null&key=d7de119c75b922e45dd7d365bcc36ccd'><img alt='网上汇款免手续费， 收货满意后卖家才能拿钱，货款都安全' src='http://img.alipay.com/pimg/button_alipaybutton_w.gif' align='absmiddle' border='0'/></a>
<a href='https://www.99bill.com/webapp/payAction.do?ad=88990610177124105&buttonid=47923&mac=75516F5353B2B790E445F20CEB0FD599' target='_blank'><img border=0 alt='通过快钱购买！热！' src='https://www.99bill.com/img_2/btnimg/button2.gif' /></a>";
echo "</td><td>"._L("Cost:")." $100.</td>
<td>";
printf(_L("Level %d user can choose %d dictionaries."), 5, 55);
echo "<br>";
printf(_L("Use full-text search for %d times in one hour."), 50);
echo "</td></tr></table>";
		} else if ($content=="1") {
		} else if ($content=="2") {
		} else if ($content=="3") {
		} else if ($content=="4") {
		} else {
			echo "You are the highest level user already!";
		}
	}
}
include 'end.inc.php';
ob_end_flush();
?>
