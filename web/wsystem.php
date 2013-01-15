<?
include 'warp/warplib/appletmanager.inc';

include 'warp/applets/main/main.inc';
include 'warp/applets/top/top.inc';
include 'warp/applets/useragent/ua.inc';
include 'warp/applets/diskfree/df.inc';
include 'warp/applets/smbstatus/smbstatus.inc';
include 'warp/applets/static/static.inc';
include 'warp/applets/ups/ups.inc';
include 'warp/applets/bluetooth/bluetooth.inc';

include_once 'system/system.inc';
include_once 'system/master_standard.conf';

$appman = getBrowser()->createManager();
$appman->init();

$appman->AddApplet(new Warp_Main_Applet());
$appman->AddApplet(new Warp_Static_Text_Applet());
$appman->AddApplet(new Warp_Disk_Space_Applet());
$appman->AddApplet(new Warp_UA_Applet());
$appman->AddApplet(new Warp_Top_Applet());
$appman->AddApplet(new Warp_SambaStatus_Applet());
$appman->AddApplet(new Warp_UPSMonitor_Applet());
$appman->AddApplet(new Warp_Bluetooth_Applet());

echo $appman->renderPage();
?>

