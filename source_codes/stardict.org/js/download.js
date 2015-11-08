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
	var submitURL="dirinfo.php?parent=" + id + "&showadddict=0";
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
