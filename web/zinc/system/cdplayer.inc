<?php
require_once 'zinc/zinclib/device.inc';
require_once 'zinc/conf/cdplayer.conf';

class CDPlayer extends ZincDevice
{
    // System handling

    public static function isPlayerPresent()
    {
        return true;
    }

    public static function isDiscPresent()
    {
        if (self::isDrawerOpen()) {
           return false;
        }
        return true;
    }

    // Querying the state

    public static function getTrackCount()
    {
        if (!self::isDiscPresent()) {
           return 0;
        }
        return (int)self::cmd("count");
    }

    public static function getCurrentTrack()
    {
        if (!self::isDiscPresent()) {
           return 0;
        }
        return (int)self::cmd("current");
    }

    public static function getDiscName() 
    {
        if (!self::isDiscPresent()) {
           return "";
        }
        return self::cmd("discname");
    }

    public static function getTrackName($track) 
    {
        if (!self::isDiscPresent()) {
           return "";
        }
        return self::cmd("trackname $track");
    }

    public static function getTrackTick($track)
    {
        return NULL;
    }

    public static function isPlaying()
    {
        if (!self::isDiscPresent()) {
           return 0;
        }
        return (int)self::cmd("isplaying");
    }

    public static function isStopped()
    {
        if (!self::isDiscPresent()) {
           return 0;
        }
        return (int)self::cmd("isstopped");
    }

    public static function isPaused()
    {
        if (!self::isDiscPresent()) {
           return 0;
        }
        return (int)self::cmd("ispaused");
    }

    public static function isDrawerOpen()
    {
        return (int)self::cmd("isdraweropen");
    }

    public static function getTrackListing()
    {
        if (!self::isDiscPresent()) {
           return 0;
        }

        $count = self::getTrackCount();
        for($i=1;$i<=$count;++$i) {
           $name = self::getTrackName($i);
           if ($name == "") {
              $tracks .= "Track $i\n";
          } else {
              $tracks .= "$name";
          }
        }
        $array = split("\n", $tracks);
        if ($array[count($array)-1] == "") {
           unset($array[count($array)-1]);
        }
        return $array;
    }
 
    protected static function cmd($cmd) 
    {
        $cmd = CDPlayerDevice::$binary." ".CDPlayerDevice::$device." ".$cmd;
        return parent::cmd($cmd);
    }

    // CD Control
    public static function playTrack($track) 
    {
         self::cmd("play $track");
    }
       
    public static function stopTrack() 
    {
         self::cmd("stop");
    }
       
    public static function pauseTrack() 
    {
         self::cmd("pause");
    }
       
    public static function resumeTrack() 
    {
        if (self::cmd("isstopped")==1) {
            return self::playTrack(self::getCurrentTrack());
        }
        self::cmd("resume");
    }
       
    public static function ejectDisc() 
    {
         self::cmd("eject");
    }
       
    public static function insertDisc() 
    {
         self::cmd("close");
    }
       
       
}

?>

