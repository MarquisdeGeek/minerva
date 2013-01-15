<?php

require_once 'warp/warplib/applet.inc';
require_once 'zinc/system/fmradio.inc';

class Warp_FMRadio_Applet extends Warp_Applet
{
    function Warp_FMRadio_Applet($caption = "FM Radio")
    {
        parent::__construct($caption);
    }

    function getDirectory()
    {
        return "fmradio";
    }

    function init(&$appMan)
    {
        $cmd = $appMan->queryParameter($this, "cmd");
        if ($cmd == "prevstation") {
            RadioDevice::setFrequency(RadioDevice::getPreviousStationsFrequency());
        } else if ($cmd == "nextstation") {
            RadioDevice::setFrequency(RadioDevice::getNextStationsFrequency());

        } else if ($cmd == "on") {
            RadioDevice::deviceOn();

        } else if ($cmd == "off") {
            RadioDevice::deviceOff();
        }

        $cmd = $appMan->queryParameter($this, "set");
        if ($cmd != "") {
            RadioDevice::setFrequency($cmd);
        }
    }

    function img($fn) {
         return "<img src='".$this->getImageURL($fn)."' border=0/>";
    }

    function renderPanel(&$appMan)
    {
        if (RadioDevice::isPlaying()) {
           $panel = RadioDevice::getFrequency()."<br/><i>".RadioDevice::getStationName()."</i>";
           $panel .= $appMan->getAppletLink($this, "cmd", "prevstation", $appMan->getStyleText("prev"));
           $panel .= $appMan->getAppletLink($this, "cmd", "nextstation", $appMan->getStyleText("next"));
           $panel.= "<br/>";
           $panel.= $appMan->getAppletLink($this, "cmd","off", "Turn off");
        } else {
           $panel.= $appMan->getAppletLink($this, "cmd","on", "Turn on");
        }

        return $panel;
    }

    function renderWindow(&$appMan)
    {
       $radio_freq = RadioDevice::getFrequency();

       $table = '<TABLE CELLSPACING="1" CELLPADDING="0">';
       $table.= '<TR><TD><b>Station Name</b></TD><TD><b>Frequency</b></TD></TR>';

       $table .= "<TR><TD>";
       $table .= "</TD><TD>&nbsp;</TD></TR>";

       $current = RadioDevice::getStationName();
       $stations = RadioDevice::getStationList();
       $table .= $results;
       foreach ($stations as $freq => $name) {
          if ($freq == $radio_freq) {
              $txt = "<b>$name</b>";
          } else {
               if ($appMan->getAuth()->IsUserValidFor("radio", "w")) {
                  $txt = $appMan->getAppletLink($this, "set", "$freq", $name);
               } else {
                  $txt = $name;
               }
          }
          $table .= "<TR><TD>$txt</TD><TD>${freq}</TD></TR>\n";
       }

       $table.="</TABLE>";

       if (RadioDevice::isPlaying()) {
          $disp='<DIV ALIGN="CENTER">';
          $disp .= '<b><h1>'.RadioDevice::getStationName($radio_freq).'</h1></b>';

          $str = substr("0".number_format($radio_freq, 2), -6);
          $num = strlen($str);
          for($i=0;$i<$num;$i++) {
             $ss = substr($str, $i, 1);
             if ($ss == '.') {
                $disp .= $this->img("colon.png")."&nbsp;";
             } else if (is_numeric($ss)) {
                $disp .= $this->img("$ss.png")."&nbsp;";
             }
          }
   
          $disp .= '<BR>';
   
          $anchor = $appMan->getStyleText("prev");
          $disp.= $appMan->getAppletLink($this, "cmd", "prevstation", $anchor);
          $disp .= "&nbsp;Station Scan&nbsp;";
   
          $anchor = $appMan->getStyleText("next");
          $disp.= $appMan->getAppletLink($this, "cmd", "nextstation", $anchor);
   
          $disp .= '<BR><br/>';
   
          $anchor = $appMan->getStyleText("prev");
          $disp.= $appMan->getAppletLink($this, "set",($radio_freq-0.05), $anchor);
          $disp .= "&nbsp;Manual Tune&nbsp;";
          $anchor = $appMan->getStyleText("next");
          $disp.= $appMan->getAppletLink($this, "set",($radio_freq+0.05), $anchor);
          $disp .= '<BR>';
   
          if (!$appMan->getAuth()->isUserValidFor("radio")) {
             $disp.= "<BR><BR><I>You are not authorised to re-tune the radio!</I>";
          }
   
          $disp .= '</DIV>';
          $disp.= "<br><br>";
          $disp.= $status_text; 
       } else { // radio is off
          $disp = $appMan->getAppletLink($this, "cmd","on", "Turn on");
       }
       
       return $appMan->getView()->split2V($table, $disp);
    }

}


?>

