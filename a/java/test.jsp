
<html>
<head>
	<title>Detecting browser close in IE</title>


	<script type="text/javascript">

		var myclose = false;

		function ConfirmClose()
		{
			if (event.clientY < 0)
			{
				event.returnValue = 'Any message you want';

				setTimeout('myclose=false',100);
				myclose=true;
			}
		}

		function HandleOnClose()
		{
			if (myclose==true)
            alert("Window is closed");
		}

	</script>
</head>


<body onbeforeunload="ConfirmClose()" onunload="HandleOnClose()">

<h4>Close browser!</h4>

</body>
</html>


[windowVar = ][window].open("URL", "windowName", ["windowFeatures"])Where windowFeatures is a comma-separated list of any of the following options and values: 

   toolbar[=yes|no]|[=1|0]
   location[=yes|no]|[=1|0]
   directories[=yes|no]|[=1|0]
   status[=yes|no]|[=1|0]
   menubar[=yes|no]|[=1|0]
   scrollbars[=yes|no]|[=1|0]
   resizable[=yes|no]|[=1|0]
   width=pixels
   height=pixels

window.open('WebForm2.aspx','_new','toolbar=no,menubar=no'); 

// replace Method 
// The replace method replaces the current History entry with the specified URL.
// After calling the replace method, you cannot navigate back to the previous URL
// using the browser's Back button. 

// Syntax: location.replace(URL) 

//This will replace the history with the current link every time a link is clicked
//<BLOCKQUOTE><font size="1" face="Verdana, Arial">code/font><HR><pre>
//&lt;SCRIPT&gt;

function getTag(el,str) {
while ((el!=null) && (str.indexOf(el.tagName + ":")<0))
el = el.parentElement
return el
}

function navigateTo(sURL,target) {
if ((target == '_self') || (target=="")) {
window.location.replace(sURL);
return false;
}
if (target == '_top') {
top.window.location.replace(sURL);
return false
}
if (target =='_parent') {
parent.window.location.replace(sURL);
return false;
}
if (target == '_blank' || parent.frames.length < 1) {
window.open(sURL, target);
return false;
}
else {
if (parent.frames[target])
parent.frames[target].location.replace(sURL);
else
window.open(sURL, target);
return false;
}
}

function checkIEClick() {
var el = getTag(event.srcElement,"A:AREA:")
if ((el!=null) && ((el.tagName=="A") || (el.tagName=="AREA"))) {
event.returnValue = false
navigateTo(el.href,String(el.target).toLowerCase())
}
}

function checkNSClick(ev) {
if (ev.target.href) {
navigateTo(ev.target.href,String(ev.target).toLowerCase())
return false
}
}

if ((document.all) || (document.layers))
if (document.layers) {
document.captureEvents(Event.CLICK)
document.onclick = checkNSClick
}
else
document.onclick = checkIEClick
//&lt;/SCRIPT&gt;
//[/code]

//A little slick redirection:
//<BLOCKQUOTE><font size="1" face="Verdana, Arial">code/font><HR><pre>
//&lt;META HTTP-EQUIV="Refresh" CONTENT="1; <A HREF="http://www.insideDHTML.com/home.asp"&gt;" TARGET=_blank>http://www.insideDHTML.com/home.asp"&gt;</A> 
//&lt;SCRIPT&gt;
//&lt;!--
var version = parseInt(navigator.appVersion)
// replace is supported
if (version>=4 || window.location.replace)
window.location.replace("newPage.htm")
else
window.location.href = "newPage.htm"
// --&gt;
//&lt;/SCRIPT&gt;
//The document, &lt;A HREF="newPage.htm"&gt;Scott's Home&lt;/A&gt; has moved.
//[/code]

//The above code was found at Site Experts. I hope it is useful.




<%
    response.setHeader("Pragma", "No-cache");
    response.setDateHeader("Expires", 0);
    response.setHeader("Cache-Control", "no-cache");
    response.setContentType("text/html; charset=utf-8");
%>