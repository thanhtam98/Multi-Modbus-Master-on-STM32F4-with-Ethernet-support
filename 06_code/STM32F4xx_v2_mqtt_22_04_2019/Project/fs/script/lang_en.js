var MenuString = new Object();

MenuString.status = "Status";
MenuString.devinfo = "Device Information";
MenuString.laninfo = "LAN Status";
MenuString.waninfo = "WAN Status";
MenuString.landev = "Home Networking";
MenuString.poninfo = "Optics Module status";
MenuString.cpuinfo = "CPU Information";
MenuString.wificlient = "Wireless Client";
MenuString.unknowndevice = "Unknown device";

MenuString.network = "Network";
MenuString.lan = "LAN";
MenuString.wan = "WAN";
MenuString.wifi = "WiFi";
MenuString.ssid = "SSID";
MenuString.route = "Routing";
MenuString.dns_host = "DNS";
MenuString.tr069 = "TR-069";
MenuString.dscp = "DSCP Remark";
MenuString.ip_qos = "Qos";

MenuString.security = "Security";
MenuString.firewall = "Firewall";
MenuString.mac_filter = "Mac Filter";
MenuString.ip_filter = "IP Filter";
MenuString.url_filter = "URL Filter";
MenuString.dmz_alg = "DMZ and ALG";

MenuString.application = "Application";
MenuString.nat = "Port Forwarding";
MenuString.ddns = "DDNS";
MenuString.upnp = "UPNP";
MenuString.aoe = "AOE Game";
MenuString.ntp = "NTP";
MenuString.usb = "USB Storage";

MenuString.admin = "Maintain";
MenuString.password = "Password";
MenuString.slid = "SLID Configuration";
MenuString.backup = "Backup and Restore";
MenuString.upgrade = "Firmware Upgrade";
MenuString.reboot = "Reboot Device";
MenuString.init = "Factory Default";
MenuString.diag = "Diagnose";
MenuString.loopdetect = "Loop Detection";
MenuString.log = "Log";
MenuString.language = "Language";

MenuString.dns_suffix = "DNS_Suffix";

var WebString = new Object();
WebString.OntName = "Home Gateway";
WebString.Reset = "Reset";
WebString.Login = "Login";
WebString.Logout = "Logout";
WebString.Username = "Username";
WebString.Password = "Password";
WebString.PasswordTip = "Password Tip";
WebString.Save = "Save";
WebString.Refresh = "Refresh";
WebString.Add = "Add";
WebString.Edit = "SaveEdit";
WebString.Delete = "Delete";
WebString.Enable = "Enable";
WebString.Disable = "Disable";
WebString.SaveConfig = "Save Config File";
WebString.Reboot = "Reboot";
WebString.Import = "Import";
WebString.Export = "Export";
WebString.Default = "Factory Default";
WebString.Upcfg = "Scan";
WebString.Upimg = "Scan";
WebString.Upgrade = "Upgrade";
WebString.all = "ALL";
WebString.on = "on";
WebString.off = "off";
WebString.cfgerr = "The config file is not right!";
WebString.cfgok = "Restore finished, click OK for ONT restart!";

WebString.LogoutS1 = "You have been logout, please click here to ";
WebString.LogoutS2 = "login";

WebString.ErrorS1 = "Login error, please after ";
WebString.ErrorS2 = " seconds click here to ";
WebString.ErrorS3 = "login";
WebString.username_alert = "Please input username!";
WebString.password_alert = "Please input password!";
WebString.password_default = "Default web password is WiFi WEP password";
WebString.ip_alert = "Please input valid ip address!";
WebString.netmask_alert = "Please input valid subnet mask address!";
WebString.gw_alert = "Please input valid gateway ip address!";
WebString.gw_ip_alert =
  "The gateway ip address and ip address should be in the same subnet!";
WebString.leasetime_alert =
  "Please input valid dhcp lease time(0 or 2~129600 minutes)!";
WebString.dhcp_alert =
  "DHCP ip range must be the same subnet mask with LAN ip address,\nand end IP address must be greater than start IP address!";
WebString.wan_service_alert =
  "WAN connection should  bind to at lease one service!";
WebString.wan_vlan_alert = "Vlan ID and vlan priority should not be NULL!";
WebString.wan_vlanpri_alert = "Please set valid vlan priority(0~7)!";
WebString.wan_vlanId_alert = "Please set valid vlan Id (0~4095)!";
WebString.wan_opt60_value_alert = "Vendor Class ID should not be NULL!";
WebString.wan_opt60_hex_alert = "Vendor Class ID have invalide hex!";
WebString.wan_pppoe_null_alert =
  "Pppoe username and password should not be NULL!";
WebString.wan_pppoe_alert = "Please set correct pppoe username and password!";
WebString.wan_pppoe_alive_alert =
  "Please set correct pppoe keeping alive time(5~60 seconds)!";
WebString.wan_pridns_alert = "Please set valid primary dns address!";
WebString.wan_secdns_alert = "Please set valid secondary dns address!";
WebString.wan_prisec_alert = "The pri dns and sec dns should be different!";
WebString.wan_service_con_alert =
  "When the service type of wan connnection is TR-069 or VoIP,the connection mode should not be bridge.";
WebString.wan_service_check_alert =
  "ONT will reject to create 2 WAN with the same service type!";
WebString.wan_bridge_check_alert = "ONT will reject to create 2 Bridge WAN !";
WebString.wan_service_lanport_alert =
  "Only when the service type is internet or iptv,wan connection can bind to lan ports";
WebString.wan_LAN1_bind_alert =
  "LAN1 is already binding to another wan,LAN1 can not bind to this wan";
WebString.wan_LAN2_bind_alert =
  "LAN2 is already binding to another wan,LAN2 can not bind to this wan";
WebString.wan_LAN3_bind_alert =
  "LAN3 is already binding to another wan,LAN3 can not bind to this wan";
WebString.wan_LAN4_bind_alert =
  "LAN4 is already binding to another wan,LAN4 can not bind to this wan";
WebString.wan_SSID1_bind_alert =
  "SSID1 is already binding to another wan,SSID1 can not bind to this wan";
WebString.wan_SSID2_bind_alert =
  "SSID2 is already binding to another wan,SSID2 can not bind to this wan";
WebString.wan_SSID3_bind_alert =
  "SSID3 is already binding to another wan,SSID3 can not bind to this wan";
WebString.wan_SSID4_bind_alert =
  "SSID4 is already binding to another wan,SSID4 can not bind to this wan";

WebString.init_confirm =
  "Do you want to reset system configuration to factory default setting?";
WebString.reboot_confirm = "Do you want to reboot the device?";
WebString.backup_alert = "Please select the configuration file imported!";
WebString.backup_import_alert =
  "Are you sure to restore configuration file to ONT?";
WebString.backup_export_alert =
  "Are you sure to backup configuration file from ONT?";
WebString.upgrade_alert = "Please select the file you want to upgrade!";
WebString.slid_alert = "Please input valid slid value!";
WebString.slid_asc_alert =
  "SLID must be equal or less than 10 ASCII characters!";
WebString.slid_invalid_asc_alert =
  "The input slid value has invalid ascii character!";
WebString.slid_hex_alert = "SLID must be equal or less than 20 HEX characters!";
WebString.slid_invalid_hex_alert =
  "The input slid value has invalid HEX character!";
WebString.samerule = "There already exist same rule!";
WebString.natstring = "Internet wan NAT is not enabled!";
WebString.ipfilter = "Please input the ip filter rule!";
WebString.ipfilterconfirm =
  "To change the current mode will lead to empty the previous rules!";

WebString.day = "Day";
WebString.days = "Day(s)";
WebString.hour = "h";
WebString.minute = "m";
WebString.second = "s";
WebString.millisecond = "ms";
WebString.seconds = "seconds";
WebString.hours = "Hours";
WebString.minutes = "Minutes";

var XdevinfoString = new Array();
XdevinfoString[0] = "Device Name";
XdevinfoString[1] = "Serial Number";
XdevinfoString[2] = "Hardware Version";
XdevinfoString[3] = "Software Version";
XdevinfoString[4] = "Device Running Time";
XdevinfoString[5] = "Vendor";
XdevinfoString[6] = "Transmitted Optical Power";
XdevinfoString[7] = "Received Optical Power";
XdevinfoString[8] = "Transceiver Temperature";

//laninfo
var XlaninfoString = new Array();
XlaninfoString[0] = "Wireless Status";
XlaninfoString[1] = "Wireless Channel";
XlaninfoString[2] = "SSID1 Name";
XlaninfoString[3] = "Wireless Encryption Status";
XlaninfoString[4] = "Wireless Rx Packets";
XlaninfoString[5] = "Wireless Tx Packets";
XlaninfoString[6] = "Wireless Rx Bytes";
XlaninfoString[7] = "Wireless Tx Bytes";
XlaninfoString[8] = "Ethernet Status";
XlaninfoString[9] = "Ethernet IP Address";
XlaninfoString[10] = "Ethernet Subnet Mask";
XlaninfoString[11] = "Ethernet MAC Address";
XlaninfoString[12] = "Ethernet Rx Packets";
XlaninfoString[13] = "Ethernet Tx Packets";
XlaninfoString[14] = "Ethernet Rx Bytes";
XlaninfoString[15] = "Ethernet Tx Bytes";
XlaninfoString[16] = "Wireless Information";
XlaninfoString[17] = "Ethernet Information";
XlaninfoString[18] = "DHCP Client List";
XlaninfoString[19] = "Hostname";
XlaninfoString[20] = "MAC Address";
XlaninfoString[21] = "IP Address";
XlaninfoString[22] = "Remaining Time";
XlaninfoString[23] = "Connection Type";
XlaninfoString[24] = "Uptime";

//lan
var XlanString = new Array();
XlanString[0] = "IP Address";
XlanString[1] = "Subnet Mask";
XlanString[2] = "DHCP Enable";
XlanString[3] = "DHCP Start IP Address";
XlanString[4] = "DHCP End IP Address";
XlanString[5] = "DHCP Lease Time";
XlanString[6] = "(2~129600 minutes, or 0 means 1 day)minutes.";

XlanString[7] = "Bind MAC Address";
XlanString[8] = "Bind IP Address";
XlanString[9] = "Delete";
XlanString[10] = "Managed Flag";
XlanString[11] = "Other Config Flag";
XlanString[12] = "Max RA Interval";
XlanString[13] = "Min RA Interval ";
XlanString[14] = "Enable DHCP6S";
XlanString[15] = "Prefix Mode";
XlanString[16] = "Prefix Set";
XlanString[17] = "Manual";
XlanString[18] = "Second";
XlanString[19] = "Prefix";
XlanString[20] = "Suffix Start Addr";
XlanString[21] = "Suffix End Addr";
XlanString[22] = "Prefix";
XlanString[23] = "Prefix Len";
XlanString[24] = "Preferred Life time";
XlanString[25] = "Valid Life time";
XlanString[26] = "Ipv6 Link ip";
XlanString[27] = "PARENT_PREFIX";
XlanString[28] = "Enable RADVD";

WebString.lan_pre_err = "Prefix format errors, please re-enter!";

WebString.lan_ra_max_err = "The maximum period should be between 4-1800!";

WebString.lan_ra_min_err =
  "The minimum period should be 3 to 0.75 times the maximum value between!";

WebString.route_rule_err =
  "Destination IP address and destination netmask conflict!";
WebString.macfilter_rule_change =
  "Are you sure to change the policy? Continue will empty the current rules!";

WebString.bind_ip_erro_alert = "Please input available bind ip address!";
WebString.bind_mac_erro_alert = "Please input available bind mac address!";
WebString.have_bind_ip_alert = "The bind ip address has existed yet!";
WebString.have_bind_mac_alert = "The bind mac address has existed yet!";
WebString.bind_ip_subnet_alert =
  "The bind ip address and lan ip address should be in the same subnet!";

var XwanString = new Array();
XwanString[0] = "WAN Connection List";
XwanString[1] = "Create One New Connection";
XwanString[2] = "Delete";
XwanString[3] = "Enable/Disable";
XwanString[4] = "Connection Mode";
XwanString[5] = "Route";
XwanString[6] = "Bridge";
XwanString[7] = "NAT";
XwanString[8] = "Service:";
XwanString[9] = "VOIP";
XwanString[10] = "TR-069";
XwanString[11] = "INTERNET";
XwanString[12] = "IPTV";
XwanString[13] = "OTHER";
XwanString[14] = "Enable VLAN";
XwanString[15] = "VLAN ID";
XwanString[16] = "VLAN PRI";
XwanString[17] = "LAN Port Binded";
XwanString[18] = "LAN1";
XwanString[19] = "LAN2";
XwanString[20] = "LAN3";
XwanString[21] = "LAN4";
XwanString[22] = "SSID1";
XwanString[23] = "SSID2";
XwanString[24] = "SSID3";
XwanString[25] = "SSID4";
XwanString[26] = "WAN IP Mode";
XwanString[27] = "PPPoE";
XwanString[28] = "DHCP";
XwanString[29] = "Static IP";
XwanString[30] = "Username";
XwanString[31] = "Password";
XwanString[32] = "Keep Alive Time";
XwanString[33] = "(5~60) seconds";
XwanString[34] = "IPV4 Address";
XwanString[35] = "IPV4 Netmask";
XwanString[36] = "IPV4 Gateway";
XwanString[37] = "IPV4 Pri DNS";
XwanString[38] = "IPV4 Sec DNS";
XwanString[39] = "SSID Port Binded";
XwanString[40] = "WAN IP Version";
XwanString[41] = "IPV4";
XwanString[42] = "IPV6";
XwanString[43] = "IPV4+IPV6";
XwanString[44] = "IPV6 Address/Prefix";
XwanString[45] = "IA_NA";
XwanString[46] = "IA_PD";
XwanString[47] = "IA_NA_OPTION";
XwanString[48] = "IA_PD_OPTION";
XwanString[49] = "SLAAC";
XwanString[50] = "IPV6 Address";
XwanString[51] = "IPV6 Prefix Length";
XwanString[52] = "IPV6 Gateway";
XwanString[53] = "IPV6 Pri DNS";
XwanString[54] = "IPV6 Sec DNS";
XwanString[55] = "IPV6 SLAAC";
XwanString[56] = "OPTION 60 Enable";
XwanString[57] = "Vendor Class ID";
XwanString[58] = "HEX Mode";
XwanString[59] = "ASCII Mode";
XwanString[60] = "IP Port";
XwanString[61] = "PPPoE DNS Mode";
XwanString[62] = "Auto";
XwanString[63] = "Manual";
XwanString[64] = "PPPoE Pri DNS";
XwanString[65] = "PPPoE Sec DNS";

var XwaninfoString = new Array();
XwaninfoString[0] = "WAN Connection List";
XwaninfoString[1] = "No wan connection existing";
XwaninfoString[2] = "Connection Mode";
XwaninfoString[3] = "Enable/Disable";
XwaninfoString[4] = "VLAN";
XwaninfoString[5] = "IPV4 Link Status";
XwaninfoString[6] = "IP Address";
XwaninfoString[7] = "Netmask";
XwaninfoString[8] = "Gateway";
XwaninfoString[9] = "Primary DNS";
XwaninfoString[10] = "Secondary DNS";
XwaninfoString[11] = "Pon Link Status";
XwaninfoString[12] = "Up FEC Enable";
XwaninfoString[13] = "Down FEC Enable";
XwaninfoString[14] = "Tx Packets";
XwaninfoString[15] = "Rx Packets";
XwaninfoString[16] = "Tx Dropped";
XwaninfoString[17] = "Rx Dropped";
XwaninfoString[18] = "Error Packets";

XwaninfoString[19] = "IPV6 Connectivity Information";
XwaninfoString[20] = "IPV6 Connection Mode";
XwaninfoString[21] = "IPV6 Link Status";
XwaninfoString[22] = "IPV6 Address";
XwaninfoString[23] = "IPV6 Gateway";
XwaninfoString[24] = "IPV6 Prefix";
XwaninfoString[25] = "IPV6 Pri DNS";
XwaninfoString[26] = "IPV6 Sec DNS";
XwaninfoString[27] = "Active IP Connections";
XwaninfoString[28] = "Uptime";
XwaninfoString[29] = "The WAN connection is down!\n";

var XinitString = new Array();
XinitString[0] = "Factory Default";

var XrebootString = new Array();
XrebootString[0] = "Reboot";

var XbackupString = new Array();
XbackupString[0] = "Select File";
XbackupString[1] = "Import Config File";
XbackupString[2] = "Export Config File";
XbackupString[3] = "Import";
XbackupString[4] = "Export";

var XupgradeString = new Array();
XupgradeString[0] = "Select File";
XupgradeString[1] = "Upgrade";
XupgradeString[2] = "Firmware uploaded successfully!";
XupgradeString[3] =
  "System is writing firmware to flash now, please don't power off or reboot device!";
XupgradeString[4] = "Device will reboot in";
XupgradeString[5] = "120";
XupgradeString[6] = "seconds.";
XupgradeString[7] =
  "Warning: uploaded file is not a valid firmware image file, or upload error!";

//dns host
var Xdns_hostString = new Array();
Xdns_hostString[0] = "Domain Name";
Xdns_hostString[1] = "IP Address";

Xdns_hostString[2] = "Delete";
Xdns_hostString[3] = "Add";

WebString.dns_host_ip_alert = "Please input available IP!";
WebString.dns_host_domain_alert = "Please input available domain name!";
WebString.dns_host_add_rule_alert = "The input rule has been in the table!";
WebString.dns_host_rule_full_alert = "There is no space in the rule table!";

var XslidString = new Array();
XslidString[0] = "Current SLID";
XslidString[1] = "Input New SLID";
XslidString[2] = "SLID Mode";
XslidString[3] = "ASCII Mode";
XslidString[4] = "HEX Mode";
XslidString[5] = "Note";
XslidString[6] = "ASCII Mode: max to 10 ASCII characters, e.g: abcdefg123";
XslidString[7] =
  "HEX Mode: max to 20 HEX numbers, 0~9/A~F/a~f, e.g: 0x1234567890ABCDEF1234";

//Application upnp
var XupnpString = new Array();
XupnpString[0] = "Enable UPNP IGD";

//Application aoe
var XaoeString = new Array();
XaoeString[0] = "Play AOE Game";

//loop detect
var XloopdetectString = new Array();
XloopdetectString[0] = "Enable Loop Detection";
XloopdetectString[1] = "Alarm List";
XloopdetectString[2] = "LAN Port";
XloopdetectString[3] = "Alarm";

//Application ddns
var XddnsString = new Array();
XddnsString[0] = "Enable DDNS";
XddnsString[1] = "ISP";
XddnsString[2] = "Domain Name";
XddnsString[3] = "Username";
XddnsString[4] = "Password";
XddnsString[5] = "WAN Connection List";
XddnsString[6] = "Dyndns.org";
XddnsString[7] = "gnudip";
XddnsString[8] = "tzo";
XddnsString[9] = "ods";
WebString.ddns_input_alert = "Please input the value!";
XddnsString[10] = "cancle";
XddnsString[11] = "noip";
XddnsString[12] = "changeip";

//Application ntp
var XntpString = new Array();
XntpString[0] = "Enable NTP Service";
XntpString[1] = "Current Time";
XntpString[2] = "First Time Server";
XntpString[3] = "Second Time Server";
XntpString[4] = "Interval Time";
XntpString[5] = "Time Zone";
XntpString[6] = "(0-259200)seconds";
XntpString[7] = "cancle";
WebString.ntp_timeZones0 = "(GMT-12:00) International Date Line West";
WebString.ntp_timeZones1 = "(GMT-11:00) Midway Island, Samoa";
WebString.ntp_timeZones2 = "(GMT-10:00) Hawaii";
WebString.ntp_timeZones3 = "(GMT-09:00) Alaska";
WebString.ntp_timeZones4 = "(GMT-08:00) Pacific Time, Tijuana";
WebString.ntp_timeZones5 = "(GMT-07:00) Arizona, Mazatlan";
WebString.ntp_timeZones6 = "(GMT-06:00) Central America";
WebString.ntp_timeZones7 = "(GMT-05:00) Bogota, Lima, Quito";
WebString.ntp_timeZones8 = "(GMT-04:00) Atlantic Time";
WebString.ntp_timeZones9 = "(GMT-03:00) Newfoundland";
WebString.ntp_timeZones10 = "(GMT-02:00) Mid-Atlantic";
WebString.ntp_timeZones11 = "(GMT-01:00) Azores";
WebString.ntp_timeZones12 = "(GMT-00:00) Casablanca, Monrovia";
WebString.ntp_timeZones13 =
  "(GMT+01:00) Amsterdam, Berlin, Bern, Rome, Stockholm";
WebString.ntp_timeZones14 = "(GMT+02:00) Athens, Istanbul, Minsk";
WebString.ntp_timeZones15 = "(GMT+03:00) Baghdad";
WebString.ntp_timeZones16 = "(GMT+04:00) Abu Dhabi, Muscat";
WebString.ntp_timeZones17 = "(GMT+05:00) Ekaterinburg";
WebString.ntp_timeZones18 = "(GMT+06:00) Almaty, Novosibirsk";
WebString.ntp_timeZones19 = "(GMT+07:00) Bangkok, Hanoi, Jakarta";
WebString.ntp_timeZones20 = "(GMT+08:00) Beijing, Chongqing, Hong Kong, Urumqi";
WebString.ntp_timeZones21 = "(GMT+09:00) Osaka, Sapporo, Tokyo";
WebString.ntp_timeZones22 = "(GMT+10:00) Brisbane";
WebString.ntp_timeZones23 = "(GMT+11:00) Magadan";
WebString.ntp_timeZones24 = "(GMT+12:00) Auckland, Wellington";
WebString.ntp_setbyself = "Customer setting";
WebString.ntp_interval_alert = "The value should be 0-259200s!";

//Application nat
var XnatString = new Array();
XnatString[0] = "WAN Port";
XnatString[1] = "LAN Port";
XnatString[2] = "LAN IP Address";
XnatString[3] = "Protocol";
XnatString[4] = "Enable Mapping";
XnatString[5] = "WAN Connection List";
XnatString[6] = "Status";
XnatString[7] = "Delete";
XnatString[8] = "Delete";
XnatString[9] = "TCP";
XnatString[10] = "UDP";
XnatString[11] = "Both";
XnatString[12] = "Edit";

WebString.nat_wan_start_port_alert = "Please input available wan start port!";
WebString.nat_wan_end_port_alert = "Please input available wan end port!";
WebString.nat_wan_port_erro_alert =
  "The end wan port must bigger than the start port!";
WebString.nat_lan_port_erro_alert = "Please input available lan port!";
WebString.nat_lan_ip_erro_alert = "Please input available LAN ip address!";
WebString.nat_mapping_same_rule_alert =
  "The range of new wan port is overlapping with the rule:";
WebString.nat_no_wan_alert = "No internet route wan connection existing!";

//Application usb
var XusbString = new Array();
XusbString[0] = "Enable FTP Server";
XusbString[1] = "Username";
XusbString[2] = "Password";
XusbString[3] = "Re-enter Password";

XusbString[4] = "HOST NUM";
XusbString[5] = "DEV NUM";
XusbString[6] = "Format";

WebString.usb_user_name_alert = "Please Input Username!";
WebString.usb_password_diff_alert = "Please Confirm Password!";
WebString.usb_password_len_alert = "Please Input Password!";

//password
var XpasswordString = new Array();
XpasswordString[0] = "Original Password";
XpasswordString[1] = "New Password";
XpasswordString[2] = "Re-enter Password";
XpasswordString[3] = "Prompt Message";
WebString.password_erro_message = "Original password error,please input again!";
WebString.password_null_alert = "Please input original password!";
WebString.password_len_alert = "The length of new password should be 5~24!";
WebString.password_diff_alert = "Please confirm password!";

//diag
var XdiagString = new Array();
XdiagString[0] = "TR-069 Report";
XdiagString[1] = "WAN Connect List";
XdiagString[2] = "IP or Domain Name";
XdiagString[3] = "Test";
WebString.diag_choose_alert = "Please choose ping or traceroute.";
WebString.diag_result_value = "ping/traceroute testing in progress";
WebString.diag_no_wan_connect = "No wan connection online.";
WebString.diag_start = "start test";
WebString.diag_display = "show result";
WebString.diag_ipv4_addr_err = "This is not a valid IPv4 address!";
WebString.diag_ipv6_addr_err = "This is not a valid IPv6 address!";

XdiagString[4] = "ping";
XdiagString[5] = "traceroute";
XdiagString[6] = "LAN/WLAN Interface";
XdiagString[7] = "IPV4 Test";
XdiagString[8] = "IPV6 Test";

var XrouteString = new Array();
XrouteString[0] = "Destination IP Address";
XrouteString[1] = "Destination Netmask";
XrouteString[2] = "Gateway";
XrouteString[3] = "Interface";
XrouteString[4] = "IP Protocol Version";
XrouteString[5] = "IPV4 Interface";
XrouteString[6] = "IPV6 Interface";
XrouteString[7] = "Static Route List Table";

var XfirewallString = new Array();
XfirewallString[0] = "Security Level";
XfirewallString[1] = "Low";
XfirewallString[2] = "Middle";
XfirewallString[3] = "High";
XfirewallString[4] = "Attack Protect";
XfirewallString[5] = "Remote Https Enabled";
XfirewallString[6] = "Remote Telnet Enabled";
XfirewallString[7] = "Request Attrack Protect";
XfirewallString[8] = "Redirect Attrack Protect";
XfirewallString[9] = "Land Attrack Protect";
var Xmac_filterString = new Array();
Xmac_filterString[0] = "White";
Xmac_filterString[1] = "Black";
Xmac_filterString[2] = "Mode";
Xmac_filterString[3] = "Mac Address";
Xmac_filterString[4] = "Please input valid mac address";
Xmac_filterString[5] = "The rule has existed yet";
Xmac_filterString[6] = "Enable Mac Filter";
Xmac_filterString[7] = "Mac Filter Mode";
Xmac_filterString[8] = "Mac Filter Table";
Xmac_filterString[9] = "Accept";
Xmac_filterString[10] = "Deny";
Xmac_filterString[11] = "Default Policy";
Xmac_filterString[12] = "Mac address format: xx:xx:xx:xx:xx:xx(0-9, a-f, A-F)";

var Xip_filterString = new Array();
Xip_filterString[0] = "Mode";
Xip_filterString[1] = "Source Start IP Address";
Xip_filterString[2] = "Source End IP Address";
Xip_filterString[3] = "Source Start Port";
Xip_filterString[4] = "Source End Port";
Xip_filterString[5] = "Destination Start IP Address";
Xip_filterString[6] = "Destination End IP Address";
Xip_filterString[7] = "Destination Start Port";
Xip_filterString[8] = "Destination End Port";
Xip_filterString[9] = "Protocol";
Xip_filterString[10] = "Accept";
Xip_filterString[11] = "Drop";
Xip_filterString[12] = "Please enable the ip filter!";
Xip_filterString[13] =
  "Source IP address and dst ip address can't be empty at the same time!";
Xip_filterString[14] = "Start IP address must be set before end ip address!";
Xip_filterString[15] = "Please input right port:0-65535!";
Xip_filterString[16] = "Start address must be litter than end address!";
Xip_filterString[17] = "Start port must be litter than end port!";
Xip_filterString[18] = "The rule has existed yet!";
Xip_filterString[19] = "Enable IP Filter";
Xip_filterString[20] = "ALL";
Xip_filterString[21] = "TCP";
Xip_filterString[22] = "UDP";
Xip_filterString[23] = "TCP/UDP";
Xip_filterString[24] = "ICMP";

var XdscpString = new Array();
XdscpString[0] = "Enable Queue Management";
XdscpString[1] = "Enable Queue Classification";
XdscpString[2] = "Classification Order";
XdscpString[3] = "Wan Interface";
XdscpString[4] = "Destination IP Address";
XdscpString[5] = "Destination Netmask";
XdscpString[6] = "Protocol";
XdscpString[7] = "Destination Start Port";
XdscpString[8] = "Destination End Port";
XdscpString[9] = "DSCP Remark";
XdscpString[10] = "ALL";
XdscpString[11] = "TCP";
XdscpString[12] = "UDP";
XdscpString[13] = "ICMP";
XdscpString[14] = "Enable";
XdscpString[15] = "Disable";
XdscpString[16] =
  "Since you selected destination netmask, please input destination ip address";
XdscpString[17] = "Please input right port:0-65535!";
XdscpString[18] = "Start port must be litter than end port!";
XdscpString[19] = "Please input right dscp value: 0-63!";
XdscpString[20] =
  "DSCP order should not be the same with other dscp remark rules, please select a different order!";
XdscpString[21] = "You should select at lease one classification rule";
XdscpString[22] = "The rule has existed yet!";

var XwifiString = new Array();
XwifiString[0] =
  "The encrypt mode is not the wpa/wpa2 personal, you can't enable wps!";
XwifiString[1] = "When enable wps, The encrypt mode must be wpa/wpa2 personal!";
XwifiString[2] = "SSID name should not be NULL!";
XwifiString[3] = "SSID name have invalid character!";
XwifiString[4] = "WEP key must be 10 hex or 5 charactors!";
XwifiString[5] = "WEP key have invalide hex!";
XwifiString[6] = "WEP key must be 26 hex or 13 charactors!";
XwifiString[7] = "WPA preshared key must be 8-64 length!";
XwifiString[8] = "The radius server should be valid IP address!";
XwifiString[9] = "The port should not be NULL!";
XwifiString[10] = "The length of pin code must be 8 number!";
XwifiString[11] = "Please enable wps first!";
XwifiString[12] = "Mode";
XwifiString[13] = "auto(b/g/n)";
XwifiString[14] = "b";
XwifiString[15] = "g";
XwifiString[16] = "n";
XwifiString[17] = "b/g";
XwifiString[18] = "Channel";
XwifiString[19] = "Auto";
XwifiString[20] = "Rate";
XwifiString[21] = "Transmitting Power";
XwifiString[22] = "SSID Name";
XwifiString[23] = "Enable SSID";
XwifiString[24] = "SSID Broadcast";
XwifiString[25] = "SSID Isolate";
XwifiString[26] = "WiFi Mac Filter";
XwifiString[27] = "Enable WPS";
XwifiString[28] = "WPS Mode";
XwifiString[29] = "PBC";
XwifiString[30] = "PIN";
XwifiString[31] = "PIN Code Number";
XwifiString[32] = "WPS Connect";
XwifiString[33] = "Encrypt Mode";
XwifiString[34] = "OPEN";
XwifiString[35] = "WEP";
XwifiString[36] = "WPA/WPA2 Personal";
XwifiString[37] = "WPA/WPA2 Enterprise";
XwifiString[38] = "WEP Encryption Mode";
XwifiString[39] = "Open System";
XwifiString[40] = "Shared Key";
XwifiString[41] = "WEP Encryption Level";
XwifiString[42] = "64bit";
XwifiString[43] = "128bit";
XwifiString[44] = "WEP Key";
XwifiString[45] = "WPA Version";
XwifiString[46] = "WPA Encryption Mode";
XwifiString[47] = "WPA Key";
XwifiString[48] = "RADIUS Server";
XwifiString[49] = "RADIUS Port";
XwifiString[50] = "NONE";
XwifiString[51] = "SSID Select";
XwifiString[52] = "Channel Width";
XwifiString[53] = "Enable";
XwifiString[54] = "Short Guard Interval";
XwifiString[55] = "RTS/CTS Threshold";
XwifiString[56] = "RTS/CTS Threshold invalid value";
XwifiString[57] = "Max Clients";
XwifiString[58] = "(value:0~16, 0 represent disable wifi portal function)";

var Xdmz_algString = new Array();
Xdmz_algString[0] = "ALG Config";
Xdmz_algString[1] = "FTP";
Xdmz_algString[2] = "TFTP";
Xdmz_algString[3] = "SIP";
Xdmz_algString[4] = "H323";
Xdmz_algString[5] = "RTSP";
Xdmz_algString[6] = "L2TP";
Xdmz_algString[7] = "IPSEC";
Xdmz_algString[8] = "Enable DMZ";
Xdmz_algString[9] = "DMZ IP Address";
Xdmz_algString[10] = "DMZ Config";
Xdmz_algString[11] = "WAN Connection List";
WebString.dmz_alg_no_wan = "No wan connection list";
WebString.dmz_alg_wan_alert =
  "There is no wan connection list, please create internet wan connection first.";

var XlogString = new Array();
XlogString[0] = "Writing Level";
XlogString[1] = "Reading Level";
XlogString[2] = "Emergency";
XlogString[3] = "Alert";
XlogString[4] = "Critical";
XlogString[5] = "Error";
XlogString[6] = "Warning";
XlogString[7] = "Notice";
XlogString[8] = "Informational";
XlogString[9] = "Debug";
XlogString[10] = "Emergency";
XlogString[11] = "Alert";
XlogString[12] = "Critical";
XlogString[13] = "Error";
XlogString[14] = "Warning";
XlogString[15] = "Notice";
XlogString[16] = "Informational";
XlogString[17] = "Debug";

var Xtr069String = new Array();
Xtr069String[0] = "Periodic Inform Enable";
Xtr069String[1] = "Periodic Inform Interval(s)";
Xtr069String[2] = "URL";
Xtr069String[3] = "Username";
Xtr069String[4] = "Password";
Xtr069String[5] = "Connect Request URL";
Xtr069String[6] = "Connect Request Username";
Xtr069String[7] = "Connect Request Password";
Xtr069String[8] = "ONU Logic ID";

var XlanguageString = new Array();
XlanguageString[0] = "Select Language";
XlanguageString[1] = "English";
XlanguageString[2] = "Spanish";

var mode_list = new Array("Bridge", "Dynamic DHCP", "PPPoE", "STATIC IP");
var status_list = new Array("Down", "UP", "linking", "Mode changed");
var ponSta_list = new Array(
  "",
  "Initial State",
  "Standby State",
  "Serial Number State",
  "Ranging State",
  "Operation State",
  "POPUP State",
  "Emergency Stop State"
);

var XlandevString = new Array();
XlandevString[0] = "Local Interface";
XlandevString[1] = "Connection Type";
XlandevString[2] = "Connected Devices";
XlandevString[3] = "Setting";
XlandevString[4] = "Ethernet";
XlandevString[5] = "Wireless";
XlandevString[6] = "setting";
XlandevString[7] = "Wireless Settings";
XlandevString[8] = "Network Name";
XlandevString[9] = "Access Point";
XlandevString[10] = "Local Devices";
XlandevString[11] = "Connection Type";
XlandevString[12] = "Device Name";
XlandevString[13] = "IP Address";
XlandevString[14] = "Hardware Address";
XlandevString[15] = "IP Address Allocation";
XlandevString[16] = "DHCP(Private NAT)";
XlandevString[17] = "Manual(Private NAT)";
XlaninfoString[18] = "Ethernet IPV6 Link Address";

var XponinfoString = new Array();
XponinfoString[0] =
  "Laser Bias Current(ONT ANI-ONT-Side Optical Measurements):";
XponinfoString[1] =
  "Optics Module Vlotage(ONT ANI-ONT-Side Optical Measurements):";
XponinfoString[2] =
  "Optics Module Temperature(ONT ANI-ONT-Side Optical Measurements):";
XponinfoString[3] =
  "Rx Optics Signal Level at 1490 nm(ONT ANI-ONT-Side Optical Measurements):";
XponinfoString[4] =
  "Tx Optics Signal Level at 1310 nm(ONT ANI-ONT-Side Optical Measurements):";
XponinfoString[5] =
  " Lower(ONT ANI-ONT-Side Optical Measurements-Optical Threshold):";
XponinfoString[6] =
  "Upper(ONT ANI-ONT-Side Optical Measurements-Optical Threshold):";

var Xdns_suffixString = new Array();
Xdns_suffixString[0] = "Domain";
Xdns_suffixString[1] = "Routed wan";
Xdns_suffixString[2] = "Delete";
Xdns_suffixString[3] = "Add";
WebString.dns_suffix_domain_alert = "Please input valid domain suffix";
WebString.dns_suffix_wan_alert =
  "There is no routed wan existed, no rule can be added";

var XcpuinfoString = new Array();
XcpuinfoString[0] = "Processor Utilization";
XcpuinfoString[1] = "Used Memory";

var XwificlientString = new Array();
XwificlientString[0] = "Total Clients";
XwificlientString[1] = "MAC Address";

var Xurl_filterString = new Array();
Xurl_filterString[0] = "Enable URL filtering";
Xurl_filterString[1] = "URL Address";
Xurl_filterString[2] = "Filtering Rules";
Xurl_filterString[3] = "Blacklist";
Xurl_filterString[4] = "Whitelist";
Xurl_filterString[5] = "Add";
Xurl_filterString[6] = "Default";
Xurl_filterString[7] = "Accept";
Xurl_filterString[8] = "Drop";
Xurl_filterString[9] = "mode";
Xurl_filterString[10] = "URL addr";
Xurl_filterString[11] = "Delete";

WebString.url_max_rule = "Rule number more than 32, can't continue to add!";
WebString.url_max_char = "URL must be less than 128 characters";
WebString.url_enter_addr = "Please enter the URL address";
WebString.url_filter_exist = "The filtering rules already exist!";

var Xip_qosString = new Array();
Xip_qosString[0] = "Enable Qos";
Xip_qosString[1] = "Source IP";
Xip_qosString[2] = "Destination IP";
Xip_qosString[3] = "DSCP original value";
Xip_qosString[4] = "802.1 p original value";
Xip_qosString[5] = "Protocol";
Xip_qosString[6] = "Source port";
Xip_qosString[7] = "Destination port";
Xip_qosString[8] = "Priority";
Xip_qosString[9] = "High";
Xip_qosString[10] = "Middle";
Xip_qosString[11] = "Normal ";
Xip_qosString[12] = "Low";
Xip_qosString[13] = "Delete";
Xip_qosString[14] = "Source Netmask";
Xip_qosString[15] = "Destination Netmask";
Xip_qosString[16] = "Source port end";
Xip_qosString[17] = "Destination port end";
Xip_qosString[18] = "Description";

WebString.qos_rule_max = "Rule number more than 16, can't continue to add!";
WebString.qos_port_range = "The port range is 0 to 65535";
WebString.qos_ip_error = "The IP address of the error";
WebString.qos_rule_exists = "This rule already exists";

var XportalString = new Array();
XportalString[0] = "Device Name";
XportalString[1] = "Serial Number";
XportalString[2] = "Hardware Version";
XportalString[3] = "Software Version";
XportalString[4] = "Device Running Time";
XportalString[5] = "Vendor";
XportalString[6] = "Transmitted Optical Power";
XportalString[7] = "Received Optical Power";
XportalString[8] = "Transceiver Temperature";
XportalString[9] = "Status:";
XportalString[10] = "Internet WAN";
XportalString[11] = "Gateway";
XportalString[12] = "DNS";
XportalString[13] = "WiFi Clients";
XportalString[14] = "DHCP Clients";
XportalString[15] = "Cable Failed &lt; Lỗi cáp quang >";
XportalString[16] = "No IP &lt; Không nhận IP >";
XportalString[17] = "DNS not Respond &lt; Không phân giải được tên miền>";
XportalString[18] = "Connected Internet &lt; Đã kết nối Internet >";
