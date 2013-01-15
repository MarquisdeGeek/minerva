<?php
require_once 'zinc/zinclib/device.inc';
require_once 'zinc/conf/mixer.conf';

class Mixer extends ZincDevice
{
    public static $MASTER     = "master";
    public static $BASS       = "bass";
    public static $TREBLE     = "treble";
    public static $WAVE       = "wave";
    public static $PC_SPEAKER = "speaker";
    public static $LINE_IN    = "linein";
    public static $MICROPHONE = "microphone";
    public static $CDPLAYER   = "cd";
    public static $RECORDING  = "recording";

    public static function getVolume($channel)
    {
        $cmd = MixerDevice::$binary." default get $channel";
        return (int)parent::cmd($cmd);
    }

    public static function setVolume($channel, $volume)
    {
        $cmd = MixerDevice::$binary." default set $channel ".(int)$volume;
        $cmd.="&";
        return (int)parent::cmd($cmd);
    }

    public static function incVolume($channel, $by) 
    {
        return self::setVolume($channel, self::getVolume($channel) + $by);
    }

    public static function decVolume($channel, $by) 
    {
        return self::setVolume($channel, self::getVolume($channel) - $by);
    }

    public static function getChannelID($idx) 
    {
        switch($idx) {
           case 0:   return self::$MASTER;
           case 1:   return self::$BASS;
           case 2:   return self::$TREBLE;
           case 3:   return self::$WAVE;
           case 4:   return self::$PC_SPEAKER;
           case 5:   return self::$LINE_IN;
           case 6:   return self::$MICROPHONE;
           case 7:   return self::$CDPLAYER;
           case 8:   return self::$RECORDING;
        }
    }

    public static function getName($id) 
    {
        return ucwords($id);
    }

    public static function getNumChannels() {
       return 9;
    }
          
}

