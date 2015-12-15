function _dictInit(){
  var agt = navigator.userAgent.toLowerCase();
  _dict_is_ie = (agt.indexOf("msie")!=-1 && document.all);
  var h = '<table width="330" cellspacing="0" cellpadding="0" ';
  h += 'style="border-top:1px solid #7E98D6;border-left:1px solid #7E98D6;';
  h += 'border-right:1px solid #7E98D6;border-bottom:1px solid #7E98D6;';
  h += '"><tr><td>';
  h += '<div width="100%" style="cursor:move;background-color:#C8DAF3;border:0px;" onmouseover="_dict_onmove=1;" onmouseout="_dict_onmove=0;">' ;
  h += '<table width="100%"><tr><td align="left" width="80%" style="background-color:#C8DAF3;">';
  h += '<div style="color:#1A9100;font-size:14px;background-color:#C8DAF3;">Select and Translate - StarDict.org</div>';
  h += '</td>';
  h += '<td align="right" style="background-color:#C8DAF3;">';
  h += '<a href="'+_dict_help+'" target="_blank" title="Help">';
  h += '<img src="'+_dict_host+'images/help.gif" style="border:none;display:inline;" align="absmiddle" />';
  h += '</a> ';
  h += '<a href="javascript:_dictClose()" title="Close" target="_self">';
  h += '<img src="'+_dict_host+'images/close.gif" style="border:none;display:inline;" align="absmiddle" />';
  h += '</a>';
  h += '</td></tr></table>';
  h += '</div>';
  
  h += '<table border="0" cellspacing="4" cellpadding="3" width="100%" align="center" onmouseover="_dict_onlayer=1;" onmouseout="_dict_onlayer=0;">';
  h += '<tr><td><fieldset color="#00c0ff">';
  h += '<table border="0" cellspacing="0" cellpadding="0" width="100%" align="center">';
  h += '<tr><td width="100%">';
  h += '<iframe id="_dictFrame" name="_dictFrame" HEIGHT="220" src="about:blank" FRAMEBORDER="0" width="100%"></iframe>';
  h += '</td></tr><tr align="center"><td width="100%" height="20">';
  h += '<font color="#000000" size="1" face="Arial">&copy;2003-2006 ';
  h += '<a href="http://www.stardict.org" target="_blank"><b><font color="#2EA8ED">StarDict.org</font></b></a> On-line Dictionaries.</font>';
  h += '</td></tr></table></fieldset></td></tr></table>';
  h += '</td></tr></table>';
  try{
    var el = document.createElement('div');
    el.id='_dict_layer';
    el.style.position='absolute';
    el.style.display='none';
    el.style.zIndex=9000;
    el.style.backgroundColor='#FFF';
    el.style.filter='Alpha(Opacity=96)';

    document.body.appendChild(el);
    _dictSet(el, h);
    
    el = document.createElement('div');
    el.id='_dict_status';
    el.style.position='absolute';
    el.style.backgroundColor='#e7f7f7';
    el.style.padding='2px';
    el.style.filter='Alpha(Opacity=96)';
    el.style.fontSize='14px';
    el.style.left = '3px';
    el.style.top = '3px';
    el.style.zIndex=9000;
    el.style.border = '1px solid #7E98D6';
    document.body.appendChild(el);
    _dictSet(el, _dictStatus());
  }catch(x){
    alert("dict can not support this page.");
    alert(x);
    _dict_layer = true;
    return;
  }
  _dictClose();

  
  if (_dict_is_ie) {
    document.attachEvent("onmousemove", _dictMove);
    document.attachEvent("ondblclick", _dictQuery);
    document.attachEvent("onmouseup", _dictQuery);
    document.attachEvent("onselectstart", _dictSelect);
    document.attachEvent("onmousedown", _dictCheck);
    window.attachEvent("onload", _dictUpdateStatus);
  }else {
    document.addEventListener("mousemove", _dictMove, true);
    document.addEventListener("dblclick", _dictQuery, true);
    document.addEventListener("mouseup", _dictQuery, true);
    document.addEventListener("selectstart", _dictSelect, true);
    document.addEventListener("mousedown", _dictCheck, true);
    window.addEventListener("load", _dictUpdateStatus, true);
  }
  //var img = new Image();
  //img.src = _dict_host+"images/loading.gif";
  _dict_layer = document.getElementById('_dict_layer');
  _dict_status = document.getElementById('_dict_status');
  _dict_iframe = document.getElementById('_dictFrame');

}

function _dictSet(el, htmlCode) {
    var ua = navigator.userAgent.toLowerCase();
    if (ua.indexOf('msie') >= 0 && ua.indexOf('opera') < 0) {
        el.innerHTML = '<div style="display:none">for IE</div>' + htmlCode;
        el.removeChild(el.firstChild);
    }
    else {
        var el_next = el.nextSibling;
        var el_parent = el.parentNode;
        el_parent.removeChild(el);
        el.innerHTML = htmlCode;
        if (el_next) {
            el_parent.insertBefore(el, el_next)
        } else {
            el_parent.appendChild(el);
        }
    }
}

function _dictGetSel()
{
	if (window.getSelection) return window.getSelection();
	else if (document.getSelection) return document.getSelection();
	else if (document.selection) return document.selection.createRange().text;
	else return '';
}

function _dictGetPos(event){
  if (_dict_is_ie) {
    _dict_x = window.event.clientX + document.documentElement.scrollLeft
      + document.body.scrollLeft;
    _dict_y = window.event.clientY + document.documentElement.scrollTop
      + document.body.scrollTop;
  }else {
    _dict_x = event.clientX + window.scrollX;
    _dict_y = event.clientY + window.scrollY;
  }
}


function _dictSelect(e){
    if (_dict_moving == 2)
        return false;
    else
        return true;
}

function _dictCheck(e) {
    _dictGetPos(e);
    var cx = 0;
    var cy = 0;
    var obj = _dict_layer;
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

    if(_dict_moving>0){
        if(_dict_onmove == 1){
            _dict_moving = 2;
            _dict_startx = _dict_x;
            _dict_starty = _dict_y;
        }else if(_dict_x < cx || _dict_x > (cx + 240) || _dict_y < cy || (!_dict_onlayer && _dict_y > (cy + 100) ) ){
	    _dictClose();
        }else{
            
            _dict_moving = 1;
        }
    }
    
}

function _dictQuery(e)  {
    if(_dict_moving == 1){
        if (_dict_is_ie) {
            window.event.cancelBubble = true;
            window.event.returnValue = false;
        }else{
            e.preventDefault();
        }
        return false;
    }
    if(_dict_moving == 2) {
        _dict_cx = _dict_x;
        _dict_cy = _dict_y;
        _dict_moving = 1;
        return false;
    }

    _dictGetPos(e);
    if (!_dict_enable) return true;

    var word = _dictGetSel();
    if(_dict_is_ie) word=word.replace(/^\s*|\s*$/g,"");
    word=""+word;
    if(word == "" || word.length > 16 || _dict_old_word == word) return true;

    _dictShow(word);

}

function _dictDisplay(){
    var dx=262;
    var dy=264;
    _dict_y += 8;
    _dict_x += 16;
    if(_dict_is_ie){
        if (document.documentElement.offsetHeight && document.body.scrollTop+document.documentElement.scrollTop+document.documentElement.offsetHeight - _dict_y < dy){
            _dict_y = document.body.scrollTop+document.documentElement.scrollTop + document.documentElement.offsetHeight - dy;
            _dict_x += 14;
        }
        if (document.documentElement.offsetWidth && document.body.scrollLeft+document.documentElement.scrollLeft+document.documentElement.offsetWidth - _dict_x < dx){
            _dict_x = document.body.scrollLeft+document.documentElement.scrollLeft + document.documentElement.offsetWidth - dx;
        }
    }else{
        dx-=1;
        dy+=11;
        if (self.innerHeight && document.body.scrollTop+document.documentElement.scrollTop + self.innerHeight - _dict_y < dy) {
            _dict_y = document.body.scrollTop+document.documentElement.scrollTop + self.innerHeight - dy;
            _dict_x += 14;
        }
        if (self.innerWidth && document.body.scrollLeft+document.documentElement.scrollLeft + self.innerWidth - _dict_x < dx) {
            _dict_x = document.body.scrollLeft+document.documentElement.scrollLeft + self.innerWidth - dx;
        }
    }
    _dict_cx = _dict_x;
    _dict_cy = _dict_y;
    _dict_startx = _dict_x;
    _dict_starty = _dict_y;
    _dict_layer.style.left = _dict_cx+'px';
    _dict_layer.style.top = _dict_cy+'px';
    _dict_layer.style.display="";
    _dict_moving = 1;
}
function _dictShow(word){
    _dictDisplay();
    try{
        _dict_iframe.src='about:blank';
        var iframeWin = window.frames['_dictFrame'];
        iframeWin.document.open();
        iframeWin.document.write('<html><body><img src="'+_dict_host+'images/loading.gif" /><span style="color:#666666;font-weight:bold;">Querying </span><span style="color:green;font-weight:bold;">'+word+'</span>...</body></html>');
        iframeWin.document.close();
    }catch(x){
    }
    
    var u= _dict_host+'mini.php?';
    if (_dict_username && _dict_md5saltpasswd) u += 'user='+encodeURIComponent(_dict_username)+'&' + 'passwd='+encodeURIComponent(_dict_md5saltpasswd)+'&';
    u += 'q='+encodeURIComponent(word);
    setTimeout(function(){_dict_iframe.src=u;},200);
    _dict_old_word = word;
}

function _dictMove(e){
    if(_dict_moving==2) {
        _dictGetPos(e);
        _dict_x = _dict_x-_dict_startx+_dict_cx;
        _dict_y = _dict_y-_dict_starty+_dict_cy;
        if (document.documentElement.scrollWidth - _dict_x < 262) {
            _dict_x = document.documentElement.scrollWidth - 262;
        }
        _dict_layer.style.left = _dict_x+'px';
        _dict_layer.style.top = _dict_y+'px';
    }
    
}

function _dictClose() {
    try
    {
        _dict_moving = 0;
        _dict_onmove = 0;
        _dict_onlayer = 0;
    	_dict_layer.style.display="none";
        setTimeout(function(){_dict_old_word = "";},500);
    }
    catch (x)
    {
    }

}

function _dictRemove() {
    try
    {
        _dict_moving = 0;
        _dict_onmove = 0;
        _dict_onlayer = 0;
    	_dict_enable = false;
    	_dict_layer.style.display="none";
    	_dict_status.style.display="none";
    }
    catch (x)
    {
    }

}

function _dictEnable(){
  if (_dict_enable){
    _dict_enable = false;
  }else{
    _dict_enable = true;
  }
  _dictUpdateStatus();
}

function _dictUpdateStatus(){
  _dictSet(_dict_status, _dictStatus());
}

function _dictStatus(){
    var h='';
    if (_dict_enable){
     h += '[<a href="'+_dict_help+'" title="Show &quot;Select and Translate&quot; help" target="_blank" style="color:#1A9100;font-size:14px;background-color:#e7f7f7;">Translate</a>: <a href="javascript:_dictEnable()" title="Disable &quot;Select and Translate&quot;" target="_self" style="color:#1A9100;font-size:14px;background-color:#e7f7f7;">Enable</a>]';
    }else{
      h += '[<a href="'+_dict_help+'" title="Show &quot;Select and Translate&quot; help" target="_blank" style="color:#1A9100;font-size:14px;background-color:#e7f7f7;">Translate</a>: <a href="javascript:_dictEnable()" title="Enable &quot;Select and Translate&quot;" target="_self" style="color:#1A9100;font-size:14px;background-color:#e7f7f7;">Disable</a>]';
    }
    h +=' <a href="javascript:_dictRemove();" target="_self"><img src='+_dict_host+'images/close.gif border=0 align=absmiddle /></a>';
    return h;
}
if(! document.getElementById('_dict_layer')){
var _dict_is_ie = true;
var _dict_host = 'http://www.stardict.org/';
var _dict_help = "http://www.stardict.org/selectandtrans.php";
<?php
$username = isset($_REQUEST['user'])?$_REQUEST['user']:"";
$password = isset($_REQUEST['passwd'])?$_REQUEST['passwd']:"";
if ($username!="" && $password!="") {
	echo "var _dict_username = '";
	echo $username;
	echo "';\n";
	echo "var _dict_md5saltpasswd = '";
	echo $password;
	echo "';\n";
} else {
	echo "var _dict_username = '';\n";
	echo "var _dict_md5saltpasswd = '';\n";
}
?>
var _dict_old_word = "";
var _dict_moving = 0;
var _dict_onmove = 0;
var _dict_onlayer = 0;
var _dict_startx = 0;
var _dict_starty = 0;
var _dict_cx = 0;
var _dict_cy = 0;
var _dict_x = 0;
var _dict_y = 0;
var _dict_enable = true;
var _dict_layer;
var _dict_status;
var _dict_iframe;
_dictInit();
}else{
    try{
    _dict_enable = true;
    _dict_status.style.display="";
    _dictUpdateStatus();
    }catch(x){;}
}
dict_enable = false;
