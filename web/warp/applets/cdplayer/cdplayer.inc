<?php
require_once 'warp/warplib/applet.inc';
require_once 'zinc/system/cdplayer.inc';

class Warp_CDPlayer_Applet extends Warp_Applet
{
var $mixer;

    function Warp_CDPlayer_Applet($caption = "CD Player")
    {
        parent::__construct($caption);
    }

    function canRead($appMan)
    {
        return $appMan->getAuth()->isUserValidFor("cdplayer", "r");
    }

    function canWrite($appMan)
    {
        return $appMan->getAuth()->isUserValidFor("cdplayer", "w");
    }

    function init(&$appMan)
    {
	Warp_Applet::Init($appMan);

        if ($this->canWrite($appMan)) {
            $play = $appMan->queryParameter($this, "play");
            if ($play != "") {
                CDPlayer::playTrack($play);
            }
    
            if ($appMan->queryParameter($this, "stop") != "") {
                CDPlayer::stopTrack();
            } else if ($appMan->queryParameter($this, "resume") != "") {
                CDPlayer::resumeTrack();
            } else if ($appMan->queryParameter($this, "pause") != "") {
                CDPlayer::pauseTrack();
            } else if ($appMan->queryParameter($this, "eject") != "") {
                if (CDPlayer::isDrawerOpen()) {
                   CDPlayer::insertDisc();
                } else {
                   CDPlayer::ejectDisc();
                }
            }
        }

	return true;
    }

    function getID() { return "MIN003A"; }


    function getDirectory()
    {
        return "cdplayer";
    }

    function renderPanel(&$appMan)
    {
        if (!$this->canRead($appMan)) {
            return "";
        }

        $web  = "<div align=center>";
        if (CDPlayer::isDiscPresent()) {
           $trackNum = CDPlayer::getCurrentTrack();
           $trackMax = CDPlayer::getTrackCount();
 
           if ($trackNum == 0 && $trackMax == 0) {
              $web .= "No CD (DVD in drive?)";
           } else {
              $name = CDPlayer::getDiscName();
              if ($name != "") {
                 $web .= "$name<br/>";
              }
              $curr = CDPlayer::isPlaying();
              if ($curr == 0) {
                 $web .= "Not playing...";
              } else {
                 if (CDPlayer::isPaused()) {
                    $web .= "<i>(paused)</i>";
                 } else {
                    $name = CDPlayer::getTrackName(CDPlayer::getCurrentTrack());
                    if ($name != "") $web .= "<i>$name</i><br/>";
                    $web .= "$trackNum / $trackMax";
                 }
              }
           }
        } else {
           $web .= "No disc...";
        }
        $web.= "<br/>";
        $web .= $this->getControls($appMan);
        $web.= "</div>";
        return $web;
    }

    function img($fn) {
         return "<img src='".$this->getImageURL($fn)."' border=0/>";
    }

    function stdimg($fn) {
         return "<img src='".$this->getStdImageURL($fn)."' border=0/>";
    }

    function renderWindow(&$appMan)
    {
       if (!$this->canRead($appMan)) {
           return "";
       }

       $array = CDPlayer::getTrackListing();
       $current = CDPlayer::getCurrentTrack();
       $total = count($array);

       if (CDPlayer::isDiscPresent()) {
          $left = "<h3>".CDPlayer::getDiscName()."</h3>";
          $left.= "<table border=1>";
          for($track=1;$track<=count($array);++$track) {
             $row = split("\t", $array[$track - 1]);
             if ($track == $current) {
                $left .= "<tr><td>#$track : $row[0]</td><td>$row[1]</td></tr>";
             } else {
                $left .= "<tr><td>#$track : ".$appMan->getAppletLink($this, "play", $track, $row[0])."</td><td>$row[1]</td></tr>";
             }
       }
          $left .= "</table>";
       }

       // now 
       $disp = '<div align="right">'.$this->img("cdda_logo.gif").'</div>';
       $disp .= "<div align=center>";
       $disp .= $this->img("cdpl.png");
       $disp .= "<BR>";

       $str = substr("0".number_format($current, 0), -2);
       $num = strlen($str);
       for($i=0;$i<$num;$i++) {
          $ss = substr($str, $i, 1);
          if (is_numeric($ss)) {
              $disp .= $this->stdimg("numbers/${ss}.jpg").'&nbsp;';
          }
       }
       $disp .= "<BR>";
       $right .= $disp;


       $right .= $this->getControls($appMan);

       $right .= "</div>";
       return $appMan->webface->view->split2V($left, $right, "50%", "50%");
    }

    function renderConfig(&$appMan)
    {
    }

    function getControls(&$appMan) 
    {
       if (!$this->canWrite($appMan)) {
           return "";
       }

       $dir = $this->getPathStub();
       $current = CDPlayer::getCurrentTrack();
       $total = CDPlayer::getTrackCount();
       if ($current == 0) {
          $next = $prev = $current = 1;
       } else if ($current > 1) {
          $prev = $current - 1;
       } else {
          $prev = $current;
       }
       if ($current < $total) {
          $next = $current + 1;
       } else {
          $next = 1; 
       }

       $right .= $appMan->getAppletLink($this, "play", $prev, $this->img("prev.png"));
       $right .= $appMan->getAppletLink($this, "stop", "stop", $this->img("stop.png"));
       if (CDPlayer::isStopped()) {
          $right .= $appMan->getAppletLink($this, "play", $prev, $this->img("play.png"));
       } else if (CDPlayer::isPaused()) {
          $right .= $appMan->getAppletLink($this, "resume", "resume", $this->img("play.png"));
       } else {
          $right .= $appMan->getAppletLink($this, "pause", "pause", $this->img("pause.png"));
       }
       $right .= $appMan->getAppletLink($this, "play", $next, $this->img("next.png"));
       $right .= $appMan->getAppletLink($this, "eject", "eject", $this->img("eject.png"));
      return $right;
   }

    function renderInfo(&$app_man, $fast=false)
    {
        if ($fast) {
            return "-";
        }

        return <<<EOD
<p>A small applet to handle the internal CD drive of the computer.</p>
<p><i>Known Bugs: </i>Problems when a CD finishes playing.<br/></p>
<p><i>Dependencies: </i>cdcd</p>
<p><i>Future Plans: </i>CDDB re-integration. It used to work many years ago, before the take-over, and I never got around to getting cdcd to talk to the new databases.<br/></p>

EOD;
    }


}

?>

