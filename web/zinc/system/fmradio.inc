<?php
require_once 'zinc/zinclib/device.inc';
require_once 'zinc/conf/fmradio.conf';

class RadioDevice extends ZincDevice
{
    protected static function cmd($cmd) 
    {
        $cmd = RadioTuner::$binary." ".RadioTuner::$device." ".$cmd;
        return parent::cmd($cmd);
    }

    // Querying the state

    public static function getStationName()
    {
        $currentFreq = (float)self::getFrequency();
        $stationList = RadioDevice::getStationList();

        $result = $stationList['$currentFreq'];
        foreach ($stationList as $freq => $name) {
           if ($currentFreq == (float)$freq) {
              return $name;
           }
        }

        return "Unknown Station";
    }

    public static function deviceOn()
    {
        return (int)(self::cmd("on"));
    }

    public static function deviceOff()
    {
        return (int)(self::cmd("off"));
    }

    public static function isPlaying()
    {
        return (int)(self::cmd("isplaying"));
    }

    public static function getFrequency()
    { 
        return trim(self::cmd("get"));
    }

    public static function getStationList()
    {
      $results = array();
      $stationlist = self::cmd("getstationlist");
		$output = split("\n", $stationlist);
      foreach($output as $station) {
         preg_match("/^(.*?)\s+(.*?)\s(.*)$/", $station, $stationInfo);
         $results[$stationInfo[2]] = $stationInfo[3];
      }
      return $results;
    }

    public static function getNextStationsFrequency()
    { 
        $currentFreq = (float)(self::getFrequency());
        $stationList = RadioDevice::getStationList();
        $lowestDiff = 1000;
        $result = $currentFreq; 

        foreach ($stationList as $freq => $name) {
            $diff = (float)($freq) - $currentFreq;
            if ($diff > 0 && $diff < $lowestDiff) {
                $result = $freq;
                $lowestDiff = $diff;
            }
        }
        return $result;
    }

    public static function getPreviousStationsFrequency()
    {
        $currentFreq = self::getFrequency();
        $stationList = RadioDevice::getStationList();
        $lowestDiff = -1000;
        $result = $currentFreq;

        foreach ($stationList as $freq => $name) {
            $diff = (float)($freq) - $currentFreq;
            if ($diff < 0 && $diff > $lowestDiff) {
                $result = $freq;
                $lowestDiff = $diff;
            }
        }

        return $result;
    }


    // Control
    public static function setFrequency($freq) 
    {
         return self::cmd("set \"$freq\"");
    }
       
}

?>

