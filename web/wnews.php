<?
require_once 'warp/warplib/appletmanager.inc';
require_once 'warp/applets/main/main.inc';
require_once 'warp/applets/weather/weather.inc';
require_once 'warp/applets/tvguide/tvguide.inc';
require_once 'warp/applets/photoframe/photoframe.inc';
require_once 'warp/applets/cookery/cookery.inc';
require_once 'warp/applets/ldb/ldb.inc';
require_once 'warp/applets/photobrowser/photobrowser.inc';

include_once 'system/system.inc';
include_once 'system/master_standard.conf';

$appman = getBrowser()->createManager();
$appman->init();

$appman->AddApplet(new Warp_Main_Applet());
$appman->AddApplet(new Warp_TVGuide_Applet());
$appman->AddApplet(new Warp_PhotoFrame_Applet());
$appman->AddApplet(new Warp_Weather_Applet());
$appman->AddApplet(new Warp_Cookery_Info_Applet("Cooking Info"));
$appman->AddApplet(new Warp_PhotoBrowser_Applet("MyMedia Photos", "minerva/media/Photos"));
$appman->AddApplet(new Warp_LiveDepartureBoards_Applet());

echo $appman->renderPage();
?>

