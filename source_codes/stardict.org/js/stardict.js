var dict_is_ie = true;
var dict_host = './';
var dict_help = './selectandtrans.php';
var dict_username = '';
var dict_md5saltpasswd = '';
var dict_partner = '';
var dict_enable = true;
var dict_old_word = "";
var dict_moving = 0;
var dict_onmove = 0;
var dict_onlayer = 0;
var dict_startx = 0;
var dict_starty = 0;
var dict_cx = 0;
var dict_cy = 0;
var dict_x = 0;
var dict_y = 0;
var dict_layer;
var dict_iframe;

function dictInit(){
var agt = navigator.userAgent.toLowerCase();
  dict_is_ie = (agt.indexOf("msie")!=-1 && document.all);
  var h = '<div id="dict_layer" style="position:absolute;z-index:6000;display:none;background-color:#FFF;';
  if (dict_is_ie) h += 'filter:Alpha(Opacity=96);';
  h += '"><table width="330" cellspacing="0" cellpadding="0" ';
  h += 'style="border-top:1px solid #7E98D6;border-left:1px solid #7E98D6;';
  h += 'border-right:1px solid #7E98D6;border-bottom:1px solid #7E98D6;';
  h += '"><tr><td>';
  h += '<div width="100%" style="cursor:move;background-color:#C8DAF3;border:0px;" onmouseover="dict_onmove=1;" onmouseout="dict_onmove=0;">' ;
  h += '<table width="100%"><tr><td align="left" width="80%" style="background-color:#C8DAF3;">';
  h += '<div style="color:#1A9100;font-size:14px;background-color:#C8DAF3;">Select and Translate - StarDict.org</div>';
  h += '</td>';
  h += '<td align="right" style="background-color:#C8DAF3;">';
  h += '<a href="'+dict_help+'" target="_blank" title="Help">';
  h += '<img src="'+dict_host+'images/help.gif" style="border:none;display:inline;" align="absmiddle" />';
  h += '</a> ';
  h += '<a href="javascript:dictClose()" title="Close">';
  h += '<img src="'+dict_host+'images/close.gif" style="border:none;display:inline;" align="absmiddle" />';
  h += '</a>';
  h += '</td></tr></table>';
  h += '</div>';
  
  h += '<table border="0" cellspacing="4" cellpadding="3" width="100%" align="center" onmouseover="dict_onlayer=1;" onmouseout="dict_onlayer=0;">';
  h += '<tr><td><fieldset color="#00c0ff">';
  h += '<table border="0" cellspacing="0" cellpadding="0" width="100%" align="center">';
  h += '<tr><td width="100%">';
  h += '<iframe id="dictFrame" name="dictFrame" HEIGHT="220" src="about:blank" FRAMEBORDER="0" width="100%"></iframe>';
  h += '</td></tr><tr align="center"><td width="100%" height="20">';
  h += '<font color="#000000" size="1" face="Arial">&copy;2003-2016 ';
  h += '<a href="http://www.stardict.org" target="_blank"><b><font color="#2EA8ED">StarDict.org</font></b></a> On-line Dictionaries.</font>';
  h += '</td></tr></table></fieldset></td></tr></table>';
  h += '</td></tr></table></div>';
  document.write(h);

  dict_layer = document.getElementById('dict_layer');
  dict_iframe = document.getElementById('dictFrame');
  dictClose();
  if (dictRCookie("dictstate") == '1' && dict_enable) dict_enable = false;
  dictUpdateStatus();

  
  if (dict_is_ie) {
    document.attachEvent("onmousemove", dictMove);
    document.attachEvent("ondblclick", dictQuery);
    document.attachEvent("onmouseup", dictQuery);
    document.attachEvent("onselectstart", dictSelect);
    document.attachEvent("onmousedown", dictCheck);
    window.attachEvent("onload", dictUpdateStatus);
  }else {
    document.addEventListener("mousemove", dictMove, true);
    document.addEventListener("dblclick", dictQuery, true);
    document.addEventListener("mouseup", dictQuery, true);
    document.addEventListener("selectstart", dictSelect, true);
    document.addEventListener("mousedown", dictCheck, true);
    window.addEventListener("load", dictUpdateStatus, true);
  }
  var img = new Image();
  img.src = dict_host+"images/loading.gif";
}

function dictGetSel()
{
	if (window.getSelection) return window.getSelection();
	else if (document.getSelection) return document.getSelection();
	else if (document.selection) return document.selection.createRange().text;
	else return '';
}

function dictGetPos(event){
  if (dict_is_ie) {
    dict_x = window.event.clientX + document.documentElement.scrollLeft
      + document.body.scrollLeft;
    dict_y = window.event.clientY + document.documentElement.scrollTop
      + document.body.scrollTop;
  }else {
    dict_x = event.clientX + window.scrollX;
    dict_y = event.clientY + window.scrollY;
  }
}


function dictSelect(e){
    if (dict_moving == 2)
        return false;
    else
        return true;
}

function dictCheck(e) {
    dictGetPos(e);
    var cx = 0;
    var cy = 0;
    var obj = dict_layer;
    if (obj.offsetParent){
        while (obj.offsetParent){
            cx += obj.offsetLeft;
            cy += obj.offsetTop;
            obj = obj.offsetParent;
        }
    }else if (obj.x){
        cx += obj.x;
        cy += obj.y;
    }

    if(dict_moving>0){
        if(dict_onmove == 1){
            dict_moving = 2;
            dict_startx = dict_x;
            dict_starty = dict_y;
        }else if(dict_x < cx || dict_x > (cx + 240) || dict_y < cy || (!dict_onlayer && dict_y > (cy + 100) ) ){
	    dictClose();
        }else{
            
            dict_moving = 1;
        }
    }
    
}

function dictQuery(e)  {
    if(dict_moving == 1){
        if (dict_is_ie) {
            window.event.cancelBubble = true;
            window.event.returnValue = false;
        }else{
            e.preventDefault();
        }
        return false;
    }
    if(dict_moving == 2) {
        dict_cx = dict_x;
        dict_cy = dict_y;
        dict_moving = 1;
        return false;
    }

    dictGetPos(e);
    if (!dict_enable) return true;

    var word = dictGetSel();
    if(dict_is_ie) word=word.replace(/^\s*|\s*$/g,"");
    word=""+word;
    if(word == "" || word.length > 16 || word == dict_old_word) return true;

    dictShow(word);

}

function dictDisplay(){
    var dx=262;
    var dy=264;
    dict_y += 8;
    dict_x += 16;
    if(dict_is_ie){
        if (document.documentElement.offsetHeight && document.body.scrollTop+document.documentElement.scrollTop+document.documentElement.offsetHeight - dict_y < dy){
            dict_y = document.body.scrollTop+document.documentElement.scrollTop + document.documentElement.offsetHeight - dy;
            dict_x += 14;
        }
        if (document.documentElement.offsetWidth && document.body.scrollLeft+document.documentElement.scrollLeft+document.documentElement.offsetWidth - dict_x < dx){
            dict_x = document.body.scrollLeft+document.documentElement.scrollLeft + document.documentElement.offsetWidth - dx;
        }
    }else{
        dx-=1;
        dy+=11;
        if (self.innerHeight && document.body.scrollTop+document.documentElement.scrollTop + self.innerHeight - dict_y < dy) {
            dict_y = document.body.scrollTop+document.documentElement.scrollTop + self.innerHeight - dy;
            dict_x += 14;
        }
        if (self.innerWidth && document.body.scrollLeft+document.documentElement.scrollLeft + self.innerWidth - dict_x < dx) {
            dict_x = document.body.scrollLeft+document.documentElement.scrollLeft + self.innerWidth - dx;
        }
    }
    dict_cx = dict_x;
    dict_cy = dict_y;
    dict_startx = dict_x;
    dict_starty = dict_y;
    dict_layer.style.left = dict_cx+'px';
    dict_layer.style.top = dict_cy+'px';
    dict_layer.style.display="";
    dict_moving = 1;
}

function dictShow(word){
    dictDisplay();
    try{
        dict_iframe.src='about:blank';
        iframeWin = window.frames.dictFrame;
        iframeWin.document.open();
        iframeWin.document.write('<html><body><img src="'+dict_host+'images/loading.gif" /><b><font color="#666666">Querying <font color="green">'+word+'</font>...</b></font></body></html>');
        iframeWin.document.close();
    }catch(x){
    }
    var u=dict_host+'mini.php?';
    if(dict_partner) u += 'id='+dict_partner+'&';
    if (dict_username && dict_md5saltpasswd) u += 'user='+encodeURIComponent(dict_username)+'&' + 'passwd='+encodeURIComponent(dict_md5saltpasswd)+'&';
    u += 'q='+encodeURIComponent(word);
    setTimeout(function(){dict_iframe.src=u;},250);
    dict_old_word = word;
}

function dictMove(e){
    if(dict_moving==2) {
        dictGetPos(e);
        dict_x = dict_x-dict_startx+dict_cx;
        dict_y = dict_y-dict_starty+dict_cy;
        if (document.documentElement.scrollWidth - dict_x < 262) {
            dict_x = document.documentElement.scrollWidth - 262;
        }
        dict_layer.style.left = dict_x+'px';
        dict_layer.style.top = dict_y+'px';
    }
    
}

function dictClose() {
    try
    {
        dict_moving = 0;
        dict_onmove = 0;
        dict_onlayer = 0;
    	dict_layer.style.display="none";
        setTimeout(function(){dict_old_word="";},500);
    	if(window.sf) sf();
    }
    catch (x)
    {
    }

}

function dictWCookie(name,value)
{
    var date=new Date();
    var now=date.getTime();
    date.setTime(now+365*24*60*60*1000);
    document.cookie=name+"="+value+"; path=/; expires="+date.toGMTString();
}

function dictRCookie(name)
{
    var cookie=String(document.cookie);
    var pos=cookie.indexOf(name+"=");
    if(pos!=-1){
        var end=cookie.indexOf("; ",pos);
        return cookie.substring(pos+name.length+1,end==-1?cookie.length:end);
    }
    return "";
}

function dictEnable(){
  if (dict_enable){
    dict_enable = false;
    dictWCookie("dictstate", '1');
  }else{
    dict_enable = true;
    dictWCookie("dictstate", '0');
  }
  dictUpdateStatus();
}

function dictUpdateStatus(){
  var el = document.getElementById('dict_status');
  if(el){
    el.innerHTML = dictStatus();
  }
}

function dictStatus(){
    if (dict_enable){
       return '[<a href="'+dict_help+'" title="Show &quot;Select and Translate&quot; help" target="_blank">Translate</a>: <a href="javascript:dictEnable()" title="Disable &quot;Select and Translate&quot;">Enable</a>]';
    }else{
      return '[<a href="'+dict_help+'" title="Show &quot;Select and Translate&quot; help" target="_blank">Translate</a>: <a href="javascript:dictEnable()" title="Enable &quot;Select and Translate&quot;">Disable</a>]';
    }
}
