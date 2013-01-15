<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';
require_once 'warp/applets/logger/logger.inc';

class Warp_MediaPlaying_Applet extends Warp_Logger_Applet 
{
    function Warp_MediaPlaying_Applet($description, $logFile)
    {
        parent::__construct($description, $logFile, self::$STYLE_DATESTAMP);
    }
 
    function getDirectory()
    {
        return "mediainfo";
    }

    function processLogFileLine($idx, $line, $regexOptions)
    {
       // Remove odd characters, as requested
       if ($regexOptions != "/[]/") {
          $line = preg_replace($regexOptions, " ", $line);
       }

       if (preg_match("/(.*?)CMD.*?mp3player\s+\w+?\s+play\s+(.*)\/(.*?)$/", $line, $matches)) {
          $line = $matches[3]."  (".$matches[2].")";
          $wlal = new WLAL_Datestamp("");
          $wlal->dateText = $matches[1];
          $wlal->cmdText = $idx;
          $wlal->text = $line;
          return $wlal;
       }
       return null;
    }
   
}

?>
