<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';
require_once 'warp/applets/browser/browser.inc';
require_once 'warp/conf/mp3player/mp3player.conf';
require_once 'zinc/system/mp3player.inc';

class Warp_MP3Play_Applet extends Warp_Browser_Applet
{
var $_filename;

    function Warp_MP3Play_Applet($caption = "MP3 Player", $rootpath = NULL)
    {
        parent::__construct($caption, $rootpath);
    }

    function getDirectory()
    {
        return "mp3play";
    }

    function init(&$appMan)
    {
        Warp_Browser_Applet::init($appMan);

        $do = $appMan->queryParameter($this, "do");
        $play = $appMan->queryParameter($this, "play");
        $playdir = $appMan->queryParameter($this, "playdir");

        if ($do != "" || $play != "" || $playdir != "") {
 
           switch($do) {
              case "next":
                 MP3Player::nextTrack(); 
                 break;
              case "prev":
                 MP3Player::previousTrack(); 
                 break;
              case "pause":
                 MP3Player::pauseTrack(); 
                 break;
              case "resume":
                 MP3Player::resumeTrack(); 
                 break;
              case "stop":
                 MP3Player::stopTrack(); 
                 break;
           }

           if ($play != "") {
              $this->_filename = $this->_filelist[$play]->_url;
              for($i=$play;$i<count($this->_filelist);++$i) {
                 if (true) {
                   $arg .= " ";
                   $arg .= '"'.$this->_rootpath."/".$this->_dir."/".$this->_filelist[$i]->_url.'"';
                 }
              }
              MP3Player::stopTrack();
              MP3Player::playTrack($arg);
           }

           if ($playdir != "") {
              MP3Player::stopTrack();
              MP3Player::playTrack($this->_rootpath."/".$playdir."/*.*");
           }

        }
        return true;
    }

    function cleanseName($track)
    {
       $track = ereg_replace("\.(ogg|mp3|mpeg|mp2)", "", $track);
       $track = ereg_replace("[_\.\-]", " ", $track);
       return $track;
    }

    function renderPanel(&$appMan)
    {
       if (MP3Player::isPlaying()) {
          $track = MP3Player::getCurrentTrack();
          $track = $this->cleanseName($track);
          $artist = MP3Player::getArtist();
          if ($artist != "") {
             $track.="&nbsp;-&nbsp;$artist";
          } 
          $track .= "<br/><div align=center>".$this->getControls($appMan)."</div>";
       }
       return $track;
    }

    function renderWindow(&$appMan)
    {
        $top = $this->getDirectoryBrowser($appMan, $this->_dir);
        $filepane = $this->getFilePane($appMan, 0);
        $dirpane = $this->getDirectoryPane($appMan, 0);
        $panel = $this->getPanel($appMan);
        $rhs = $appMan->getView()->split2H($filepane, $panel);
        return $top . $appMan->getView()->split2V($dirpane, $rhs);
    }

    function renderInfo(&$appMan)
    {
       return <<<EOS
<p>A simple applet that lets you explore a designated directory structure
and play the MP3 files within using the Bearskin <tt>mp3player</tt> wrapper.
<p>The left hand side displays a list of the current directories, with a
play button to play all files in that directory. The right hand side shows
the files in the current directory, and by clicking on them will play that
specific file, and (if left) all those after it. You can forward through this
list, but not backwards. If an MP3 is playing, the track information (album
and artist, if given in the MP3) will appear below, in addition to the control
gadgets.
<p>Only files beneath the specified root directory will be visible. This can
be set by constructing the applet appropriately. i.e.
<pre>
\$applet = new Warp_MP3Play_Applet("MP3Player", "/media/mp3/");
</pre>
in the originating PHP file.

EOS;
    }

    function img($fn) {
         return "<img src='".$this->getImageURL($fn)."' border=0/>";
    }

    function getPanel(&$appMan)
    {
        if (MP3Player::isPlaying()) {
           $info .= "<hr/>";

           $track = MP3Player::getCurrentTrack();
           $track = $this->cleanseName($track);
           $info .= "<h3>$track</h3>";

           $info .= "<table border=0><tr><td valign=top>";
           $info .= "<i>Artist:</i> ".MP3Player::getArtist()."<br/>";
           $info .= "<i>Album:</i> ".MP3Player::getAlbum()."<br/>";
           $info .= "<i>Duration:</i> ".MP3Player::getDuration()."<br/>";
           $info .= "</td><td valign=bottom>";

           $info .= "<div align=right>";
           $info .= $this->getControls($appMan);
           $info .= "</div>";

           $info .= "</td></tr></table>";
        }

        return $info;
    }

    function getControls(&$appMan) 
    {
       $right .= $appMan->getAppletLink($this, "do", "prev", $this->img("prev.png"));
       $right .= $appMan->getAppletLink($this, "do", "stop", $this->img("stop.png"));
       if (MP3Player::isPaused()) {
          $right .= $appMan->getAppletLink($this, "do", "resume", $this->img("play.png"));
       } else {
          $right .= $appMan->getAppletLink($this, "do", "pause", $this->img("pause.png"));
       }
       $right .= $appMan->getAppletLink($this, "do", "next", $this->img("next.png"));
      return $right;
   }

   function renderFileLine($appMan, $f)
   {
      $full = $this->_rootpath."/".$this->_dir."/".$f->_url;
      $title = $this->cleanseName($f->_title);
      return $this->img("file.png")."&nbsp;".$appMan->getAppletLink($this, "play", $f->_idx, $title)."<br/>";
   }

   function renderDirectoryLine($appMan, $d)
   {
      $full = $d->_dir;
      $link = $this->img("folder.gif")."&nbsp;".$d->getRenderLine($appMan, $this);
      return $link."&nbsp;".$appMan->getAppletLink($this, "playdir", $full, $this->img("play.png"))."<br/>";
   }


   function getRefreshTime($appMan)
   {
      if (!Warp_MP3Player_Config::refreshPage()) {
         return 0;
      }
  
      return Warp_MP3Player_Config::getRefreshTime();
   }

}

?>


