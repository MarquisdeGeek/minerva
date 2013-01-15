<?
include 'warp/warplib/appletmanager.inc';

include 'warp/applets/main/main.inc';
include 'warp/applets/static/static.inc';

$appman = new Warp_Applet_Manager;
$appman->init();

$appman->AddApplet(new Warp_Main_Applet());
$appman->AddApplet(new Warp_Static_Text_Applet());

echo $appman->renderPage();
?>

