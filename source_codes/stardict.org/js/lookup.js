function set_focus() {
	document.getElementById("entry").focus();
}
function clear_entry() {
	var entry = document.getElementById("entry");
	entry.value = "";
	entry.focus();
}
var xmlHttp;
function wlist_click(div) {
	xmlHttp = createXMLHttpRequest();
	xmlHttp.onreadystatechange = handleDefineStateChange;
	xmlHttp.open("GET", "define-backend.php?q=" + encodeURIComponent(div.childNodes[0].nodeValue), true);
	xmlHttp.send(null);
}
function handleDefineStateChange() {
	if (xmlHttp.readyState == 4) {
		document.getElementById("definition").innerHTML = xmlHttp.responseText;
	}
}
function doPreviousWord() {
	var pword = document.getElementById("wordlist").childNodes[1].childNodes[0].nodeValue;
	if (pword) {
		xmlHttp = createXMLHttpRequest();
		xmlHttp.onreadystatechange = handlePWordStateChange;
		xmlHttp.open("GET", "previous-backend.php?q=" + encodeURIComponent(pword), true);
		xmlHttp.send(null);
	}
}
function doNextWord() {
	var wordlist = document.getElementById("wordlist");
	var nword = wordlist.childNodes[wordlist.childNodes.length-2].childNodes[0].nodeValue;
	if (nword) {
		xmlHttp = createXMLHttpRequest();
		xmlHttp.onreadystatechange = handleNWordStateChange;
		xmlHttp.open("GET", "next-backend.php?q=" + encodeURIComponent(nword), true);
		xmlHttp.send(null);
	}
}
function handlePWordStateChange() {
	if (xmlHttp.readyState == 4) {
		var wordList = xmlHttp.responseText.split('\n');
		var wordlist = document.getElementById("wordlist");
		var old_elmcount = wordlist.childNodes.length-2;
		for (var w in wordList) {
			var divElmt = document.createElement("div");
			divElmt.onmouseover = function(){this.className='son'};
			divElmt.onmouseout = function(){this.className=''};
			divElmt.onclick = function(){wlist_click(this)};
			divElmt.innerHTML = escapeHTML(wordList[w]);
			wordlist.insertBefore(divElmt, wordlist.childNodes[1]);
		}
		if (old_elmcount > wordList.length) {
			for (var i=0; i< wordList.length; i++)
				wordlist.removeChild(wordlist.childNodes[wordlist.childNodes.length-2]);
		}
	}
}
function handleNWordStateChange() {
	if (xmlHttp.readyState == 4) {
		pcontent = '<div onmouseover="this.className=\'son\'" onmouseout="this.className=\'\'" onclick="doPreviousWord()"><font color="blue">Previous Words</font></div>';
		var wordList = xmlHttp.responseText.split('\n');
		for (var w in wordList) {
			pcontent += '<div onmouseover="this.className=\'son\'" onmouseout="this.className=\'\'" onclick="wlist_click(this)">' + escapeHTML(wordList[w]) + "</div>";
		}
		pcontent += '<div onmouseover="this.className=\'son\'" onmouseout="this.className=\'\'" onclick="doNextWord()"><font color="blue">Next Words</font></div>';
		var wordlist = document.getElementById("wordlist");
		wordlist.innerHTML = pcontent;
	}
}
function analyse_query_type(searchword) {
	if (searchword.charAt(0)=="/") {
		return 1;
	}
	if (searchword.charAt(0)=="|") {
		return 2;
	}
	for (var i=0; i< searchword.length; i++) {
		if (searchword.charAt(i)=="\\") {
			i++;
		} else if (searchword.charAt(i) == "*" || searchword.charAt(i) == "?") {
			return 3;
		}
	}
	return 0;
}
function sendLookupRequest() {
	var searchword = document.getElementById("entry").value;
	xmlHttp = createXMLHttpRequest();
	xmlHttp.onreadystatechange = handleStateChange;
	xmlHttp.open("GET", "lookup-backend.php?q=" + encodeURIComponent(searchword), true);
	xmlHttp.send(null);
}
function do_fuzzy_query() {
	var searchword = document.getElementById("entry").value;
	if (!searchword)
		return;
	var type = analyse_query_type(searchword);
	if (type == 0) {
		xmlHttp = createXMLHttpRequest();
		xmlHttp.onreadystatechange = handleStateChange;
		xmlHttp.open("GET", "lookup-backend.php?q=%2F" + encodeURIComponent(searchword), true);
		xmlHttp.send(null);
	} else {
		sendLookupRequest();
	}
}
var previousWord;
function do_go_back() {
	if (previousWord) {
		var entry = document.getElementById("entry");
		if (entry.value != previousWord) {
			entry.value = previousWord;
			sendLookupRequest();
			previousWord = "";
		}
	}
}
var timeId;
var oldEntryValue;
function on_entry_changed(e) {
	var searchword = document.getElementById("entry").value;
	if (!searchword)
		return;
	if (e.keyCode == 27) {
		previousWord = searchword;
		clear_entry();
		return;
	}
	if (e.keyCode == 13) {
		if (timeId) {
			window.clearTimeout(timeId);
			timeId = false;
		}
		previousWord = searchword;
		sendLookupRequest();
		return;
	}
	if (oldEntryValue == searchword)
		return;
	oldEntryValue = searchword;
	var type = analyse_query_type(searchword);
	if (type == 1) {
		if (searchword.length == 1)
			document.getElementById("definition").innerHTML = "Fuzzy query...";
	} else if (type == 2) {
		if (searchword.length == 1)
			document.getElementById("definition").innerHTML = "Full-text search...";
	} else if (type == 3) {
		document.getElementById("definition").innerHTML = "Press Enter to list the words that match the pattern.";
	} else {
		if (timeId)
			window.clearTimeout(timeId);
		timeId = window.setTimeout(sendLookupRequest, 500);
	}
}
function stardict_query(text) {
	var entry = document.getElementById("entry");
	previousWord = entry.value;
	entry.value = text;
	sendLookupRequest();
}
function handleStateChange() {
	if (xmlHttp.readyState == 4) {
		parseResult();
	} else if (xmlHttp.readyState == 1) {
		document.getElementById("definition").innerHTML = "Loading...";
	}
}
function  escapeHTML(str) {
	var div = document.createElement('div');
	var text = document.createTextNode(str);
	div.appendChild(text);
	return div.innerHTML;
}
function parseResult() {
	var i;
	i = xmlHttp.responseText.indexOf("");
	var definition = document.getElementById("definition");
	if (i == -1) {
		definition.innerHTML = xmlHttp.responseText;
	} else {
		var result_definition = xmlHttp.responseText.slice(0, i);
		definition.innerHTML = result_definition;
		if (xmlHttp.responseText.charAt(i+1) == "d" && xmlHttp.responseText.charAt(i+2) == "\n") {
			var result_list = xmlHttp.responseText.slice(i+3, xmlHttp.responseText.length);
			var pcontent = "<ul class=\"fulltext_list\" style='margin-left:0px; padding-left:0px;'>";
			var dictList = result_list.split('\n');
			for (var d in dictList) {
				var wordList = dictList[d].split('\t');
				pcontent += '<li id="pw';
				pcontent += d;
				pcontent += '"><img src="images/minus.gif" OnClick="showHide(\'pw';
				pcontent += d;
				pcontent += '\')"><font color=\"blue\">';
				pcontent += wordList[0];
				pcontent += '</font><ul>';
				for (var w=1; w<wordList.length; w++) {
					pcontent += '<li onmouseover="this.className=\'son\'" onmouseout="this.className=\'\'" onclick="wlist_click(this)">' + escapeHTML(wordList[w]) + "</li>";
				}
				pcontent += '</ul></li>';
			}
			pcontent += '</ul>';
			var wordlist = document.getElementById("wordlist");
			wordlist.innerHTML = pcontent;
		} else {
			var islist = false;
			if (xmlHttp.responseText.charAt(i+1) == "l" && xmlHttp.responseText.charAt(i+2) == "\n")
				islist = true;
			var pcontent = "";
			var result_list = xmlHttp.responseText.slice(i+3, xmlHttp.responseText.length);
			var wordList = result_list.split('\n');
			if (islist)
				pcontent += '<div onmouseover="this.className=\'son\'" onmouseout="this.className=\'\'" onclick="doPreviousWord()"><font color="blue">Previous Words</font></div>';
			for (var w in wordList) {
				pcontent += '<div onmouseover="this.className=\'son\'" onmouseout="this.className=\'\'" onclick="wlist_click(this)">' + escapeHTML(wordList[w]) + "</div>";
			}
			if (islist)
				pcontent += '<div onmouseover="this.className=\'son\'" onmouseout="this.className=\'\'" onclick="doNextWord()"><font color="blue">Next Words</font></div>';
			var wordlist = document.getElementById("wordlist");
			wordlist.innerHTML = pcontent;
		}
	}
}
function showInfo() {
	document.getElementById("definition").innerHTML = "<center>Welcome to <b>www.StarDict.org</b></center><br>This website is the on-line version of <a href=\"http://stardict.sourceforge.net\" target=\"_blank\">StarDict</a>.<br>StarDict is a Cross-Platform and international dictionary written in Gtk2.<br>It has powerful features such as \"Glob-style pattern matching,\" \"Scan selected word,\" \"Fuzzy query,\" etc.<br><br>Here is an introduction to using StarDict:<br><br><b>1. Glob-style pattern matching</b><br>You can input strings containing \'*\' (wildcard) and \'?\' (joker) as the pattern. \'*\' matches an arbitrary, possibly empty, string, and \'?\' matches an arbitrary character. After pressing Enter, the words that match this pattern will be shown in the list.<br><b>2. Fuzzy query</b><br>When you can't remember how to spell a word exactly, you can try StarDict's Fuzzy query. It uses \"Levenshtein Edit Distance\" to compute the similarity between two words, and gives the match results which are most similar to the word that you input. To create a fuzzy query, just input the word with a beginning \"/\", and then press Enter.<br><b>3. Full-text search</b><br>Full-text search searchs a word in the data. To create a Full-text search, just input the word with a beginning \"|\", and then press Enter. For example, \"|ab cd\" searchs data which contain both \"ab\" and \"cd\". If the words contain Space character, you can use \"\\ \", such as \"|apple\\ pie\", other escaping characters are \"\\\\\" for \'\\\', \"\\t\" for Tab and \"\\n\" for new line.<br><b>4. Special character search</b><br>If your words contain special characters, you can use \'\\\' to escape it, for example, \"a\\*b\\?\" searchs \"a*b?\", \"\\/abc\" searchs \"/abc\".<br><br>Any question or suggestion, just mail to <a href=\"mailto:huzheng001@gmail.com\">huzheng001@gmail.com</a>."
}
