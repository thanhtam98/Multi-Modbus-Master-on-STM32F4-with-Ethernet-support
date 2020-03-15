function $(id){return document.getElementById(id);}
function $G(id){return $(id).value;}
function $S(id, value){$(id).value = value;}
function $W(obj){document.write(obj);}
function isNameUnsafe(compareChar){
var unsafeString = "\\\"<>%\\\\^[]`\\+\\$\\,='#&@.: \\t";
if ( unsafeString.indexOf(compareChar) == -1&& compareChar.charCodeAt(0) > 32
&& compareChar.charCodeAt(0) < 123 ){return false;}
else{return true;}}
function isWifiNameUnsafe(compareChar){
var unsafeString = "\\\"<>%\\\\^[]`\\+\\$\\,='#&@.:\\t";
if ( unsafeString.indexOf(compareChar) == -1&& compareChar.charCodeAt(0) >= 32
&& compareChar.charCodeAt(0) < 123 ){return false;}
else{return true;}}
function isPppoeNameUnsafe(compareChar){
var unsafeString = "\\\"\\\\\\': \\t\\n";
if ( unsafeString.indexOf(compareChar) == -1&& compareChar.charCodeAt(0) > 32
&& compareChar.charCodeAt(0) < 123 ){return false;}
else{return true;}}
function isValidIpChar(ipchar){
var str="0123456789abcdefABCDEF ";var i = 0;
if (ipchar.length == 0 || ipchar.length > 4){return false;}
for(i = 0; i < ipchar.length; i++){
if (str.indexOf(ipchar.charAt(i)) < 0){return false;}}
return true;}
function isValidSuffix(address){var j = 0;
if (address == "0:0:0:0" || address == "FFFF:FFFF:FFFF:FFFF"
|| address == "ffff:ffff:ffff:ffff")return false;
var ipParts = address.split(':');
if (ipParts.length != 4){return false;}
else{for(j = 0; j < 4; j++){
if(false == isValidIpChar(ipParts[j])){return false;}}}
return true;}
function isValidIpv6Prefix(prefix){
var parts1=prefix.split('/');
if(parts1.length > 2)return false;
var part2=prefix.split('::');
if(part2.length !=2)return false;
var ipParts = part2[0].split(':');
if(ipParts.length > 4|| part2[0]=='')return false;
return true;}
function compareIpv6Addr(ipv6addrmin, ipv6addrmax){
var ip6addr1 = ipv6addrmin.split(':');
var ip6addr2 = ipv6addrmax.split(':');
for(i = 0; i < 4; i++){
if (parseInt(ip6addr1[i], 16) < parseInt(ip6addr2[i], 16)) {return 1;}
if (parseInt(ip6addr1[i], 16) > parseInt(ip6addr2[i], 16)) {return -1;}}
return 0;}
function isValidIpAddress(address){
ipParts = address.split('/');
if (ipParts.length > 2) return false;
if (ipParts.length == 2) {num = parseInt(ipParts[1]);if (num <= 0 || num > 32)return false;}
if (ipParts[0] == '0.0.0.0' || ipParts[0] == '255.255.255.255')return false;
addrParts = ipParts[0].split('.');
if ( addrParts.length != 4 ) return false;
for (i = 0; i < 4; i++) {
if (isNaN(addrParts[i]) || addrParts[i] ==" ")return false;
num = parseInt(addrParts[i]);
if ( num < 0 || num >= 255 )return false;}
return true;}
function isValidIpAddress6(address){ipParts = address.split('/');
if (ipParts.length > 2) return false;
if (ipParts.length == 2) {num = parseInt(ipParts[1]);if (num <= 0 || num > 128)return false;}
addrParts = ipParts[0].split(':');
if (addrParts.length < 3 || addrParts.length > 8)return false;
for (i = 0; i < addrParts.length; i++) {
if ( addrParts[i] != " " )num = parseInt(addrParts[i], 16);
if ( i == 0 ) {}
else if ( (i + 1) == addrParts.length) {
if ( num == 0 || num == 1)return false;}
if ( num != 0 )break;}
return true;}
function isValidName(name){var i = 0;
for ( i = 0; i < name.length; i++ ) {
if ( isNameUnsafe(name.charAt(i)) == true )return false;}
return true;}
function isWifiValidName(name){var i = 0;
for ( i = 0; i < name.length; i++ ) {
if ( isWifiNameUnsafe(name.charAt(i)) == true )return false;}
return true;}
function isValidPppoeName(name){var i = 0;
for ( i = 0; i < name.length; i++ ) {
if ( isPppoeNameUnsafe(name.charAt(i)) == true )return false;}
return true;}
function isValidIpAddress(address){
if ( address== '0.0.0.0' || address == '255.255.255.255' )return false;
var addParts=new Array();
addrParts = address.split('.');
if ( addrParts.length != 4 ) return false;
for (i = 0; i < 4; i++) {
if (isNaN(addrParts[i]) || addrParts[i] ==" ")return false;
num = parseInt(addrParts[i]);
if ( num < 0 || num >= 255 )return false;}
return true;}
function isSameSubNet(lan1Ip, lan1Mask, lan2Ip, lan2Mask){
var count = 0;lan1a = lan1Ip.split('.');lan1m = lan1Mask.split('.');
lan2a = lan2Ip.split('.');lan2m = lan2Mask.split('.');
for (i = 0; i < 4; i++) {
l1a_n = parseInt(lan1a[i]);l1m_n = parseInt(lan1m[i]);
l2a_n = parseInt(lan2a[i]);l2m_n = parseInt(lan2m[i]);
if ((l1a_n & l1m_n) == (l2a_n & l2m_n))count++;}
if (count == 4)return true;
else return false;}
function getLeftMostZeroBitPos(num){var i = 0;
var numArr = [128, 64, 32, 16, 8, 4, 2, 1];
for ( i = 0; i < numArr.length; i++ )
if ( (num & numArr[i]) == 0 )return i;
return numArr.length;}
function getRightMostOneBitPos(num){
var i = 0;var numArr = [1, 2, 4, 8, 16, 32, 64, 128];
for ( i = 0; i < numArr.length; i++ )
if ( ((num & numArr[i]) >> i) == 1 )return (numArr.length - i - 1);
return -1;}
function isValidSubnetMask(mask){
var i = 0, num = 0;var zeroBitPos = 0, oneBitPos = 0;
var zeroBitExisted = false;
if ( mask == '0.0.0.0' )return false;
maskParts = mask.split('.');
if ( maskParts.length != 4 ) return false;
for (i = 0; i < 4; i++) {
if ( isNaN(maskParts[i]) == true )return false;
num = parseInt(maskParts[i]);
if ( num < 0 || num > 255 )return false;
if ( zeroBitExisted == true && num != 0 )return false;
zeroBitPos = getLeftMostZeroBitPos(num);
oneBitPos = getRightMostOneBitPos(num);
if ( zeroBitPos < oneBitPos )return false;
if ( zeroBitPos < 8 )zeroBitExisted = true;}
return true;}
function IsIpaddr(ip){
var c;var subip = ip.split(".");
if (4 != subip.length){return false;}
if (ip == "0.0.0.0"){return true;}
if (ip.length < 7 || ip.length > 15){return false;}
for (var i = 0; i < ip.length; i++){c = ip.charCodeAt(i);
if (c == '.'.charCodeAt(0)){continue;}
else{if (c < 0x30 || c > 0x39){return false;}}}
for (var i = 0; i < 4; i++){
if (subip[i].length == 0){return false;}
var n = parseInt(subip[i]);
if (n < 0 || n > 255){return false;}}
return true;}
function CheckIpAddr(id){
var ipaddr = $(id).value;
if (false == IsIpaddr(ipaddr)){$(id).focus();return false;}
return true;}
function IP2Int(ip){
var sip = ip.split(".");
if (sip.length != 4){return 0;}
var n = parseInt(sip[0])*256*256*256;
n += parseInt(sip[1])*256*256;
n += parseInt(sip[2])*256;
n += parseInt(sip[3]);
return n;}
function CheckNetmask(id){
if (false == CheckIpAddr(id)){return false;}
var nip = IP2Int($(id).value);var zero = true;
for (var i = 0; i < 32; i++){
if (zero == false){if ((nip&(1<<i)) == 0){$(id).focus();return false;}}
else{if ((nip&(1<<i)) != 0){zero = false;}}}
return true;}
function IsPort(port){var c;
if (port.length < 1 || port.length > 5){return false;}
for (var i = 0; i < port.length; i++){
c = port.charCodeAt(i);
if (c < 0x30 || c > 0x39){return false;}}
var n = parseInt(port);
if (n < 0 || n > 65535){return false;}
return true;}
function CheckPort(id){var port = $(id).value;
if (false == IsPort(port)){$(id).focus();return false;}
return true;}
function isHexaDigit(digit){
    var hexVals = new Array("0  ",   "1  ",   "2  ",   "3  ",   "4  ",   "5  ",   "6  ",   "7  ",   "8  ",   "9  ",   "A  ",   "B  ",   "C  ",   "D  ",   "E  ",   "F  ",   "a  ",   "b  ",   "c  ",   "d  ",   "e  ",   "f  ");
    var len = hexVals.length;var i = 0;var ret = false;
for (i = 0; i < len; i++){
if ( digit == hexVals[i] ){ret = true;break;}}
return ret;}
function isValidHexKey(val, size){
var ret = false;
if (val.length == size) {
for ( i = 0; i < val.length; i++ ) {
if ( isHexaDigit(val.charAt(i)) == false ) {break;}}
if ( i == val.length ) {ret = true;}}
return ret;}
function KeyPressIP(c){var ValidString = ".0123456789"
return (-1 != ValidString.indexOf(c))?true:false;}
function KeyPressNUM(c){
var ValidString = "0123456789";
return (-1 != ValidString.indexOf(c))?true:false;}
function KeyPressHEX(c){
var ValidString = "0123456789abcdefABCDEF";
return (-1 != ValidString.indexOf(c))?true:false;}
function KeyPressID(c){
var InvalidString = "\\\"\\\\\\': \\t\\n";
return (-1 == InvalidString.indexOf(c))?true:false;}
function KeyPressMAC(c){
var ValidString = ":0123456789abcdefABCDEF";
return (-1 != ValidString.indexOf(c))?true:false;}
function KeyPressPasswd(c){
var InvalidString = "\\\"\\\\\\':\\t\\n";
return (-1 == InvalidString.indexOf(c))?true:false;}
function KeyPressURL(c){
var InvalidString = "\\\"\\'\\t\\n";
return (-1 == InvalidString.indexOf(c))?true:false;}
function KeyPressIPV6(c){
var InvalidString = "/:0123456789abcdefABCDEF";
return (-1 != InvalidString.indexOf(c))?true:false;}
var X_INPUT_IP = 1;
var X_INPUT_NUM = 2;
var X_INPUT_HEX = 3;
var X_INPUT_ID = 4;
var X_INPUT_MAC = 5;
var X_INPUT_PASSWD = 6;
var X_INPUT_URL = 7;
var X_INPUT_IPV6 = 8;
function OnKeyPress(e, type){
var	k = 0;var	c;
if (window.event){k = e.keyCode;}
else if (e.which){k = e.which;}
if (k == 8 || k == 0){return true;}
c = String.fromCharCode(k);
switch (type){
case X_INPUT_IP:return KeyPressIP(c);
case X_INPUT_NUM:return KeyPressNUM(c);
case X_INPUT_HEX:return KeyPressHEX(c);
case X_INPUT_ID:return KeyPressID(c);
case X_INPUT_MAC:return KeyPressMAC(c);
case X_INPUT_PASSWD:return KeyPressPasswd(c);
case X_INPUT_URL:return KeyPressURL(c);
case X_INPUT_IPV6:return KeyPressIPV6(c);}
return false;}
function ChangeEnable(var_name){
var element = document.getElementById(var_name);
var element_enable = document.getElementById(var_name+"enable");
if (element.checked==true){element_enable.value="1";return true;}
else{element_enable.value="0";return false;}}
function isValidMacAddress(address) {
var c = '';var num = 0;var i = 0, j = 0;var zeros = 0;
addrParts = address.split(':');
if ( addrParts.length != 6 ) return false;
for (i = 0; i < 6; i++) {
if ( addrParts[i] == '' )return false;
for ( j = 0; j < addrParts[i].length; j++ ) {
c = addrParts[i].toLowerCase().charAt(j);
if ( (c >= '0' && c <= '9') ||(c >= 'a' && c <= 'f'))continue;
else return false;}
num = parseInt(addrParts[i], 16);
if ( num == NaN || num < 0 || num > 255 )return false;
if ( num == 0 )zeros++;}
if (zeros == 6)return false;
if ( parseInt(addrParts[0], 16) & 1 )return false;
return true;}
function samesubnet(ip, mask){
var sip = ip.split(".");
var smask = mask.split(".");
if ((sip.length != 4) || (smask.length != 4)){return false;}
if ((parseInt(sip[0]) != (parseInt(sip[0]) & parseInt(smask[0]))) || 
(parseInt(sip[1]) != (parseInt(sip[1]) & parseInt(smask[1]))) || 
(parseInt(sip[2]) != (parseInt(sip[2]) & parseInt(smask[2]))) || 
(parseInt(sip[3]) != (parseInt(sip[3]) & parseInt(smask[3])))){return false;}
else{return true;}}
