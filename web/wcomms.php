<?
ini_set('include_path', './warp:'.ini_get('include_path'));
ini_set('include_path', './webface:'.ini_get('include_path'));

include 'warp/warplib/appletmanager.inc';

include 'warp/applets/main/main.inc';
include 'warp/applets/fellowship/fellowship.inc';

include_once 'system/system.inc';
include_once 'system/master_standard.conf';

$appman = getBrowser()->createManager();
$appman->init();

$appman->AddApplet(new Warp_Main_Applet());
$appman->AddApplet(new Warp_Fellowship_Applet());

echo $appman->renderPage();

?>

