var mydomain_StarDict_org = "StarDict.net";
var mydomain_www_stardict_org = "www.stardict.net";
var mydomain_www_StarDict_org = "www.StarDict.net";


function addEngine(name,ext,cat,type)
{
	if ((typeof window.sidebar == "object") && (typeof window.sidebar.addSearchEngine == "function")) { 
		window.sidebar.addSearchEngine(
		"http://' + mydomain_www_stardict_org + '/files/"+name+".src",
		"http://' + mydomain_www_stardict_org + '/files/"+name+"."+ext, name, cat);
	} else {
		alert("Sorry, you need a Mozilla-based browser to install a search plugin.");
	}
}
