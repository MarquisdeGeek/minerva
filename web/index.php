<?
include 'warp/warplib/appletmanager.inc';

include 'warp/applets/main/index.inc';

include_once 'system/system.inc';
include_once 'system/master_standard.conf';

$appman = getBrowser()->createManager();
$appman->init();

$appman->AddApplet(new Warp_Main_Applet());

echo $appman->renderPage();
?>

