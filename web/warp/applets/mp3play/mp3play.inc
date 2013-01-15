<?php
require_once 'system/setup.conf';
require_once 'system/utils.inc';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';
require_once 'warp/applets/browser/browser.inc';
require_once 'warp/conf/mp3player/mp3player.conf';
require_once 'zinc/system/mp3player.inc';

class Warp_MP3Play_Applet extends Warp_Browser_Applet
{
var $_filename;
var $_relativeRoot;

    function Warp_MP3Play_Applet($caption = "MP3 Player", $rootpath = NULL)
    {
        parent::__construct($caption, $rootpath);
        $this->_relativeRoot = $rootpath;
    }

    function getDirectory()
    {
        return "mp3play";
    }

    function init(&$appMan)
    {
        Warp_Browser_Applet::init($appMan);

        if ($appMan->getAuth()->isUserValidFor("mp3jukebox", "w")) {
            $do = $appMan->queryParameter($this, "do");
            $play = $appMan->queryParameter($this, "play");
            $playdir = $appMan->queryParameter($this, "playdir");
        }

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

           if ($playdir != "") {
					$playdir = appletUtils::validatePath($playdir);

					$dirlist = array();
					$filelist = array();

					$this->getDirectoryList($appMan, $playdir, $dirlist, $filelist);
					$arg = $this->getPlayList($playdir, $filelist, 0);

          		MP3Player::stopTrack();
           		MP3Player::playTrack($arg);
           } 
				if ($play != "") {
					// When asked to play the track, we also queue the rest of the
					// album up. (It's easier to stop the music, than it is to
					// continually restart it.)
					$arg = $this->getPlayList($this->_dir, $this->_filelist, $play);
            # print "PLAY : $arg"; 
              MP3Player::stopTrack();
              MP3Player::playTrack($arg);
           }


        }
        return true;
    }

	function getPlayList($playdir, &$filelist, $firstIndex) {
		$arg = "";
		for($i=$firstIndex;$i<count($filelist);++$i) {
			$arg .= " ";
			$arg .= '"'.$this->_rootpath."/$playdir/".$filelist[$i]->_url.'"';
		}
		return $arg;
	}

    function renderPanel(&$appMan)
    {
        if ($appMan->getAuth()->isUserValidFor("mp3jukebox", "r")) {
            if (MP3Player::isPlaying()) {
              $pathStub = $this->getPathStub()."/current.php?curr=short";
$track = <<< EOS
              <div id='mp3panel'></div>
              <script language="Javascript">
                 function poll() {
                    xmlhttpPost("$pathStub", "", "mp3panel");
                    setTimeout("poll()", 30000);
                 }
                 poll();
               </script>
EOS;
               $track .= "<br/><div align=center>".$this->getControls($appMan)."</div>";
            }
       }
       return $track;
    }

    function renderWindow(&$appMan)
    {
        if ($appMan->getAuth()->isUserValidFor("mp3jukebox", "r")) {
            $top = $this->getDirectoryBrowser($appMan, $this->_dir);
            $filepane = $this->getFilePane($appMan, 0);
            $dirpane = $this->getDirectoryPane($appMan, 0);
            $panel = $this->getPanel($appMan);
            $rhs = $appMan->getView()->split2H($filepane, $panel);
            $html = $top . $appMan->getView()->split2V($dirpane, $rhs);
        }
        return $html;
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
<p><i>Dependencies: </i>mplayer or mpg123, mp3info</p>
<p><i>Future Plans: </i>Upgrade its functionality with playlists to make it a
fully fledged MP3 jukebox.<br/></p>
<p><i>Known bugs: </i>Directory names will not work correctly if they
are invalid in an HTTP GET request. You can not move to the 'next' MP3
specified when the Linux user requesting it differs from who initiated it.</p>

EOS;
    }

    function img($fn) {
         return "<img src='".$this->getImageURL($fn)."' border=0/>";
    }

    function getPanel(&$appMan)
    {
        $pathStub = $this->getPathStub()."/current.php?curr=full";
        $info .= <<< EOS
           <div id='mp3panel_full'></div>
           <script language="Javascript">
           function pollPanel() {
              xmlhttpPost("$pathStub", "", "mp3panel_full");
              setTimeout("pollPanel()", 20000);
           }
           setTimeout("pollPanel()", 2000);
           </script>
EOS;
        $info .= "<div align=right>";
        $info .= $this->getControls($appMan);
        $info .= "</div>";

        return $info;
    }

    function getControls(&$appMan) 
    {
       if (!$appMan->getAuth()->isUserValidFor("mp3jukebox", "w")) {
           return ""; 
       }

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
      $html = $this->img("file.png")."&nbsp;";
      if ($appMan->getAuth()->isUserValidFor("mp3jukebox", "w")) {
         $html .= $appMan->getAppletLink($this, "play", $f->_idx, $f->_title);
      } else {
         $html .= $f->_title; 
      }

      $stream = "http://".getServerName();
      $stream.= "/".$this->_relativeRoot.$this->_dir."/".$f->_url;
      $html .= " (<a href='$stream?stream'>stream</a>)";
      $html .= "<br/>";
      return $html;
   }

   function renderDirectoryLine($appMan, $d)
   {
      $full = $d->_dir;
      $link = $this->img("folder.gif")."&nbsp;".$d->getRenderLine($appMan, $this);
      return $link."&nbsp;".$appMan->getAppletLink($this, "playdir", $full, $this->img("play.png"))."<br/>";
   }


   function etRefreshTime($appMan)
   {
      if (!Warp_MP3Player_Config::refreshPage()) {
         return 0;
      }
  
      return Warp_MP3Player_Config::getRefreshTime();
   }

}

?>


