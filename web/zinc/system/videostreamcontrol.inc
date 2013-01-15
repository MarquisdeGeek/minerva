<?php
require_once 'zinc/zinclib/device.inc';
require_once 'zinc/conf/videostreamcontrol.conf';

class VideoStreamControl extends ZincDevice
{
    // System handling

    public static function isPlaying()
    {
        return (int)self::cmd("isplaying");
    }

    public static function getCurrentURL()
    {
        return self::cmd("geturl");
    }

    public static function getCurrentName()
    {
        $url = self::cmd("geturl");
        $name = strrchr($url, "/");
        if ($name == FALSE) {
           return $url;
        }
        return substr($name, 1);
    }

    public static function getServerIP()
    {
        return self::cmd("getip");
    }

    public static function getServerPort()
    {
        return self::cmd("getport");
    }

    protected static function cmd($cmd) 
    {
        $cmd = VideoStreamDevice::$binary." ".VideoStreamDevice::$device." ".$cmd;
        return trim(parent::cmd($cmd));
    }

    // Control
    public static function playURL($url) 
    {
        return self::cmd("play $url");
    }
       
    public static function stop() 
    {
         self::cmd("stop");
    }
       
}

?>

