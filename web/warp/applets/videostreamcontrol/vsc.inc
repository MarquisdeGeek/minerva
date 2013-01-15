<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';
require_once 'warp/applets/browser/browser.inc';
require_once 'zinc/system/videostreamcontrol.inc';

class Warp_VideoStreamControl_Applet extends Warp_Browser_Applet
{
var $_filename;

    function Warp_VideoStreamControl_Applet($caption = "Video Stream Control", $rootpath = NULL)
    {
        parent::__construct($caption, $rootpath);
    }

    function getDirectory()
    {
        return "videostreamcontrol";
    }

    function init(&$appMan)
    {
        Warp_Browser_Applet::init($appMan);

        if ($appMan->getAuth()->isUserValidFor("videostream", "w")) {
            $do = $appMan->queryParameter($this, "do");
            $play = $appMan->queryParameter($this, "play");
        }

        if ($do != "" || $play != "") {
 
           switch($do) {
              case "stop":
                 VideoStreamControl::stop(); 
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

              VideoStreamControl::stop();
              VideoStreamControl::playURL($arg);
           }
        }
        return true;
    }

    function renderPanel(&$appMan)
    {
       if ($appMan->getAuth()->isUserValidFor("videostream", "r")) {
          $ply = "Unavailable...";
       } else if (VideoStreamControl::isPlaying()) {
          $ply = "Playing ".VideoStreamControl::getCurrentName();
       } else {
          $ply = "The server is not running...";
       }
       return $ply;
    }

    function renderWindow(&$appMan)
    {
        if ($appMan->getAuth()->isUserValidFor("videostream", "r")) {
           $top = $this->getDirectoryBrowser($appMan, $this->_dir);
           $filepane = $this->getFilePane($appMan, 0);
           $dirpane = $this->getDirectoryPane($appMan, 0);
           $panel = $this->getControls($appMan);
           $rhs = $appMan->getView()->split2H($filepane, $panel);
           $html = $top . $appMan->getView()->split2V($dirpane, $rhs);
        }
        return $html;
    }

    function renderInfo(&$appMan)
    {
    }

    function img($fn) {
         return "<img src='".$this->getImageURL($fn)."' border=0/>";
    }


    function getControls(&$appMan) 
    {
        if ($appMan->getAuth()->isUserValidFor("videostream", "w")) {
           $html = "<hr>";
           if (VideoStreamControl::isPlaying()) {
              $html .= $appMan->getAppletLink($this, "do", "stop", "Stop playing '".VideoStreamControl::getCurrentName()."'");
              $html .= "<hr>";
              $html .= "Watch this video at http://".VideoStreamControl::getServerIP().":".VideoStreamControl::getServerPort();
           } else {
              $html .= "Click on the media file to play it through the server";
           }
        }
      return $html;
   }

   function renderFileLine($appMan, $f)
   {
      $full = $this->_rootpath."/".$this->_dir."/".$f->_url;
      return $this->img("file.png")."&nbsp;".$appMan->getAppletLink($this, "play", $f->_idx, $f->_title)."<br/>";
   }

   function renderDirectoryLine($appMan, $d)
   {
      $full = $d->_dir;
      $link = $this->img("folder.gif")."&nbsp;".$d->getRenderLine($appMan, $this);
      return $link."<br/>";
   }

}

?>


