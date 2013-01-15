<?
ini_set('include_path', './warp:'.ini_get('include_path'));
ini_set('include_path', './webface:'.ini_get('include_path'));

include 'warp/warplib/appletmanager.inc';
include 'warp/applets/main/main.inc';
include 'warp/applets/logger/logger.inc';

include_once 'system/system.inc';
include_once 'system/master_standard.conf';

$appman = getBrowser()->createManager();
$appman->init();

$appman->AddApplet(new Warp_Main_Applet());

$bsl="/var/log/minerva/bearskin/";
$s= Warp_Logger_Applet::$STYLE_DATESTAMP;

$appman->AddApplet(new Warp_Logger_Applet("Announce", "$bsl/announce", $s));
$appman->AddApplet(new Warp_Logger_Applet("Cosmic Control", "$bsl/cosmic", $s));
$appman->AddApplet(new Warp_Logger_Applet("Manifest", "$bsl/manifest", $s));
$appman->AddApplet(new Warp_Logger_Applet("Audio Mixer", "$bsl/mixer", $s));
$appman->AddApplet(new Warp_Logger_Applet("MP3 Player", "$bsl/mp3player", $s));
$appman->AddApplet(new Warp_Logger_Applet("Message Xmit", "$bsl/msgxmit", $s));
$appman->AddApplet(new Warp_Logger_Applet("Message Receive", "$bsl/msgrcv", $s));
$appman->AddApplet(new Warp_Logger_Applet("Say", "$bsl/say", $s));
$appman->AddApplet(new Warp_Logger_Applet("Twitter", "$bsl/tweet", $s));
$appman->AddApplet(new Warp_Logger_Applet("Wave player", "$bsl/wavplayer", $s));
$appman->AddApplet(new Warp_Logger_Applet("X10 Control", "$bsl/x10control", $s));

echo $appman->renderPage();
?>

