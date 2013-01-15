<?
include_once 'warp/warplib/appletmanager.inc';
include_once 'warp/applets/main/main.inc';
include_once 'warp/applets/logger/logger.inc';

include_once 'system/system.inc';
include_once 'system/master_standard.conf';

$appman = getBrowser()->createManager();
$appman->init();

$appman->AddApplet(new Warp_Main_Applet());
$appman->AddApplet(new Warp_Logger_Applet("Downloads", "/var/www/minerva/logs/log1"));
$appman->AddApplet(new Warp_Logger_Applet("Second Test", "/var/www/minerva/logs/log2"));
$appman->AddApplet(new Warp_Logger_Applet("X10 Heyu", "/var/log/heyu/heyu.log.ttyS0", Warp_Logger_Applet::$STYLE_LOGGER));
$appman->AddApplet(new Warp_Logger_Applet("X10 Minerva", "/var/log/minerva/x10control.err"));
$appman->AddApplet(new Warp_Logger_Applet("Message System", "/var/log/minerva/bearskin/msgxmit", Warp_Logger_Applet::$STYLE_DATESTAMP));

echo $appman->renderPage();
?>

