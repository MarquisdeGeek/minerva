<?
include 'warp/warplib/appletmanager.inc';

include 'warp/applets/main/main.inc';
include 'warp/applets/tvguide/tvguide.inc';
include 'warp/applets/cdplayer/cdplayer.inc';
include 'warp/applets/mp3play/mp3play.inc';
include 'warp/applets/mixer/mixer.inc';
include 'warp/applets/vlc/vlc.inc';
include 'warp/applets/videostreamcontrol/vsc.inc';
include 'warp/applets/mediainfo/whatsplaying.inc';
include 'warp/applets/fmradio/fmradio.inc';

include_once 'system/system.inc';
include_once 'system/master_standard.conf';

$appman = getBrowser()->createManager();
$appman->init();

$appman->AddApplet(new Warp_Main_Applet());
$appman->AddApplet(new Warp_TVGuide_Applet());
$appman->AddApplet(new Warp_CDPlayer_Applet());
$appman->AddApplet(new Warp_MP3Play_Applet("MP3 Player", "media/mp3"));
$appman->AddApplet(new Warp_FMRadio_Applet());
$appman->AddApplet(new Warp_Mixer_Applet());
$appman->AddApplet(new Warp_VLC_Applet());
$appman->AddApplet(new Warp_MediaPlaying_Applet("MP3 Playlist", "/var/log/minerva/bearskin/mp3player"));
$appman->AddApplet(new Warp_VideoStreamControl_Applet("Video Control", "/net/media/videos"));
$appman->AddApplet(new Warp_MP3Play_Applet("Magic", "/net/media/Magic/audio"));

echo $appman->renderPage();
?>

