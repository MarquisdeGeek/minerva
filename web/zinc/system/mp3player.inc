<?php
require_once 'zinc/zinclib/device.inc';
require_once 'zinc/conf/mp3player.conf';

class MP3Player extends ZincDevice
{
    // System handling

    public static function isPlaying()
    {
        return (int)self::cmd("isplaying");
    }

    public static function getCurrentTrack()
    {
        return self::cmd("current");
    }

    public static function getArtist()
    {
        return self::cmd("artist");
    }

    public static function getAlbum()
    {
        return self::cmd("album");
    }

    public static function getDuration()
    {
        return self::cmd("length");
    }

    public static function isPaused()
    {
        return (int)self::cmd("ispaused");
    }

    protected static function cmd($cmd) 
    {
        $cmd = MP3PlayerDevice::$binary." ".MP3PlayerDevice::$device." ".$cmd;
        return parent::cmd($cmd);
    }

    // Control
    public static function playTrack($track) 
    {
        $track = str_replace("`", "\`", $track);

        $cmd = MP3PlayerDevice::$binary." ".MP3PlayerDevice::$device;
        $cmd.= " play $track";

        # This convoluted method is necessary to start the command in the bg
        # without PHP waiting for _something_ to complete.
        $out = system("($cmd 2>&1 >/dev/null) >/dev/null 2>&1  &");

        # And this is a hack to give the mp3 player a chance to start, so
        # that when the rest of the system queries the artist/album/track
        # data, it actually exists.
        sleep(1);
        return $out;
    }
       
    public static function stopTrack() 
    {
         self::cmd("stop");
    }
       
    public static function nextTrack() 
    {
         self::cmd("next");
    }

    public static function previousTrack() 
    {
         self::cmd("previous");
    }

    public static function pauseTrack() 
    {
         self::cmd("pause");
    }
       
    public static function resumeTrack() 
    {
        self::cmd("resume");
    }
       
}

?>

