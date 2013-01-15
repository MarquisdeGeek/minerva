<?
include 'warp/warplib/appletmanager.inc';

include 'warp/applets/main/main.inc';
include 'warp/applets/timer/timer.inc';
include 'warp/applets/weather/weather.inc';
include 'warp/applets/todo/todo.inc';
include 'warp/applets/mp3play/mp3play.inc';
require_once 'warp/applets/cookery/cookery.inc';
require_once 'warp/applets/recipes/recipes.inc';

include_once 'system/system.inc';
include_once 'system/master_standard.conf';

$appman = getBrowser()->createManager();
$appman = new Warp_Applet_ManagerFullScreen();
$appman->init();

$appman->AddApplet(new Warp_Main_Applet());
$appman->AddApplet(new Warp_Recipes_Applet());
$appman->AddApplet(new Warp_Cookery_Info_Applet("Cooking Info"));
$appman->AddApplet(new Warp_Timer_Applet());
$appman->AddApplet(new Warp_TODO_Applet());
$appman->AddApplet(new Warp_Weather_Applet());
$appman->AddApplet(new Warp_MP3Play_Applet("MP3 Player", "media/mp3"));

echo $appman->renderPage();
?>

