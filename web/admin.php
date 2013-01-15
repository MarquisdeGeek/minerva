<?
include 'warp/warplib/appletmanager.inc';

include 'warp/applets/admin/admin.inc';

include_once 'system/system.inc';
include_once 'system/master_standard.conf';

$appman = getBrowser()->createManager();
$appman->init();

$appman->AddApplet(new Warp_MainAdmin_Applet());

echo $appman->renderPage();
?>

