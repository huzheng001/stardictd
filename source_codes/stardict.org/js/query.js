function stardict_query(text) {
	url = "query.php?q=" + encodeURIComponent(text);
	window.location.href = url;
}
