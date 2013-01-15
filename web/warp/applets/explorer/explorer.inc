<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';
require_once 'warp/applets/browser/browser.inc';

class Warp_Explorer_Applet extends Warp_Browser_Applet
{
var $_filename;

    function Warp_Explorer_Applet($caption = "Explorer", $rootpath = NULL)
    {
        parent::__construct($caption, $rootpath);
    }

    function getDirectory()
    {
        return "explorer";
    }

    function init(&$appMan)
    {
        Warp_Browser_Applet::init($appMan);

        $view = $appMan->queryParameter($this, "view");
        if ($view != "") {
           $this->_filename = $this->_filelist[$view]->_url;
        }
        return true;
    }


    function renderWindow(&$appMan)
    {
        $top = $this->getDirectoryBrowser($appMan, $this->_dir);
        $filepane = $this->getFilePane($appMan, 0);
        $dirpane = $this->getDirectoryPane($appMan, 0);
        $preview = $this->getPreviewHTML($appMan, $this->_filename);
        $rhs = $appMan->getView()->split2H($filepane, $preview);
        return $top . $appMan->getView()->split2V($dirpane, $rhs);
    }

    function renderInfo(&$appMan)
    {
        $p = parent::renderInfo($appMan);
        $p.= "<p>This is the Explorer variant, that also allows you to preview .txt files.";
        return $p;
    }

    function getPreviewHTML(&$appMan, $fn)
    {
       # We only preview text files because we can't guarantee
       # that files that need direct links (e.g. images) will be
       # visible through the web server.
       $full = $this->_rootpath."/".$this->_dir."/".$fn;
       if (appletUtils::isTextFile($full)) {
          $txt = file_get_contents($full);
          return "<hr><h3>$fn</h3><textarea rows=16 cols=50 readonly>$txt</textarea>";
       }

       return "";
    }

    function renderFileLine($appMan, $f)
    {
       $full = $this->_rootpath."/".$this->_dir."/".$f->_url;
       if (appletUtils::isTextFile($full)) {
          return $appMan->getAppletLink($this, "view", $f->_idx, $f->_title)."<br/>";
       }
       return parent::renderFileLine($appMan, $f);
    }

}

?>


