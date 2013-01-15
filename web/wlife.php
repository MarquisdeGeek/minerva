<?
ini_set('include_path', './warp:'.ini_get('include_path'));
ini_set('include_path', './webface:'.ini_get('include_path'));

include 'warp/warplib/appletmanager.inc';

include 'warp/applets/main/main.inc';
include 'warp/applets/calendar/calendar.inc';
include 'warp/applets/x10/x10control.inc';
include 'warp/applets/weather/weather.inc';
include 'warp/applets/contacts/contacts.inc';
include 'warp/applets/moonbeam/moonbeam_minerva.inc';
include 'warp/applets/currency/currency.inc';
include 'warp/applets/todo/todo.inc';
include 'warp/applets/simulate/manifest/manifest.inc';

include_once 'system/system.inc';
include_once 'system/master_standard.conf';

$appman = getBrowser()->createManager();
$appman->init();

$appman->AddApplet(new Warp_Main_Applet());
$appman->AddApplet(new Warp_Calendar_Applet());
$appman->AddApplet(new Warp_X10_Applet());
$appman->AddApplet(new Warp_Weather_Applet());
$appman->AddApplet(new Warp_Contacts_Applet());
$appman->AddApplet(new Warp_Moonbeam_Applet());
$appman->AddApplet(new Warp_Currency_Applet());
$appman->AddApplet(new Warp_TODO_Applet());
$appman->AddApplet(new Warp_Manifest_Applet());
/*
*/
echo $appman->renderPage();

?>

