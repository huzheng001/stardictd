var maxZ;
var topapp = null;

function Setup (win)
{	
   topapp = xGetElementById (win);
   if (ele = xGetElementById (win + 'DBar')) {
      xEnableDrag (ele, toTop, onDrag, endOp);
      if (ele = xGetElementById (win + 'DDow'))
         xEnableDrag (ele, toTop, onDrag, endOp);
      if (ele = xGetElementById (win + 'RBtn'))
         xEnableDrag (ele, toTop, onResize, endOp);
      if (ele = xGetElementById (win + 'MBtn'))
         xGetElementById (ele).onclick = MBtnOnClick;
   } else
      xEnableDrag (win, toTop, null, endOp);
   xShow (win);
   dbgMessage (win + ' at '+xLeft(win)+', '+xTop(win) + ' z = ' + xZIndex(win));
}

function toTop (ele, mx, my)
{
   var e1 = xGetElementById (ele.id.substring (0, ele.id.length-4));
   ele = e1 ? e1 : ele;
  
   if (topapp != ele) {
      xZIndex (topapp = ele, ++maxZ);
      dbgMessage ("toTop ("+ ele.id+") = " + maxZ);
   }
}

function onDrag (ele, dx, dy)
{
   var e1 = xGetElementById (ele.id.substring (0, ele.id.length-4));
   ele = e1 ? e1 : ele;
   
   xLeft (ele, xLeft(ele) + dx);
   xTop (ele, xTop(ele) + dy);
}


function endOp (ele)
{
   var e1 = xGetElementById (ele.id.substring (0, ele.id.length-4));
   ele = e1 ? e1 : ele;

   dbgMessage ("endOp ("+ele.id + ") at "+ xLeft(ele) + ', ' + xTop(ele));	
   sendWindowPos (ele.id, xLeft (ele), xTop (ele), xWidth (ele), xHeight (ele), xZIndex (ele));
}


function onResize (ele, mdx, mdy)
{
   var e1 = xGetElementById (ele.id.substring (0, ele.id.length-4));
   ele = e1 ? e1 : ele;

   if (xWidth (ele) > 200 && xHeight (ele) > 100)
    xResizeTo (ele, xWidth (ele) + mdx, xHeight (ele) + mdy);
   else
    xResizeTo (ele, 205, 105);
   //StarDict hack.
   document.getElementById("wordlist").style.height = (xHeight(ele)-98)+'px';
   document.getElementById("definition").style.height = (xHeight(ele)-98)+'px';
}

function MBtnOnClick (ev)
{
//  fixE (ev);
  var id = xGetElementById (ev.currentTarget.id.substring (0, ev.currentTarget.id.length-4));
  
  if (id.maximized) {
    id.maximized = false;
    xResizeTo(id, id.prevW, id.prevH);
    xMoveTo(id, id.prevX, id.prevY);
  }
  else {
    id.prevW = xWidth(id);
    id.prevH = xHeight(id);
    id.prevX = xLeft(id);
    id.prevY = xTop(id);
    xMoveTo(id, xScrollLeft(), 5);
    id.maximized = true;
    xResizeTo(id, xClientWidth(), xClientHeight() - 10);
  }
   //StarDict hack.
   document.getElementById("wordlist").style.height = (xHeight(id)-98)+'px';
   document.getElementById("definition").style.height = (xHeight(id)-98)+'px';
}

function dbgMessage (msg) {
   if (dbg = xGetElementById ('eyeOSdbg')) {
      dbg.innerHTML = msg + '<br />' + dbg.innerHTML;
   }
}

var IEreqTypes = [
    'Microsoft.XMLHTTP',
    'MSXML2.XMLHTTP.5.0',
    'MSXML2.XMLHTTP.4.0',
    'MSXML2.XMLHTTP.3.0',
    'MSXML2.XMLHTTP' ];
    
function newReq () {
  var
    req = null;

  if (window.XMLHttpRequest) // Mozilla, Safari,...
    req = new XMLHttpRequest();
  else if (window.ActiveXObject) { // IE
    var i = IEreqTypes.length; 
    while (!req && i--) {
      try {
        req = new ActiveXObject (IEreqTypes[i]);
      } catch (e) {
//      alert (IEreqTypes[i] + ' : ' + e.name + ' - ' + e.message);
        req = null;
      }
      if (req == null) {
        if (! confirm ('Unable to create HTTP request object\n'+
        'For Microsoft IE browsers this is most likely due to a security check\n'+
        '\nThe target URL may be set to trusted in the Internet Options/Security/Trusted\n'+
        'sites list\n   or ActiveX controls may be enabled\n'+ 
        "\nClick 'OK' to retry, 'Cancel' to exit"))
          return null;
        i = IEreqTypes.length;   
      }
    }
  }

  if (!req)
    alert ('Failed to create HTTP request object');

  return req;
}

var movereport = false;
function sendWindowPos (app, x, y, w, h, z) {
   while (movereport) {
   
   }

   if (movereport = newReq ()) {  
      movereport.onreadystatechange = clearMoveReq;
      movereport.open ("GET", "desktop.php?pos="+app+"&x="+x+"&y="+y+"&w="+w+"&h="+h+"&z="+z, true);
      movereport.send ("");
   } 
}

function clearMoveReq () {
   if (movereport.readyState == 4)
      movereport = false;
}


function sysCall (module, action,  msg, callback, p, sync) {
//alert ('sysCall (' + module + ', ' + action + ', ' + msg + ', ' + callback + ', ' + p + ', ' + sync + ')');
  var syscallreq;

  if (sync !== false) sync = true;
  
  if (syscallreq = newReq ()) {
    syscallreq.onreadystatechange = function () { 
      (syscallreq.readyState == 4) && callback && callback (syscallreq, p);
    };

    syscallreq.open ("GET", "desktop.php?syscall=[" + module + "][" + action + "][" + msg + "]", sync);
    syscallreq.send ("");
    
    !sync && callback && callback (syscallreq, p);
  }
}


function ipc (app, msg, action, param, sync) {

//  alert ('ipc (' + app + ', ' + msg +', ' + action + ', ' + param + ', ' + sync +')');
  
  if (typeof app != 'string') {
    while (app && (!app.className || !app.className.match(/(^|\s+)eyeapp(\s+|$)/)))
      app = app.parentNode;
  }
  else
    app = document.elementById (app);
  
  app && sysCall ('sys', 'app', app.id + '][ipc][' + msg, action, param || app, sync);
}

function clearSysCallReq () {
   if (syscallreq.readyState == 4)
      syscallreq = false;
}


function addEvent(obj, evType, fn, useCapture){
  if (obj.addEventListener){
    obj.addEventListener(evType, fn, useCapture);
    return true;
  } else if (obj.attachEvent){
    var r = obj.attachEvent("on"+evType, fn);
    return r;
  } else {
    alert("Handler could not be attached");
  }
}

function removeEvent(obj, evType, fn, useCapture){
  if (obj.removeEventListener){
    obj.removeEventListener(evType, fn, useCapture);
    return true;
  } else if (obj.detachEvent){
    var r = obj.detachEvent("on"+evType, fn);
    return r;
  } else {
    alert("Handler could not be removed");
  }
}

function getElementsByClass (node, searchClass, tag) {
//  alert ('getElementsByClass ('+node+', '+searchClass+', '+tag+')');
  
  if (typeof node == 'string') { // no node specified
    tag = searchClass;
    searchClass = node;
    node = document;
  }
   
  var 
    classElements = new Array (),
    els = node.getElementsByTagName (tag || '*'),
    pattern = new RegExp ("(^|\\s)("+searchClass+")(\\s|$)",'i');
     
  for (var j = 0, i = els.length; i--;)
    if (pattern.test (els[i].className))
      classElements[j++] = els[i];
  
  return classElements;
}


function toggleElementDisplay (ele) {
  (typeof ele == 'string') && (ele = getElementById (ele));
  with (ele.style) {
    if (display == 'none')
      display = prevDisplay || '';
    else
      ele.style.prevDisplay = display, display = 'none';
  }
}


function parseAttribute (ele, attrib) {

// alert ('parseAttribute (' + ele + ', ' + attrib + ')'); 
  
  if (arguments.length == 2) {
    if (typeof ele =='string') ele= document.getElementById (ele);
    attrib = ele.getAttribute (attrib);
  }
  else
    attrib = ele;
  
  var a = {};
//var s = attrib + '\n\n'; 
  attrib = attrib.split (/\s*;\s*/);
  for (var i = attrib.length; i--;)
    if (attrib[i]) {
      var av = attrib[i].match (/^([a-z0-9-_]+)(:(.*))?$/i);
      av && av.length && (a[av[1]] = (av.length > 2) ? av[3] : true);
    }

//for (var i in a) s += i + '=' + a[i] + ';\n'; alert (s);      
  return a;
}  


function appParam (ele, param, value) {

//  alert (arguments.length + ' appParam (' + ele + ', ' + param + ', ' + value + ')');
  
  if ((arguments.length == 2) && ele.responseText) {
    ele = ele.responseText.split ('://:');
    param.appParams = {};
    for (var i = ele.length; i--;) {
      ele[i] = parseAttribute (ele[i]);
      param.appParams[ele[i].name] = ele[i];
      if (ele[i].type)
        switch ((ele[i].type).toLowerCase ()) {
        case 'bool':  
          param.appParams[ele[i].name].value = ele[i].value === '1';
          break;
        case 'num':
          param.appParams[ele[i].name].value = parseInt (ele[i].value, 10);
          break;
        }
    }
//    ele = param; var s = ''; for (var i in ele.appParams) with (ele.appParams[i]) s += name + '=' + value + '\n'; alert (s);
    return;
  }

  (typeof ele == 'string') && (ele = document.getElementById (ele));
  while (ele && (!ele.className || !ele.className.match(/(^|\s)eyeApp(\s|$)/i)))
    ele = ele.parentNode;

  var undef;
  if (!ele) return undef; 

  !ele.appParams && sysCall ('sys', 'app', ele.id + '][getparams]', appParam, ele, false);

  if ((typeof value != 'undefined') && ele.appParams[param] && (value != ele.appParams[param].value)) {
     ele.appParams[param].value = value;
     sysCall ('sys', 'app', ele.id + '][setparams][' + param + '=' + value +']');
  }
  
  return ele.appParams[param] ? ele.appParams[param].value : undef;
}


function editParams (req, ele) {
  var
    app,
    state;
  
  if (arguments.length == 1) {
    ele = req;
    if (ele.tagName == 'BUTTON') {
      while (ele && (!ele.className || !ele.className.match(/(^|\s)(eyeApp|eyeConfig)(\s|$)/i)))
        ele = ele.parentNode;
      ele && (state = ele.state);
    }
    else
      state = 0;
  }
  else {
    app = ele.app,
    state = ele.state;
  }
  
//  alert ('editParams (' + req + ',' + ele+ ') - ' + state);  
  if (!ele) return;

  switch (state) {
  case 0 : // Request param table from server
//    alert ('editParams 0 : ' + ele);  
      while (ele && (!ele.className || !ele.className.match(/(^|\s)eyeApp(\s|$)/i)))
        ele = ele.parentNode;
      app = ele;
      ele && (ele = getElementsByClass (ele, 'eyeConfig')[0]);

    if (!ele || !app)
      return;

    if (ele.style.display != 'none') {
      ele.style.display = 'none';
      ele.innerHTML = '';
      return;
    }

    ele.app = app;
    if (!app.appParams) {
      ele.state = 1;
      sysCall ('sys', 'app', app.id + '][getparams', editParams, ele);
      return;
    }
    
  case 1:  // build param edit panel
//    alert ('editParams ' + state + ' : ' + ele);  
    var p;
    if (ele.state == 1) {
      p = req.responseText.split ('://:');
      app.appParams = {};
      for (var i = p.length; i--;) {
        p[i] = parseAttribute (p[i]);
        app.appParams[p[i].name] = p[i];
      }
    }  
      
    var ps = '<table>';
    for (p in app.appParams) {
      if (!(p = app.appParams[p]).label)
        continue;
      
      ps += '<tr><td align="right">'+
        (p.help ? '<a class="helplink" href="?a=eyeHelp('+app.id + ',,'+p.help+')">?</a>' : '') +
        (p.required ? '*' : '') + (p.restart ? '!': '') +' '+ p.label.replace ('\\n', '<br/>') +
        "</td><td> : </td><td>";
      switch (p.type) {
      case 'bool':
        (typeof p.value == 'string') && (p.value = (p.value != '0'));
        ps += "<input type='checkbox' name='"+p.name+"'"+ (p.value ? " checked=1 defaultChecked=1>" : ">");
        break;
          
      case 'sel':
         ps += '<select name="'+p.name+'">';
         var sv = p.selectors.split (',').reverse ();
         for (var i = sv.length; i--;) {
           var pss = sv[i].split ('=');
           ps += '<option value="'+ (pss[1] || pss[0]) + '"' + ((p.value == (pss[1] || pss[0])) ? ' selected' : '') + '>' + pss[0];
         }
         ps += '</select>';
         break;
         
      case 'int':
        ps += "<input type='text' size='5' value='"+p.value+"' name='"+p.name+"'>";
        break;
        
      case 'str':
      default:
        ps += "<input type='text' size='30' value='"+p.value+"' name='"+p.name+"'>";
      }       
      ps+= '</td></tr>';
    }
    ps += '<tr valign="bottom"><td></td><td></td>'+ 
      '<td align="right"><button onclick="editParams(this)" bvalue="1">Save</button>&nbsp;'+
      '<button onclick="editParams(this)" bvalue="0">Cancel</button></td></tr></table>';
      
    ele.innerHTML = ps;
    ele.style.display = 'block';
    ele.state = 2;
    return;
    
  case 2:// param edit panel button pressed
    app = ele.app;
    if (req.getAttribute('bvalue') == 1) { // update and send parameters to server
      var
        pv = '',
        ps = ele.getElementsByTagName ('SELECT');
      
      for (var i = ps.length; i--;) with (ps[i]) {
          var 
            p = app.appParams[name],
            v = p.selectors.split(',')[selectedIndex].split ('=');
            
          if (p.value != (v = (v[1] || v[0]))) {  
            appParam (app, p.name, v);
            pv += '://:' + p.name + '=' + v;
          }
      }
    
      ps = ele.getElementsByTagName ('INPUT');
      for (var i = ps.length; i--;) with (ps[i])
        if (((type == 'checkbox') && (checked != defaultChecked)) ||
            ((type != 'checkbox') && (value != defaultValue))) {
          appParam (app, name, (type == 'checkbox') ? checked : value);
          pv += '://:' + name + '=' + ((type == 'checkbox') ? (checked ? '1' : '0'): value);
        }  
      
      if (pv) {
//      alert ('Updating parameters : ' + pv.slice(4));
        sysCall ('sys', 'app', app.id + '][setparams][' + pv.slice(4) +']', editParams, ele);
        ele.state = 3;
        ele.updateAction && ele.updateAction (); 
        return;
      }
    }
    
  case 3:
    if ((state == 3) && (req.responseText.substring (0,2) != 'OK'))
        alert (req.responseText);

    ele.style.display = 'none';
    ele.innerHTML = '';
    ele.state = 0;
    return;
  }  
}


function closeApp (w) {
  while (w && (!w.className || !w.className.match(/(^|\s+)eyeapp(\s+|$)/)))
    w = w.parentNode;
  
  if (w) {
    sysCall ('sys', 'wclose', w.id, null, null, false);
    w.parentNode.removeChild (w);
  }

  var appid = w.id;  
  w = getElementsByClass (document, 'APPICON');
  for (var i = w.length; i--;)
    if (w[i].className.indexOf (appid) >= 0) { 
      if (w[i].style.filter)
        w[i].style.filter =  w[i].style.filter.replace ('ico_b', 'ico_c');
      else
        w[i].src = w[i].src.replace ('ico_b', 'ico_c');
    }
}


function closeAll () {
    sysCall ('sys', 'wclose', '*', null, null, false);
    window.location = "index.php";
}


function restoreApp (req, a) {
//alert ('restoreApp (' + req + ', ' + a + ')');

  if (arguments.length == 1) {
    sysCall ('sys', 'wrestore', req, restoreApp, req);
    return;
  }
    
  var attribs = parseAttribute (req.responseText);

  if (attribs.error) {
    alert (attribs.error);
    return;
  }
  
  var apps = getElementsByClass (document, 'eyeapp');
  for (var i = apps.length; i--;) 
    if (a == apps[i].id) {
      a = apps[i];
      a.style.left = attribs.x || 100;
      a.style.top = attribs.y || 100;
      xResizeTo (a, parseInt (attribs.w, 10), parseInt (attribs.h, 10));
//      alert ('restoreApp : ' + a + ' - ' + attribs.x + ', ' + attribs.y + ', ' + attribs.w + ', ' +  attribs.h + ' => ' + xWidth(a) + ', ' + xHeight(a));
    }
}
