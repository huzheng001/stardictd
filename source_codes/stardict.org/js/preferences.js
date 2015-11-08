var xmlHttp;
function apply_collate_func() {
	var select = document.getElementById("collatefunc");
	xmlHttp = createXMLHttpRequest();
	xmlHttp.onreadystatechange = handleCollateStateChange;
	xmlHttp.open("GET", "collatefunc-backend.php?f=" + select.value, true);
	xmlHttp.send(null);
}
function handleCollateStateChange() {
	if (xmlHttp.readyState == 4) {
		document.getElementById("collatefunc_info").innerHTML = xmlHttp.responseText;
	} else if (xmlHttp.readyState == 1) {
		document.getElementById("collatefunc_info").innerHTML = "Loading...";
	}
}
function apply_language() {
	var select = document.getElementById("language");
	xmlHttp = createXMLHttpRequest();
	xmlHttp.onreadystatechange = handleLanguageStateChange;
	xmlHttp.open("GET", "language-backend.php?l=" + select.value, true);
	xmlHttp.send(null);
}
function handleLanguageStateChange() {
	if (xmlHttp.readyState == 4) {
		if (xmlHttp.responseText.charAt(0))
			alert(xmlHttp.responseText);
		else
			window.location.reload(true);
	}
}
