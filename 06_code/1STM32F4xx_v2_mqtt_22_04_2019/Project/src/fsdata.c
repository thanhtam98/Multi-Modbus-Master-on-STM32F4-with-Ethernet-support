#include "fs.h"
#include "lwip/def.h"
#include "fsdata.h"


#define file_NULL (struct fsdata_file *) NULL


static const unsigned int dummy_align__404_html = 0;
static const unsigned char data__404_html[] = {
/* /404.html (10 chars) */
0x2f,0x34,0x30,0x34,0x2e,0x68,0x74,0x6d,0x6c,0x00,0x00,0x00,

/* HTTP header */
/* "HTTP/1.0 404 File not found
" (29 bytes) */
0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x30,0x20,0x34,0x30,0x34,0x20,0x46,0x69,0x6c,
0x65,0x20,0x6e,0x6f,0x74,0x20,0x66,0x6f,0x75,0x6e,0x64,0x0d,0x0a,
/* "Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)
" (63 bytes) */
0x53,0x65,0x72,0x76,0x65,0x72,0x3a,0x20,0x6c,0x77,0x49,0x50,0x2f,0x31,0x2e,0x33,
0x2e,0x31,0x20,0x28,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x73,0x61,0x76,0x61,0x6e,
0x6e,0x61,0x68,0x2e,0x6e,0x6f,0x6e,0x67,0x6e,0x75,0x2e,0x6f,0x72,0x67,0x2f,0x70,
0x72,0x6f,0x6a,0x65,0x63,0x74,0x73,0x2f,0x6c,0x77,0x69,0x70,0x29,0x0d,0x0a,
/* "Content-type: text/html

" (27 bytes) */
0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x74,0x79,0x70,0x65,0x3a,0x20,0x74,0x65,
0x78,0x74,0x2f,0x68,0x74,0x6d,0x6c,0x0d,0x0a,0x0d,0x0a,
/* raw file data (1112 bytes) */
0x3c,0x21,0x44,0x4f,0x43,0x54,0x59,0x50,0x45,0x20,0x48,0x54,0x4d,0x4c,0x20,0x50,
0x55,0x42,0x4c,0x49,0x43,0x20,0x22,0x2d,0x2f,0x2f,0x57,0x33,0x43,0x2f,0x2f,0x44,
0x54,0x44,0x20,0x48,0x54,0x4d,0x4c,0x20,0x34,0x2e,0x30,0x31,0x20,0x54,0x72,0x61,
0x6e,0x73,0x69,0x74,0x69,0x6f,0x6e,0x61,0x6c,0x2f,0x2f,0x45,0x4e,0x22,0x3e,0x0d,
0x0a,0x3c,0x68,0x74,0x6d,0x6c,0x3e,0x3c,0x68,0x65,0x61,0x64,0x3e,0x3c,0x74,0x69,
0x74,0x6c,0x65,0x3e,0x53,0x54,0x4d,0x33,0x32,0x46,0x34,0x78,0x37,0x3c,0x2f,0x74,
0x69,0x74,0x6c,0x65,0x3e,0x0d,0x0a,0x0d,0x0a,0x3c,0x2f,0x68,0x65,0x61,0x64,0x3e,
0x0d,0x0a,0x3c,0x62,0x6f,0x64,0x79,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x63,
0x6f,0x6c,0x6f,0x72,0x3a,0x20,0x62,0x6c,0x61,0x63,0x6b,0x3b,0x20,0x62,0x61,0x63,
0x6b,0x67,0x72,0x6f,0x75,0x6e,0x64,0x2d,0x63,0x6f,0x6c,0x6f,0x72,0x3a,0x20,0x77,
0x68,0x69,0x74,0x65,0x3b,0x22,0x3e,0x0d,0x0a,0x3c,0x74,0x61,0x62,0x6c,0x65,0x20,
0x77,0x69,0x64,0x74,0x68,0x3d,0x22,0x31,0x30,0x30,0x25,0x22,0x3e,0x0d,0x0a,0x3c,
0x74,0x62,0x6f,0x64,0x79,0x3e,0x0d,0x0a,0x3c,0x74,0x72,0x20,0x76,0x61,0x6c,0x69,
0x67,0x6e,0x3d,0x22,0x74,0x6f,0x70,0x22,0x3e,0x0d,0x0a,0x3c,0x74,0x64,0x20,0x77,
0x69,0x64,0x74,0x68,0x3d,0x22,0x38,0x30,0x22,0x3e,0x3c,0x62,0x72,0x3e,0x0d,0x0a,
0x3c,0x64,0x69,0x76,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x74,0x65,0x78,0x74,
0x2d,0x61,0x6c,0x69,0x67,0x6e,0x3a,0x20,0x63,0x65,0x6e,0x74,0x65,0x72,0x3b,0x22,
0x3e,0x3c,0x69,0x6d,0x67,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x77,0x69,0x64,
0x74,0x68,0x3a,0x20,0x39,0x36,0x70,0x78,0x3b,0x20,0x68,0x65,0x69,0x67,0x68,0x74,
0x3a,0x20,0x35,0x37,0x70,0x78,0x3b,0x22,0x20,0x61,0x6c,0x74,0x3d,0x22,0x53,0x54,
0x20,0x6c,0x6f,0x67,0x6f,0x22,0x20,0x73,0x72,0x63,0x3d,0x22,0x53,0x54,0x4d,0x33,
0x32,0x46,0x34,0x78,0x37,0x5f,0x66,0x69,0x6c,0x65,0x73,0x2f,0x6c,0x6f,0x67,0x6f,
0x2e,0x6a,0x70,0x67,0x22,0x3e,0x3c,0x2f,0x64,0x69,0x76,0x3e,0x0d,0x0a,0x3c,0x2f,
0x74,0x64,0x3e,0x0d,0x0a,0x3c,0x74,0x64,0x20,0x77,0x69,0x64,0x74,0x68,0x3d,0x22,
0x35,0x30,0x30,0x22,0x3e,0x0d,0x0a,0x3c,0x68,0x31,0x3e,0x3c,0x73,0x6d,0x61,0x6c,
0x6c,0x3e,0x3c,0x73,0x6d,0x61,0x6c,0x6c,0x3e,0x3c,0x73,0x6d,0x61,0x6c,0x6c,0x3e,
0x3c,0x73,0x6d,0x61,0x6c,0x6c,0x3e,0x3c,0x62,0x69,0x67,0x3e,0x3c,0x62,0x69,0x67,
0x3e,0x3c,0x62,0x69,0x67,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x66,0x6f,0x6e,
0x74,0x2d,0x77,0x65,0x69,0x67,0x68,0x74,0x3a,0x20,0x62,0x6f,0x6c,0x64,0x3b,0x22,
0x3e,0x3c,0x62,0x69,0x67,0x3e,0x3c,0x73,0x74,0x72,0x6f,0x6e,0x67,0x3e,0x3c,0x73,
0x70,0x61,0x6e,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x66,0x6f,0x6e,0x74,0x2d,
0x73,0x74,0x79,0x6c,0x65,0x3a,0x20,0x69,0x74,0x61,0x6c,0x69,0x63,0x3b,0x22,0x3e,
0x53,0x54,0x4d,0x33,0x32,0x46,0x34,0x78,0x37,0x20,0x57,0x65,0x62,0x73,0x65,0x72,
0x76,0x65,0x72,0x20,0x44,0x65,0x6d,0x6f,0x3c,0x2f,0x73,0x70,0x61,0x6e,0x3e,0x3c,
0x2f,0x73,0x74,0x72,0x6f,0x6e,0x67,0x3e,0x3c,0x2f,0x62,0x69,0x67,0x3e,0x3c,0x2f,
0x62,0x69,0x67,0x3e,0x3c,0x2f,0x62,0x69,0x67,0x3e,0x3c,0x2f,0x62,0x69,0x67,0x3e,
0x3c,0x2f,0x73,0x6d,0x61,0x6c,0x6c,0x3e,0x3c,0x2f,0x73,0x6d,0x61,0x6c,0x6c,0x3e,
0x3c,0x2f,0x73,0x6d,0x61,0x6c,0x6c,0x3e,0x3c,0x2f,0x73,0x6d,0x61,0x6c,0x6c,0x3e,
0x0d,0x0a,0x3c,0x73,0x6d,0x61,0x6c,0x6c,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,
0x66,0x6f,0x6e,0x74,0x2d,0x66,0x61,0x6d,0x69,0x6c,0x79,0x3a,0x20,0x56,0x65,0x72,
0x64,0x61,0x6e,0x61,0x3b,0x22,0x3e,0x3c,0x73,0x6d,0x61,0x6c,0x6c,0x3e,0x3c,0x62,
0x69,0x67,0x3e,0x3c,0x62,0x69,0x67,0x3e,0x3c,0x62,0x69,0x67,0x3e,0x3c,0x62,0x69,
0x67,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x66,0x6f,0x6e,0x74,0x2d,0x77,0x65,
0x69,0x67,0x68,0x74,0x3a,0x20,0x62,0x6f,0x6c,0x64,0x3b,0x20,0x63,0x6f,0x6c,0x6f,
0x72,0x3a,0x20,0x72,0x67,0x62,0x28,0x35,0x31,0x2c,0x20,0x35,0x31,0x2c,0x20,0x32,
0x35,0x35,0x29,0x3b,0x22,0x3e,0x3c,0x62,0x69,0x67,0x3e,0x3c,0x73,0x74,0x72,0x6f,
0x6e,0x67,0x3e,0x3c,0x73,0x70,0x61,0x6e,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,
0x66,0x6f,0x6e,0x74,0x2d,0x73,0x74,0x79,0x6c,0x65,0x3a,0x20,0x69,0x74,0x61,0x6c,
0x69,0x63,0x3b,0x22,0x3e,0x3c,0x2f,0x73,0x70,0x61,0x6e,0x3e,0x3c,0x2f,0x73,0x74,
0x72,0x6f,0x6e,0x67,0x3e,0x3c,0x2f,0x62,0x69,0x67,0x3e,0x3c,0x73,0x70,0x61,0x6e,
0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x63,0x6f,0x6c,0x6f,0x72,0x3a,0x20,0x72,
0x67,0x62,0x28,0x35,0x31,0x2c,0x20,0x35,0x31,0x2c,0x20,0x32,0x35,0x35,0x29,0x3b,
0x22,0x3e,0x3c,0x62,0x72,0x3e,0x0d,0x0a,0x3c,0x2f,0x73,0x70,0x61,0x6e,0x3e,0x3c,
0x2f,0x62,0x69,0x67,0x3e,0x3c,0x2f,0x62,0x69,0x67,0x3e,0x3c,0x2f,0x62,0x69,0x67,
0x3e,0x3c,0x2f,0x62,0x69,0x67,0x3e,0x3c,0x2f,0x73,0x6d,0x61,0x6c,0x6c,0x3e,0x3c,
0x2f,0x73,0x6d,0x61,0x6c,0x6c,0x3e,0x3c,0x2f,0x68,0x31,0x3e,0x0d,0x0a,0x3c,0x68,
0x32,0x3e,0x34,0x30,0x34,0x20,0x2d,0x20,0x50,0x61,0x67,0x65,0x20,0x6e,0x6f,0x74,
0x20,0x66,0x6f,0x75,0x6e,0x64,0x3c,0x2f,0x68,0x32,0x3e,0x0d,0x0a,0x3c,0x70,0x3e,
0x3c,0x73,0x70,0x61,0x6e,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x66,0x6f,0x6e,
0x74,0x2d,0x66,0x61,0x6d,0x69,0x6c,0x79,0x3a,0x20,0x54,0x69,0x6d,0x65,0x73,0x20,
0x4e,0x65,0x77,0x20,0x52,0x6f,0x6d,0x61,0x6e,0x2c,0x54,0x69,0x6d,0x65,0x73,0x2c,
0x73,0x65,0x72,0x69,0x66,0x3b,0x22,0x3e,0x20,0x53,0x6f,0x72,0x72,0x79,0x2c,0x0d,
0x0a,0x74,0x68,0x65,0x20,0x70,0x61,0x67,0x65,0x20,0x79,0x6f,0x75,0x20,0x61,0x72,
0x65,0x20,0x72,0x65,0x71,0x75,0x65,0x73,0x74,0x69,0x6e,0x67,0x20,0x77,0x61,0x73,
0x20,0x6e,0x6f,0x74,0x20,0x66,0x6f,0x75,0x6e,0x64,0x20,0x6f,0x6e,0x20,0x74,0x68,
0x69,0x73,0x20,0x73,0x65,0x72,0x76,0x65,0x72,0x2e,0x3c,0x2f,0x73,0x70,0x61,0x6e,
0x3e,0x20,0x3c,0x2f,0x70,0x3e,0x0d,0x0a,0x3c,0x2f,0x74,0x64,0x3e,0x0d,0x0a,0x3c,
0x2f,0x74,0x72,0x3e,0x0d,0x0a,0x3c,0x2f,0x74,0x62,0x6f,0x64,0x79,0x3e,0x0d,0x0a,
0x3c,0x2f,0x74,0x61,0x62,0x6c,0x65,0x3e,0x0d,0x0a,0x3c,0x2f,0x62,0x6f,0x64,0x79,
0x3e,0x3c,0x2f,0x68,0x74,0x6d,0x6c,0x3e,};

static const unsigned int dummy_align__login_html = 1;
static const unsigned char data__login_html[] = {
"/login.html"
"\0"
"HTTP/1.0 200 OK\r\n"
"Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)\r\n"
"Content-type: text/html\r\n\r\n"
"<!DOCTYPE html PUBLIC \"-//W3C//Dtd XHTML 1.0 Transitional//EN\" \"http://www.w3.org/tr/xhtml1/Dtd/xhtml1-Transitional.dtd\">"
"<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"utf-8\" lang=\"utf-8\" dir=\"ltr\">"
"\n<head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"
"\n<link href=\"./style/style.css\" type=\"text/css\" rel=\"stylesheet\">"
"\n<script src=\"./script/common.js\" type=\"text/javascript\"></script>"
"\n<title>Home Login</title>"
"\n<style type=\"text/css\">"
"\ninput {margin-top: 5px;}"
"\ninput[type=\"text\"], input[type=\"password\"] {width: 120px;}"
"\ndiv {margin: 0 auto;text-align: center;}"
"\n#l_tip {width: 800px;height: 30px;text-align: center;font-size: 14px;}"
"\n#tip {color: #ff0000;}"
"\n#l_content {width: 350px;height: 180px;border: 2px solid #4393a3;font-size: 14px;}"
"\n#l_content li {float: left;}"
"\n.l_title {width: 350px;height: 30px;line-height: 30px;background-color: #4393a3;text-align: center;}"
"\n.l_pad {width: 350px;height: 20px;line-height: 20px;}"
"\n.l_user, .l_pass {width: 150px;height: 30px;line-height: 30px;}"
"\n.l_foot1, .l_foot5 {width: 85px;height: 30px;line-height: 30px;background-color: #4393a3;}"
"\n.l_foot2, .l_foot4 {width: 70px;height: 30px;line-height: 30px;background-color: #4393a3;}"
"\n.l_foot3 {width: 40px;height: 30px;line-height: 30px;background-color: #4393a3;}"
"\n</style>"
"\n<script type=\"text/javascript\">"
"\nvar XUsername = \"\";"
"\nvar XWebTimeout = 120;"
"\nvar XCurrentUser = 0;"
"\nvar XCurrentMenu = 'devinfo';"
"\nvar XLogoFilename = '';"
"\nvar XMultiLang = '0';"
"\nvar XOntName = 'Home Login';"
"\nvar XHasWifi = 1;"
"\nvar XHasUsb  = 1;"
"\nvar lang  = 0;"
"\nXUsername = \"admin\";"
"\nvar XPasswdTip = '';"
"\nvar XHasLogin = '0';"	
"\nfunction WebLoadString(){document.title = XOntName;$(\"XS0\").innerHTML = \"Password Tip\";"
"\nif(XPasswdTip.length > 2)"
"\nXPasswdTip = \"Default web password is WiFi WEP password\";"
"\n$(\"tip\").innerHTML = XPasswdTip;"
"\nif(lang==0)"
"\n$(\"XS1\").innerHTML = XOntName;"
"\nelse"
"\n$(\"XS1\").innerHTML = \"Login\";"
"\n$(\"XS2\").innerHTML = \"Username\";"
"\n$(\"XS3\").innerHTML = \"Password\""
"\n$S(\"login\", \"Login\");"
"\n$S(\"clear\", \"Clear\");}"
"\nfunction WebInit(){"
"\nif (window.self != window.top)"
"\n{window.top.location.href = window.self.location.href;return;}"
"\n$(\"l_tip\").style.display = \"none\";"
"\nif (XPasswdTip.length > 2){$(\"l_tip\").style.display = \"block\";}"
"\nWebLoadString();"
"\n$(\"XForm\").action = \"/login.html\";"
"\n$S(\"XWebPageName\", \"Login\");"
"\n$S(\"username\", XUsername);"
"\n$(\"username\").focus();}"
"\nfunction OnClear(){"
"\n$S(\"username\", \"\");"
"\n$S(\"password\", \"\");}"
"\nfunction OnLogin()"
"\n{if ($(\"username\").value.length == 0){"
"\nalert(\"Please input username!\");"
"\n$(\"username\").focus;return;}"
"\nif ($(\"password\").value.length == 0){"
"\nalert(\"Please input password!\");"
"\n$(\"password\").focus;return;}"
"\n$(\"XForm\").action = \"/login\";"
"\n$(\"XForm\").submit();}"
"\nfunction OnEnter(e){var	k = 0;"
"\nif (window.event){k = e.keyCode;}"
"\nelse if (e.which){k = e.which;}"
"\nif (k == 13){OnLogin();}}"
"\nfunction OnUserEnter(e){"
"\nvar	k = 0;"
"\nif (window.event){k = e.keyCode;}"
"\nelse if (e.which){k = e.which;}"
"\nif (k == 13){$(\"password\").focus();}}"
"\n</script>"
"\n</head>"
"\n<body onload=\"WebInit();\">"
"\n<div style=\"height: 100px\"></div>"
"\n<div id=\"l_tip\" style=\"display: none;\">"
"\n<ul>"
"\n<li><span id=\"XS0\">Password Tip</span><span id=\"tip\"></span></li>"
"\n</ul>"
"\n</div>"
"\n<div id=\"l_content\">"
"\n<form id=\"XForm\" name=\"XForm\" method=\"post\" action=\"\">"
"\n<input type=\"hidden\" id=\"XWebPageName\" name=\"XWebPageName\" value=\"Login\">"
"\n<ul>"
"\n<li class=\"l_title\"><span id=\"XS1\">Home Login</span></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pad\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_user\"><span style=\"width: 150px;\" id=\"XS2\">Username</span></li>"
"\n<li class=\"l_user\"><input id=\"username\" name=\"username\" type=\"text\" style=\"width: 120px;\" maxlength=\"24\" onkeypress=\"OnUserEnter(event);\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pad\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS3\">Password</span></li>"
"\n<li class=\"l_pass\"><input id=\"password\" name=\"password\" type=\"password\" style=\"width: 120px;\" maxlength=\"24\" onkeypress=\"OnEnter(event);\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pad\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_foot1\"></li>"
"\n<li class=\"l_foot2\"><input id=\"login\" name=\"login\" type=\"button\" value=\"Login\" onclick=\"OnLogin();\"></li>"
"\n<li class=\"l_foot3\"></li>"
"\n<li class=\"l_foot4\"><input id=\"clear\" type=\"button\" value=\"Clear\" onclick=\"OnClear();\"></li>"
"\n<li class=\"l_foot5\"></li>"
"\n</ul>"
"\n</form>"
"\n</div>"
"\n</body></html>"
};
static const unsigned int dummy_align__index_html = 2;
static const unsigned char data__index_html[] =
{
"/index.html"
"\0"
"HTTP/1.0 200 OK\r\n"
"Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)\r\n"
"Content-type: text/html\r\n\r\n"
"<!DOCTYPE html PUBLIC \"-//W3C//Dtd XHTML 1.0 Transitional//EN\" \"http://www.w3.org/tr/xhtml1/Dtd/xhtml1-Transitional.dtd\">"
"\n<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"utf-8\" lang=\"utf-8\" dir=\"ltr\">"
"\n<head>"
"\n<title>Device Config</title>"
"\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
"\n<link href=\"style/style.css\" type=\"text/css\" rel=\"stylesheet\" />"
"\n<script type=\"text/javascript\" src=\"script/common.js\"></script>"
"\n<style type=\"text/css\">"
"\n#l_content {width: 600px;height: 360px;border: 2px solid #4393a3;font-size: 14px;}"
"\n#l_content li {float: left;}"
"\n.l_title1 {width: 606px;height: 30px;line-height: 30px;}"
"\n.l_pad {width: 606px;height: 20px;line-height: 20px;}"
"\n.l_user, .l_pass {width: 150px;height: 30px;line-height: 30px;}"
"\n.l_foot1 {width: 150px;height: 30px;line-height: 30px;background-color: #ffffff;}"
"\n.l_foot2 {width: 80px;height: 30px;line-height: 30px;background-color: #ffffff;}"
"\n.l_foot3 {width: 60px;height: 30px;line-height: 30px;background-color: #ffffff;}"
"\n</style>"
"\n<script type=\"text/javascript\">"
"\nfunction XLogout(){"
"\ntop.window.location.href = \"./logout.html\";}"
"\nfunction XWebInit(){"
"\n$(\"XForm\").action = \"/index.html\";"
"\nsetTimeout(\"XLogout();\", 180*1000);"
"\nreturn true;}"
"\nfunction change_color(varid){"
"\nif(varid ==1){"
"\n$(\"lanconfig\").style=\"background: #10F53F\";"
"\n$(\"portconfig\").style=\"background: #4393a3\";"
"\n$(\"hostconfig\").style=\"background: #4393a3\";"
"\n$(\"maintain\").style=\"background: #4393a3\";"
"\n$(\"reboot\").style=\"background: #4393a3\";}"
"\nelse if(varid ==2){"
"\n$(\"lanconfig\").style=\"background: #4393a3\";"
"\n$(\"portconfig\").style=\"background: #4393a3\";"
"\n$(\"hostconfig\").style=\"background: #10F53F\";"
"\n$(\"maintain\").style=\"background: #4393a3\";"
"\n$(\"reboot\").style=\"background: #4393a3\"}"
"\nelse if(varid ==3){"
"\n$(\"lanconfig\").style=\"background: #4393a3\";"
"\n$(\"portconfig\").style=\"background: #4393a3\";"
"\n$(\"hostconfig\").style=\"background: #4393a3\";"
"\n$(\"maintain\").style=\"background: #10F53F\";"
"\n$(\"reboot\").style=\"background: #4393a3\"}"
"\nelse if(varid ==4){"
"\n$(\"lanconfig\").style=\"background: #4393a3\";"
"\n$(\"portconfig\").style=\"background: #4393a3\";"
"\n$(\"hostconfig\").style=\"background: #4393a3\";"
"\n$(\"maintain\").style=\"background: #4393a3\";"
"\n$(\"reboot\").style=\"background: #10F53F\"}"
"\nelse if(varid ==5){"
"\n$(\"lanconfig\").style=\"background: #4393a3\";"
"\n$(\"portconfig\").style=\"background: #10F53F\";"
"\n$(\"hostconfig\").style=\"background: #4393a3\";"
"\n$(\"maintain\").style=\"background: #4393a3\";"
"\n$(\"reboot\").style=\"background: #4393a3\"}}</script>"
"\n</head>"
"\n<body id=\"x_root_body\" onload=\"XWebInit();\">"
"\n<div style=\"height: 100px\"></div>"
"\n<div id=\"l_tip\" style=\"display: none;\">"
"\n<ul>"
"\n<li><span id=\"XS0\">Password Tip</span><span id=\"tip\"></span></li>"
"\n</ul>"
"\n</div>"
"\n<div id=\"l_content\">"
"\n<form id=\"XForm\" name=\"XForm\" method=\"post\" action=\"\">"
"\n<input type=\"hidden\" id=\"XWebPageName\" name=\"LanConfig\" value=\"Save\">"
"\n<div id=\"menu\">"
    "\n<ul>"
        "\n<li><a id=\"lanconfig\" style=\"background:#10F53F\" onclick=\"change_color(1)\" href=\"./lanconfig.html\" target=\"iframecontent\" >Lan Config</a></li>"
        "\n<li><a id=\"portconfig\" onclick=\"change_color(5)\" href=\"./portconfig.html\" target=\"iframecontent\" >Port Config</a></li>"
        "\n<li><a id=\"hostconfig\" onclick=\"change_color(2)\" href=\"./hostconfig.html\" target=\"iframecontent\" >Host Config</a></li>"
        "\n<li><a id=\"maintain\" onclick=\"change_color(3)\" href=\"./maintain.html\" target=\"iframecontent\" >Maintain</a></li>"
        "\n<li><a id=\"reboot\" onclick=\"change_color(4)\" href=\"./Reboot.html\" target=\"iframecontent\">Reboot</a></li>"
        "\n<li><a href=\"./logout.html\">Logout</a></li>"
    "\n</ul>"
"\n</div>"
"\n<div>"
"\n<iframe id=\"iframe\" name=\"iframecontent\" src=\"./lanconfig.html\" width=\"480\" height=\"340\" frameborder = \"0\">"
"\n</iframe>"
"\n</div>"
"\n</form>"
"\n</div>"
"\n</body>"
"\n</html>"
};

static const unsigned int dummy_align__lanconfig_html = 3;
static const unsigned char data__lanconfig_html[] =
{
"/lanconfig.html"
"\0"
"HTTP/1.0 200 OK\r\n"
"Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)\r\n"
"Content-type: text/html\r\n\r\n"
"<!DOCTYPE html PUBLIC \"-//W3C//Dtd XHTML 1.0 Transitional//EN\" \"http://www.w3.org/tr/xhtml1/Dtd/xhtml1-Transitional.dtd\">"
"\n<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"utf-8\" lang=\"utf-8\" dir=\"ltr\">"
"\n<head>"
"\n<title>Lan Config</title>"
"\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
"\n<script type=\"text/javascript\" src=\"script/datalan.js\"></script>"
"\n<style type=\"text/css\">"
"\nhtml, body, div, ul, li, span, p, a img, form, frameset, frame,"
"\ntable, caption, tbody, tfoot, thead, tr, th, td {margin: 0;padding: 0;border: 0;outline: 0;}"
"\ntable {border-collapse: collapse;border-spacing: 0;}"
"\nbody {font-family: arial, serif, fixedsys;}"
"\ntd {text-align: center;vertical-align: middle;}"
"\nol, ul {list-style: none;}"
"\ndiv {text-align: left;vertical-align: middle;}"
"\ninput {margin-top: 5px;}"
"\ninput[type=\"text\"], input[type=\"password\"] {width: 120px;}"
"\ndiv {margin: 0 auto;text-align: center;}"
"\n#l_tip {width: 800px;height: 30px;text-align: center;font-size: 14px;}"
"\n#tip {color: #ff0000;}"
"\n#l_content {width: 350px;height: 180px;border: 2px solid #ffffff;font-size: 14px;}"
"\n#l_content li {float: left;}"
"\n.l_title {width: 350px;height: 30px;line-height: 30px;background-color: #ffffff; }"
"\n.l_pad {width: 350px;height: 20px;line-height: 20px;}"
"\n.l_user, .l_pass {width: 150px;height: 30px;line-height: 30px;}"
"\n.l_foot1 {width: 85px;height: 30px;line-height: 30px;background-color: #ffffff;}"
"\n.l_foot2 {width: 70px;height: 30px;line-height: 30px;background-color: #ffffff;}"
"\n.l_foot3 {width: 40px;height: 30px;line-height: 30px;background-color: #ffffff;}"
"\n</style>"
"\n<script type=\"text/javascript\">"
"\nfunction $(id){return document.getElementById(id);}"
"\nfunction $G(id){return $(id).value;}"
"\nfunction $S(id, value){$(id).value = value;}"
"\nfunction $W(obj){document.write(obj);}"
"\nfunction WebLoadString(){"
"\ntry{"
"\ndocument.getElementById(\"Dhcp\").selectedIndex = eval(\"DataString.dhcp\");"
"\n$S(\"DeviceID\", eval(\"DataString.deviceid\"));"
"\n$S(\"Set_ip\", eval(\"DataString.ip\"));"
"\n$S(\"Subnet\", eval(\"DataString.subnetmask\"));"
"\n$S(\"Gateway\", eval(\"DataString.gateway\"));}"
"\ncatch(error){}}"
"\nfunction XWebInit(){"
"\n$(\"XForm\").action = \"/lanconfig.html\";"
"\nWebLoadString();}"
"\nfunction XDoSubmit(){"
"\n$(\"XForm\").action = \"/lanconfig\";"
"\n$(\"XForm\").submit();}"
"\nfunction XDoClear(){"
"\n$S(\"Set_ip\", \"\");"
"\n$S(\"Subnet\", \"\");"
"\n$S(\"Gateway\", \"\");"
"\ndocument.getElementById(\"Dhcp\").selectedIndex = 0;"
"\nreturn true;}</script>"
"\n</head>"
"\n<body id=\"x_root_body\" onload=\"XWebInit();\">"
"\n<div style=\"height: 10px\"></div>"
"\n<div id=\"l_content\">"
"\n<form id=\"XForm\" name=\"XForm\" method=\"post\" action=\"\">"
"\n<input type=\"hidden\" id=\"XWebPageName\" name=\"LanConfig\" value=\"Save\">"
"\n<ul>"
"\n<li class=\"l_pad\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS2\">Device ID</span></li>"
"\n<li class=\"l_pass\"><input disabled id=\"DeviceID\" name=\"DeviceID\" type=\"text\" style=\"width: 120px;\" maxlength=\"24\" onkeypress=\"OnEnter(event);\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pad\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS2\">Set IP Device</span></li>"
"\n<li class=\"l_pass\"><input id=\"Set_ip\" name=\"Set_ip\" type=\"text\" style=\"width: 120px;\" maxlength=\"24\" onkeypress=\"OnEnter(event);\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pad\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS3\">Subnet Mask</span></li>"
"\n<li class=\"l_pass\"><input id=\"Subnet\" name=\"Subnet\" type=\"text\" style=\"width: 120px;\" maxlength=\"24\" onkeypress=\"OnEnter(event);\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pad\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS4\">Gateway</span></li>"
"\n<li class=\"l_pass\"><input id=\"Gateway\" name=\"Gateway\" type=\"text\" style=\"width: 120px;\" maxlength=\"24\" onkeypress=\"OnEnter(event);\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pad\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS5\">DHCP</span></li>"
"\n<li class=\"l_pass\"><select id=\"Dhcp\" name=\"Dhcp\">"
"\n<option value=\"0\">Disable</option>"
"\n<option value=\"1\">Enable</option>"
"\n</select>"
"</li>"
"\n<ul>"
"\n<li class=\"l_pad\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_foot1\"></li>"
"\n<li class=\"l_foot2\"><input id=\"Save\" name=\"Save\" type=\"button\" value=\"Save\" onclick=\"XDoSubmit();\"></li>"
"\n<li class=\"l_foot3\"></li>"
"\n<li class=\"l_foot2\"><input id=\"clear\" name=\"Clear\" type=\"button\" value=\"Clear\" onclick=\"XDoClear();\"></li>"
"\n</ul>"
"\n</form>"
"\n</div>"
"\n</body>"
"\n</html>"
};

static const unsigned int dummy_align__hostconfig_html = 4;
static const unsigned char data__hostconfig_html[] =
{
"/hostconfig.html"
"\0"
"HTTP/1.0 200 OK\r\n"
"Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)\r\n"
"Content-type: text/html\r\n\r\n"
"<!DOCTYPE html PUBLIC \"-//W3C//Dtd XHTML 1.0 Transitional//EN\" \"http://www.w3.org/tr/xhtml1/Dtd/xhtml1-Transitional.dtd\">"
"\n<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"utf-8\" lang=\"utf-8\" dir=\"ltr\">"
"\n<head>"
"\n<title>Lan Config</title>"
"\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
"\n<link href=\"style/style.css\" type=\"text/css\" rel=\"stylesheet\" />"
"\n<script type=\"text/javascript\" src=\"script/datahost.js\"></script>"
"\n<style type=\"text/css\">"
"\nhtml, body, div, ul, li, span, p, a img, form, frameset, frame,"
"\ntable, caption, tbody, tfoot, thead, tr, th, td {margin: 0;padding: 0;border: 0;outline: 0;}"
"\ntable {border-collapse: collapse;border-spacing: 0;}"
"\nbody {font-family: arial, serif, fixedsys;}"
"\ntd {text-align: center;vertical-align: middle;}"
"\nol, ul {list-style: none;}"
"\ndiv {text-align: left;vertical-align: middle;}"
"\ninput {margin-top: 5px;}"
"\ninput[type=\"text\"], input[type=\"password\"] {width: 120px;}"
"\ndiv {margin: 0 auto;text-align: center;}"
"\n#l_tip {width: 800px;height: 30px;text-align: center;font-size: 14px;}"
"\n#tip {color: #ff0000;}"
"\n#l_content {width: 350px;height: 180px;border: 2px solid #ffffff;font-size: 14px;}"
"\n#l_content li {float: left;}"
"\n.l_title {width: 350px;height: 30px;line-height: 30px;background-color: #ffffff; }"
"\n.l_pad {width: 350px;height: 20px;line-height: 20px;}"
"\n.l_user, .l_pass {width: 150px;height: 30px;line-height: 30px;}"
"\n.l_foot1 {width: 85px;height: 30px;line-height: 30px;background-color: #ffffff;}"
"\n.l_foot2 {width: 70px;height: 30px;line-height: 30px;background-color: #ffffff;}"
"\n.l_foot3 {width: 40px;height: 30px;line-height: 30px;background-color: #ffffff;}"
"\n</style>"
"\n<script type=\"text/javascript\">"
"\nfunction $(id){return document.getElementById(id);}"
"\nfunction $G(id){return $(id).value;}"
"\nfunction $S(id, value){$(id).value = value;}"
"\nfunction $W(obj){document.write(obj);}"
"\nfunction WebLoadString(){"
"\ntry{"
"\n$S(\"hostip\", eval(\"DataString.hostip\"));"
"\n$S(\"hostname\", eval(\"DataString.hostname\"));"
"\n$S(\"username\", eval(\"DataString.username\"));"
"\n$S(\"password\", eval(\"DataString.password\"));}"
"\ncatch(error){}}"
"\nfunction XWebInit(){"
"\n$(\"XForm\").action = \"/hostconfig.html\";"
"\nWebLoadString();}"
"\nfunction XDoSubmit(){"
"\n$(\"XForm\").action = \"/hostconfig\";"
"\n$(\"XForm\").submit();}"
"\nfunction XDoClear(){"
"\n$S(\"hostip\", \"\");"
"\n$S(\"hostname\", \"\");"
"\n$S(\"username\", \"\");"
"\n$S(\"password\", \"\");"
"\nreturn true;}</script>"
"\n</head>"
"\n<body id=\"x_root_body\" onload=\"XWebInit();\">"
"\n<div style=\"height: 10px\"></div>"
"\n<div id=\"l_content\">"
"\n<form id=\"XForm\" name=\"XForm\" method=\"post\" action=\"\">"
"\n<input type=\"hidden\" id=\"XWebPageName\" name=\"HostConfig\" value=\"Save\">"
"\n<ul>"
"\n<li class=\"l_pad\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS2\">Set Host IP</span></li>"
"\n<li class=\"l_pass\"><input id=\"hostip\" name=\"hostip\" type=\"text\" style=\"width: 120px;\" maxlength=\"24\" onkeypress=\"OnEnter(event);\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pad\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS3\">Set Host Name</span></li>"
"\n<li class=\"l_pass\"><input id=\"hostname\" name=\"hostname\" type=\"text\" style=\"width: 120px;\" maxlength=\"50\" onkeypress=\"OnEnter(event);\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pad\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS4\">Set Username</span></li>"
"\n<li class=\"l_pass\"><input id=\"username\" name=\"username\" type=\"text\" style=\"width: 120px;\" maxlength=\"24\" onkeypress=\"OnEnter(event);\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pad\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS4\">Set Password</span></li>"
"\n<li class=\"l_pass\"><input id=\"password\" name=\"password\" type=\"text\" style=\"width: 120px;\" maxlength=\"24\" onkeypress=\"OnEnter(event);\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pad\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_foot1\"></li>"
"\n<li class=\"l_foot2\"><input id=\"Save\" name=\"Save\" type=\"button\" value=\"Save\" onclick=\"XDoSubmit();\"></li>"
"\n<li class=\"l_foot3\"></li>"
"\n<li class=\"l_foot2\"><input id=\"clear\" name=\"Clear\" type=\"button\" value=\"Clear\" onclick=\"XDoClear();\"></li>"
"\n<li class=\"l_foot1\"></li>"
"\n</ul>"
"\n</form>"
"\n</div>"
"\n</body>"
"\n</html>"
};

static const unsigned int dummy_align__maintain_html = 5;
static const unsigned char data__maintain_html[] =
{
"/maintain.html"
"\0"
"HTTP/1.0 200 OK\r\n"
"Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)\r\n"
"Content-type: text/html\r\n\r\n"
"<!DOCTYPE html PUBLIC \"-//W3C//Dtd XHTML 1.0 Transitional//EN\" \"http://www.w3.org/tr/xhtml1/Dtd/xhtml1-Transitional.dtd\">"
"\n<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"utf-8\" lang=\"utf-8\" dir=\"ltr\">"
"\n<head>"
"\n<title>Lan Config</title>"
"\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
"\n<style type=\"text/css\">"
"\nhtml, body, div, ul, li, span, p, a img, form, frameset, frame,"
"\ntable, caption, tbody, tfoot, thead, tr, th, td {margin: 0;padding: 0;border: 0;outline: 0;}"
"\ntable {border-collapse: collapse;border-spacing: 0;}"
"\nbody {font-family: arial, serif, fixedsys;}"
"\ntd {text-align: center;vertical-align: middle;}"
"\nol, ul {list-style: none;}"
"\ndiv {text-align: left;vertical-align: middle;}"
"\ninput {margin-top: 5px;}"
"\ninput[type=\"text\"], input[type=\"password\"] {width: 120px;}"
"\ndiv {margin: 0 auto;text-align: center;}"
"\n#l_tip {width: 800px;height: 30px;text-align: center;font-size: 14px;}"
"\n#tip {color: #ff0000;}"
"\n#l_content {width: 350px;height: 180px;border: 2px solid #ffffff;font-size: 14px;}"
"\n#l_content li {float: left;}"
"\n.l_title {width: 350px;height: 30px;line-height: 30px;background-color: #ffffff; }"
"\n.l_pad {width: 350px;height: 20px;line-height: 20px;}"
"\n.l_user, .l_pass {width: 150px;height: 30px;line-height: 30px;}"
"\n.l_foot1 {width: 85px;height: 30px;line-height: 30px;background-color: #ffffff;}"
"\n.l_foot2 {width: 70px;height: 30px;line-height: 30px;background-color: #ffffff;}"
"\n.l_foot3 {width: 40px;height: 30px;line-height: 30px;background-color: #ffffff;}"
"\n</style>"
"\n<script type=\"text/javascript\">"
"\nfunction $(id){return document.getElementById(id);}"
"\nfunction $G(id){return $(id).value;}"
"\nfunction $S(id, value){$(id).value = value;}"
"\nfunction $W(obj){document.write(obj);}"
"\nfunction WebLoadString(){"
"\ntry{"
"\n$S(\"username\", \"\");"
"\n$S(\"password\", \"\");"
"\n$S(\"confirmpass\", \"\");}"
"\ncatch(error){}}"
"\nfunction XWebInit(){"
"\n$(\"XForm\").action = \"/maintain.html\";"
"\nWebLoadString();}"
"\nfunction OnEnter(e){var	k = 0;"
"\nif (window.event){k = e.keyCode;}"
"\nelse if (e.which){k = e.which;}"
"\nif (k == 13){XDoSubmit();}}"
"\nfunction OnUserEnter(e){"
"\nvar	k = 0;"
"\nif (window.event){k = e.keyCode;}"
"\nelse if (e.which){k = e.which;}"
"\nif (k == 13){$(\"password\").focus();}}"
"\nfunction OnPassWordEnter(e){"
"\nvar	k = 0;"
"\nif (window.event){k = e.keyCode;}"
"\nelse if (e.which){k = e.which;}"
"\nif (k == 13){$(\"confirmpass\").focus();}}"
"\nfunction XDoSubmit(){"
"\nif ($(\"username\").value.length == 0){"
"\nalert(\"Please input username!\");"
"\n$(\"username\").focus;return;}"
"\nif ($(\"password\").value.length == 0){"
"\nalert(\"Please input password!\");"
"\n$(\"password\").focus;return;}"
"\nif($(\"password\").value!=$(\"confirmpass\").value){"
"\nalert(\"Comfirm Pass Fail!\");"
"\n$(\"confirmpass\").focus;return;}"
"\n$(\"XForm\").action = \"/maintain\";"
"\n$(\"XForm\").submit();}"
"\nfunction XDoClear(){"
"\n$S(\"username\", \"\");"
"\n$S(\"password\", \"\");"
"\n$S(\"confirmpass\", \"\");"
"\nreturn true;}</script>"
"\n</head>"
"\n<body id=\"x_root_body\" onload=\"XWebInit();\">"
"\n<div style=\"height: 10px\"></div>"
"\n<div id=\"l_content\">"
"\n<form id=\"XForm\" name=\"XForm\" method=\"post\" action=\"\">"
"\n<input type=\"hidden\" id=\"XWebPageName\" name=\"Maintain\" value=\"Save\">"
"\n<ul>"
"\n<li class=\"l_pad\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS2\">Username</span></li>"
"\n<li class=\"l_pass\"><input id=\"username\" name=\"username\" type=\"text\" style=\"width: 120px;\" maxlength=\"24\" onkeypress=\"OnUserEnter(event);\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pad\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS3\">New Password</span></li>"
"\n<li class=\"l_pass\"><input id=\"password\" name=\"password\" type=\"password\" style=\"width: 120px;\" maxlength=\"24\" onkeypress=\"OnPassWordEnter(event);\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pad\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS4\">Confirm Password</span></li>"
"\n<li class=\"l_pass\"><input id=\"confirmpass\" name=\"confirmpass\" type=\"password\" style=\"width: 120px;\" maxlength=\"24\" onkeypress=\"OnEnter(event);\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pad\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_foot1\"></li>"
"\n<li class=\"l_foot2\"><input id=\"Save\" name=\"Save\" type=\"button\" value=\"Save\" onclick=\"XDoSubmit();\"></li>"
"\n<li class=\"l_foot3\"></li>"
"\n<li class=\"l_foot2\"><input id=\"clear\" name=\"Clear\" type=\"button\" value=\"Clear\" onclick=\"XDoClear();\"></li>"
"\n<li class=\"l_foot1\"></li>"
"\n</ul>"
"\n</form>"
"\n</div>"
"\n</body>"
"\n</html>"
};

static const unsigned int dummy_align__reboot_html = 6;
static const unsigned char data__reboot_html[] =
{
"/reboot.html"
"\0"
"HTTP/1.0 200 OK\r\n"
"Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)\r\n"
"Content-type: text/html\r\n\r\n"
"<!DOCTYPE html PUBLIC \"-//W3C//Dtd XHTML 1.0 Transitional//EN\" \"http://www.w3.org/tr/xhtml1/Dtd/xhtml1-Transitional.dtd\">"
"\n<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"utf-8\" lang=\"utf-8\" dir=\"ltr\">"
"\n<head>"
"\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
"\n<title>Host Config</title>"
"\n</head>"
"\n<body id=\"x_root_body\">"
"\n<div id=\"l_content\">"
"\n<div style=\"height: 10px\"></div>"
"\n<center><br>Device Rebooting...<br /></center>"
"\n</div>"
"\n</body>"
"\n</html>"
};
static const unsigned int dummy_align__style_css = 7;
static const unsigned char data__style_css[] =
{
"/style/style.css"
"\0"
"html, body, div, ul, li, span, p, a img, form, frameset, frame,"
"\ntable, caption, tbody, tfoot, thead, tr, th, td {margin: 0;padding: 0;border: 0;outline: 0;}"
"\ntable {border-collapse: collapse;border-spacing: 0;}"
"\nbody {font-family: arial, serif, fixedsys;}"
"\ntd {text-align: center;vertical-align: middle;}"
"\nol, ul {list-style: none;}"
"\ndiv {text-align: left;vertical-align: middle;}"
"\ninput {margin-top: 5px;}"
"\ninput[type=\"text\"], input[type=\"password\"] {width: 120px;}"
"\ndiv {margin: 0 auto;text-align: center;}"
"\n#l_tip {width: 800px;height: 30px;text-align: center;font-size: 14px;}"
"\n#tip {color: #ff0000;}"
"\n#l_content {	width: 350px;height: 180px;border: 2px solid #4393a3;font-size: 14px;}"
"\n#l_content li {float: left;}"
"\n.l_title {width: 350px;height: 30px;line-height: 30px;background-color: #4393a3;text-align: center;}"
"\n.l_pad {width: 350px;height: 20px;line-height: 20px;}"
"\n.l_user, .l_pass {width: 150px;height: 30px;line-height: 30px;}"
"\n.l_foot1, .l_foot5 {width: 85px;	height: 30px;line-height: 30px;background-color: #4393a3;}"
"\n.l_foot2, .l_foot4 {width: 70px;height: 30px;line-height: 30px;background-color: #4393a3;}"
"\n.l_foot3 {width: 40px;height: 30px;line-height: 30px;background-color: #4393a3;}"
"\n#div_xcontent {width: 800px;}"
"\n#div_xleft {width: 40px;height: 100px;}"
"\n#div_xright {width: 760px;}"
"\n#div_xleft, #div_xright {float: left;}"
"\n.x_form_line {width: 760px;height: 40px;line-height: 40px;}"
"\n.x_form_line li {float: left;}"
"\n.x_form_caption {width: 300px;font-size: 18px;text-align: left;vertical-align: middle;}"
"\n.x_form_input {width: 460px;font-size: 14px;text-align: left;vertical-align: middle;}"
"\n.x_form_l560 {width: 560px;font-size: 14px;text-align: left;vertical-align: middle;}"
"\n.x_form_r200 {width: 200px;font-size: 14px;text-align: left;vertical-align: middle;}"
"\n.x_form_text {width: 760px;font-size: 14px;text-align: left;vertical-align: middle;}"
"\n.x_form_pad {width: 200px;}"
"\n.x_form_left {width: 200px;font-size: 14px;text-align: left;vertical-align: middle;}"
"\n.x_form_middle {width: 100px;font-size: 14px;text-align: right;vertical-align: middle;}"
"\n.x_form_right {width: 260px;}"
"\n.x_form_input input, .x_form_input select, .x_form_left input, .x_form_middle input {margin-top: 4px !important; margin-top: 8px;padding: 0;}"
"\n.x_line {width: 760px;height: 45px;background-image: url(/images/hline.png)}"
"\n.x_hide {display: none;}"
"\n.x_bold {font-weight: bold;}"
"\n.x_table {border-collapse: collapse;border: 1px solid #888888;font-size: 14px;}"
"\n.x_table tr {height: 24px;vertical-align: middle;}"
"\n.x_table th {border: 1px solid #888888;background-color: #cccccc;vertical-align: middle;}"
"\n.x_table .x_td1 {border: 1px solid #aaaaaa;background-color: #efefef;vertical-align: middle;}"
"\n.x_table .x_td2 {border: 1px solid #aaaaaa;background-color: #bdd7e7;vertical-align: middle;}"
"\n.x_table th, .x_table td {text-align: center;}"
"\n.x_sub_title {width: 790px;}"
"\n.x_sub_title li {float: left;}"
"\n.x_form_area {width: 530px;margin: 0px 33px 0px 0px;}"
"\n#menu ul{list-style-type:none;padding:0px;margin:0px;}"
"\n#menu ul li{display:inline-block;}"
"\n#menu ul a{text-decoration:none;width:100px;float:left;background:#4393a3;color:#fff;font-weight:bold;text-align:center;line-height:30px;}"
"\n#menu a:hover {background: #F1F1F1;color: #333;}"
"\n#menu a:active {background: #F1F1F1;color: #333;}"
};
static const unsigned int dummy_align__script_common_js = 8;
static const unsigned char data__script_common_js[] =
{
"/script/common.js"
"\0"
"\nfunction $(id){return document.getElementById(id);}"
"\nfunction $G(id){return $(id).value;}"
"\nfunction $S(id, value){$(id).value = value;}"
"\nfunction $W(obj){document.write(obj);}"
};

static const unsigned int dummy_align__portconfig_html = 9;
static const unsigned char data__portconfig_html[] =
{
"/portconfig.html"
"\0"
"HTTP/1.0 200 OK\r\n"
"Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)\r\n"
"Content-type: text/html\r\n\r\n"
"<!DOCTYPE html PUBLIC \"-//W3C//Dtd XHTML 1.0 Transitional//EN\" \"http://www.w3.org/tr/xhtml1/Dtd/xhtml1-Transitional.dtd\">"
"\n<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"utf-8\" lang=\"utf-8\" dir=\"ltr\">"
"\n<head>"
"\n<title>Port Config</title>"
"\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
"\n<script type=\"text/javascript\" src=\"script/dataport.js\"></script>"
"\n<style type=\"text/css\">"
"\nhtml, body, div, ul, li, span, p, a img, form, frameset, frame,"
"\ntable, caption, tbody, tfoot, thead, tr, th, td {margin: 0;padding: 0;border: 0;outline: 0;}"
"\ntable {border-collapse: collapse;border-spacing: 0;}"
"\nbody {font-family: arial, serif, fixedsys;}"
"\ntd {text-align: center;vertical-align: middle;}"
"\nol, ul {list-style: none;}"
"\ndiv {text-align: left;vertical-align: middle;}"
"\ninput {margin-top: 5px;}"
"\ninput[type=\"text\"], input[type=\"password\"] {width: 120px;}"
"\ndiv {margin: 0 auto;text-align: center;}"
"\n#l_tip {width: 800px;height: 30px;text-align: center;font-size: 14px;}"
"\n#tip {color: #ff0000;}"
"\n#l_content {width: 450px;height: 240px;border: 2px solid #ffffff;font-size: 14px;}"
"\n#l_content li {float: left;}"
"\n.l_title {width: 450px;height: 40px;line-height: 40px;background-color: #ffffff; }"
"\n.l_pad {width: 450px;height: 20px;line-height: 20px;}"
"\n.l_user, .l_pass {width: 150px;height: 30px;line-height: 30px;}"
"\n.l_foot1 {width: 135px;height: 30px;line-height: 30px;background-color: #ffffff;}"
"\n.l_foot2 {width: 70px;height: 30px;line-height: 30px;background-color: #ffffff;}"
"\n.l_foot3 {width: 40px;height: 30px;line-height: 30px;background-color: #ffffff;}"
"\n</style>"
"\n<script type=\"text/javascript\">"
"\nfunction $(id){return document.getElementById(id);}"
"\nfunction $G(id){return $(id).value;}"
"\nfunction $S(id, value){$(id).value = value;}"
"\nfunction $W(obj){document.write(obj);}"
"\nfunction WebLoadString(){"
"\ntry{"
"\ndocument.getElementById(\"port0_type\").selectedIndex = eval(\"DataString.port0_type\");"
"\ndocument.getElementById(\"port0_active\").selectedIndex = eval(\"DataString.port0_active\");"
"\ndocument.getElementById(\"port1_type\").selectedIndex = eval(\"DataString.port1_type\");"
"\ndocument.getElementById(\"port1_active\").selectedIndex = eval(\"DataString.port1_active\");"
"\ndocument.getElementById(\"port2_type\").selectedIndex = eval(\"DataString.port2_type\");"
"\ndocument.getElementById(\"port2_active\").selectedIndex = eval(\"DataString.port2_active\");"
"\ndocument.getElementById(\"port3_type\").selectedIndex = eval(\"DataString.port3_type\");"
"\ndocument.getElementById(\"port3_active\").selectedIndex = eval(\"DataString.port3_active\");"
"\ndocument.getElementById(\"port4_type\").selectedIndex = eval(\"DataString.port4_type\");"
"\ndocument.getElementById(\"port4_active\").selectedIndex = eval(\"DataString.port4_active\");"
"\ndocument.getElementById(\"port5_type\").selectedIndex = eval(\"DataString.port5_type\");"
"\ndocument.getElementById(\"port5_active\").selectedIndex = eval(\"DataString.port5_active\");"
"\n}catch(error){}}"
"\nfunction XWebInit(){"
"\n$(\"XForm\").action = \"/portconfig\";"
"\nWebLoadString();}"
"\nfunction XDoSubmit(){"
"\n$(\"XForm\").action = \"/portconfig\";"
"\n$(\"XForm\").submit();}"
"\nfunction XDoClear(){"
"\nWebLoadString();"
"\nreturn true;}</script>"
"\n</head>"
"\n<body id=\"x_root_body\" onload=\"XWebInit();\">"
"\n<div style=\"height: 10px\"></div>"
"\n<div id=\"l_content\">"
"\n<form id=\"XForm\" name=\"XForm\" method=\"post\" action=\"\">"
"\n<input type=\"hidden\" id=\"XWebPageName\" name=\"PortConfig\" value=\"Save\">"
"\n<ul>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS2\">Port Name</span></li>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS2\">Port Type</span></li>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS2\">Active</span></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS2\">Port 0:</span></li>"
"\n<li class=\"l_pass\"><select disabled id=\"port0_type\" name=\"port0_type\">"
"\n<option value=\"0\">None</option>"
"\n<option value=\"1\">Exit Button</option>"
"\n<option value=\"2\">Entry Reader</option>"
"\n<option value=\"3\">Exit Reader</option>"
"\n<option value=\"4\">Entry Keypad</option>"
"\n<option value=\"5\">Exit Keypad</option>"
"\n<option value=\"6\">Door Detect</option>"
"\n<\select>"
"\n</li>"
"\n<li>"
"\n<li class=\"l_pass\"><select id=\"port0_active\" name=\"port0_active\">"
"\n<option value=\"0\">Disable</option>"
"\n<option value=\"1\">Enable</option>"
"\n<\select>"
"\n</li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS2\">Port 1:</span></li>"
"\n<li class=\"l_pass\"><select disabled id=\"port1_type\" name=\"port1_type\">"
"\n<option value=\"0\">None</option>"
"\n<option value=\"1\">Exit Button</option>"
"\n<option value=\"2\">Entry Reader</option>"
"\n<option value=\"3\">Exit Reader</option>"
"\n<option value=\"4\">Entry Keypad</option>"
"\n<option value=\"5\">Exit Keypad</option>"
"\n<option value=\"6\">Door Detect</option>"
"\n<\select>"
"\n</li>"
"\n<li>"
"\n<li class=\"l_pass\"><select id=\"port1_active\" name=\"port1_active\">"
"\n<option value=\"0\">Disable</option>"
"\n<option value=\"1\">Enable</option>"
"\n<\select>"
"\n</li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS2\">Port 2:</span></li>"
"\n<li class=\"l_pass\"><select id=\"port2_type\" name=\"port2_type\">"
"\n<option value=\"0\">None</option>"
"\n<option value=\"1\">Exit Button</option>"
"\n<option value=\"2\">Entry Reader</option>"
"\n<option value=\"3\">Exit Reader</option>"
"\n<option value=\"4\">Entry Keypad</option>"
"\n<option value=\"5\">Exit Keypad</option>"
"\n<option value=\"6\">Door Detect</option>"
"\n<\select>"
"\n</li>"
"\n<li>"
"\n<li class=\"l_pass\"><select id=\"port2_active\" name=\"port2_active\">"
"\n<option value=\"0\">Disable</option>"
"\n<option value=\"1\">Enable</option>"
"\n<\select>"
"\n</li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS2\">Port 3:</span></li>"
"\n<li class=\"l_pass\"><select id=\"port3_type\" name=\"port3_type\">"
"\n<option value=\"0\">None</option>"
"\n<option value=\"1\">Exit Button</option>"
"\n<option value=\"2\">Entry Reader</option>"
"\n<option value=\"3\">Exit Reader</option>"
"\n<option value=\"4\">Entry Keypad</option>"
"\n<option value=\"5\">Exit Keypad</option>"
"\n<option value=\"6\">Door Detect</option>"
"\n<\select>"
"\n</li>"
"\n<li>"
"\n<li class=\"l_pass\"><select id=\"port3_active\" name=\"port3_active\">"
"\n<option value=\"0\">Disable</option>"
"\n<option value=\"1\">Enable</option>"
"\n<\select>"
"\n</li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS2\">Port 4:</span></li>"
"\n<li class=\"l_pass\"><select id=\"port4_type\" name=\"port4_type\">"
"\n<option value=\"0\">None</option>"
"\n<option value=\"1\">Exit Button</option>"
"\n<option value=\"2\">Entry Reader</option>"
"\n<option value=\"3\">Exit Reader</option>"
"\n<option value=\"4\">Entry Keypad</option>"
"\n<option value=\"5\">Exit Keypad</option>"
"\n<option value=\"6\">Door Detect</option>"
"\n<\select>"
"\n</li>"
"\n<li>"
"\n<li class=\"l_pass\"><select id=\"port4_active\" name=\"port4_active\">"
"\n<option value=\"0\">Disable</option>"
"\n<option value=\"1\">Enable</option>"
"\n<\select>"
"\n</li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pass\"><span style=\"width: 150px;\" id=\"XS2\">Port 5:</span></li>"
"\n<li class=\"l_pass\"><select id=\"port5_type\" name=\"port5_type\">"
"\n<option value=\"0\">None</option>"
"\n<option value=\"1\">Exit Button</option>"
"\n<option value=\"2\">Entry Reader</option>"
"\n<option value=\"3\">Exit Reader</option>"
"\n<option value=\"4\">Entry Keypad</option>"
"\n<option value=\"5\">Exit Keypad</option>"
"\n<option value=\"6\">Door Detect</option>"
"\n<\select>"
"\n</li>"
"\n<li>"
"\n<li class=\"l_pass\"><select id=\"port5_active\" name=\"port5_active\">"
"\n<option value=\"0\">Disable</option>"
"\n<option value=\"1\">Enable</option>"
"\n<\select>"
"\n</li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_pad\"></li>"
"\n</ul>"
"\n<ul>"
"\n<li class=\"l_foot1\"></li>"
"\n<li class=\"l_foot2\"><input id=\"Save\" name=\"Save\" type=\"button\" value=\"Save\" onclick=\"XDoSubmit();\"></li>"
"\n<li class=\"l_foot3\"></li>"
"\n<li class=\"l_foot2\"><input id=\"clear\" name=\"Clear\" type=\"button\" value=\"Clear\" onclick=\"XDoClear();\"></li>"
"\n<li class=\"l_foot1\"></li>"
"\n</ul>"
"\n</form>"
"\n</div>"
"\n</body>"
"\n</html>"
};

const struct fsdata_file file__common_js[] = { {
file_NULL,
data__script_common_js,
data__script_common_js + 18,
sizeof(data__script_common_js) - 19,
0,
}};

const struct fsdata_file file__style_css[] = { {
file__common_js,
data__style_css,
data__style_css + 17,
sizeof(data__style_css) - 18,
0,
}};

const struct fsdata_file file__404_html[] = { {
file__style_css,
data__404_html,
data__404_html + 12,
sizeof(data__404_html) - 12,
1,
}};
const struct fsdata_file file__reboot_html[] = { {
file__404_html,
data__reboot_html,
data__reboot_html + 13,
sizeof(data__reboot_html) - 13,
0,
}};
const struct fsdata_file file__maintain_html[] = { {
file__reboot_html,
data__maintain_html,
data__maintain_html + 15,
sizeof(data__maintain_html) - 15,
0, 
}};
const struct fsdata_file file__hostconfig_html[] = { {
file__maintain_html,
data__hostconfig_html,
data__hostconfig_html + 17,
sizeof(data__hostconfig_html) - 17,
0,
}};
const struct fsdata_file file__lanconfig_html[] = { {
file__hostconfig_html,
data__lanconfig_html,
data__lanconfig_html + 16,
sizeof(data__lanconfig_html) - 16,
0,
}};

const struct fsdata_file file__portconfig_html[] = { {
file__lanconfig_html,
data__portconfig_html,
data__portconfig_html + 17,
sizeof(data__portconfig_html) - 17,
1,
}};

const struct fsdata_file file__index_html[] = { {
file__portconfig_html,
data__index_html,
data__index_html + 12,
sizeof(data__index_html) - 12,
0,
}};

const struct fsdata_file file__login_html[] = { {
file__index_html,
data__login_html,
data__login_html + 12,
sizeof(data__login_html) - 12,
1,
}};


#define FS_ROOT file__login_html
#define FS_NUMFILES 9

