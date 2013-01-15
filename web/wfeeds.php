<?
require_once 'warp/warplib/appletmanager.inc';
require_once 'warp/applets/main/main.inc';
require_once 'warp/applets/rssreader/rssreader.inc';

include_once 'system/system.inc';
include_once 'system/master_standard.conf';

$appman = getBrowser()->createManager();
$appman->init();

$appman->AddApplet(new Warp_Main_Applet());

exec("/usr/local/minerva/bin/news-get public", $publicFeedList);

foreach($publicFeedList AS $name) {
   $appman->AddApplet(new Warp_RSS_Applet("public", $name));
}

echo $appman->renderPage();
?>

