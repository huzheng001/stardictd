function addEngine(name,ext,cat,type)
{
	if ((typeof window.sidebar == "object") && (typeof window.sidebar.addSearchEngine == "function")) { 
		window.sidebar.addSearchEngine(
		"http://www.stardict.org/files/"+name+".src",
		"http://www.stardict.org/files/"+name+"."+ext, name, cat);
	} else {
		alert("Sorry, you need a Mozilla-based browser to install a search plugin.");
	}
}
