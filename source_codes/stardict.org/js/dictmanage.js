function parseDictInfo(id)
{
	var el= document.getElementById("dictinfo");
	el.innerHTML = _xmlHttp.responseText;
	var loadDiv = document.getElementById("load");
	loadDiv.style.display = "none";
}
function parseSubTree(id)
{
	var el= document.getElementById(id);
	var ulElmt = document.createElement("UL");
	ulElmt.innerHTML = _xmlHttp.responseText;
	el.appendChild(ulElmt);
	var images = el.getElementsByTagName("IMG");
	images[0].setAttribute("src", "images/minus.gif");
	images[0].onclick = function(){showHide(id)};
	var aTag = el.getElementsByTagName("A");
	aTag[0].onclick = function(){showHide(id)};
	var loadDiv = document.getElementById("load");
	loadDiv.style.display = "none";
}
function load()
{
	var loadDiv = document.getElementById("load");
	loadDiv.style.display = "block";
}
function getSubTree(id)
{
	var submitURL="dirinfo.php?parent=" + id;
	postXmlHttp(submitURL, 'parseSubTree("' + id + '")');
}
var _ChangeCallBack;
var _xmlHttp;
function postXmlHttp(submitURL, callbackFunc)
{
	_ChangeCallBack = callbackFunc;
	_xmlHttp = createXMLHttpRequest();
	_xmlHttp.open('POST', submitURL, true);
	_xmlHttp.onreadystatechange = handleStateChange;
	_xmlHttp.send(null);
}
function handleStateChange() {
	if (_xmlHttp.readyState == 4 && _xmlHttp.status == 200) {
		setTimeout(_ChangeCallBack, 2);
	}
	if (_xmlHttp.readyState == 1) {
		load();
	}
}
function showDictInfo(id)
{
	var submitURL="dictinfo.php?uid=" + id;
	postXmlHttp(submitURL, 'parseDictInfo("' + id + '")');
}
function showNeedLevel()
{
	alert("Only advance user can choose this dictionary.");
}
function addDict(uid, bookname, wordcount)
{
	var exist = document.getElementById(uid);
	if (exist)
		return;
	var maxdictcount = document.getElementById("maxdictcount").innerHTML;
	var el= document.getElementById("dictlist");
	if (el.childNodes.length>=maxdictcount) {
		alert("You can only choose "+ maxdictcount +" dictionaries!");
		return;
	}
	var oElmt = document.createElement("option");
	oElmt.setAttribute("id", uid);
	oElmt.setAttribute("value", uid);
	oElmt.innerHTML = bookname + " " + wordcount;
	el.appendChild(oElmt);
}
function showDictInfo2()
{
	var el= document.getElementById("dictlist");
	var uid = el.value;
	if (uid)
		showDictInfo(el.value);
}
function deleteDict()
{
	var el= document.getElementById("dictlist");
	var uid = el.value;
	if (uid) {
		var oElmt = document.getElementById(uid);
		if (oElmt) {
			el.removeChild(oElmt);
		}
	}
}
function swapNode(node1, node2)
{
	var parent = node1.parentNode;
	var t1 = node1.nextSibling;
	var t2 = node2.nextSibling;
	if (t1)
		parent.insertBefore(node2, t1);
	else
		parent.appendChild(node2);
	if (t2)
		parent.insertBefore(node1, t2);
	else
		parent.appendChild(node1);
}
function moveDictUp()
{
	var el= document.getElementById("dictlist");
	var uid = el.value;
	if (uid) {
		var a = document.getElementById(uid);
		if (a) {
			if (a.previousSibling)
				swapNode(a, a.previousSibling);
		}
	}
}
function moveDictDown()
{
	var el= document.getElementById("dictlist");
	var uid = el.value;
	if (uid) {
		var a = document.getElementById(uid);
		if (a) {
			if (a.nextSibling)
				swapNode(a, a.nextSibling);
		}
	}
}
function submitDictMask()
{
	var dictmask;
	var el= document.getElementById("dictlist");
	var child;
	var uid;
	if (el.childNodes.length!=0) {
		child = el.childNodes[0];
		uid = child.getAttribute("id");
		dictmask = uid;
	}
	for (var i = 1; i< el.childNodes.length; i++) {
		child = el.childNodes[i];
		uid = child.getAttribute("id");
		dictmask += " ";
		dictmask += uid;
	}
	el = document.getElementById("setdictmask");
	el.value = dictmask;
}
