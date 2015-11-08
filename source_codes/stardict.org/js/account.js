var xmlHttp;
function update_email() {
	var email = document.getElementById("email");
	xmlHttp = createXMLHttpRequest();
	xmlHttp.onreadystatechange = handleEmailStateChange;
	xmlHttp.open("GET", "email-backend.php?e=" + email.value, true);
	xmlHttp.send(null);
}
function handleEmailStateChange() {
	if (xmlHttp.readyState == 4) {
		document.getElementById("update_info").innerHTML = xmlHttp.responseText;
	} else if (xmlHttp.readyState == 1) {
		document.getElementById("update_info").innerHTML = "Loading...";
	}
}
