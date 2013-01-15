<?php
require_once 'system/setup.conf';
require_once 'system/utils.inc';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';
require_once 'warp/applets/browser/browser.inc';

class Warp_PhotoBrowser_Applet extends Warp_Browser_Applet
{
var $_filename;
var $_relativeRoot;

    function Warp_PhotoBrowser_Applet($caption = "Photos", $rootpath = NULL)
    {
        parent::__construct($caption, $rootpath);
        $this->_relativeRoot = $rootpath;
    }

    function getDirectory()
    {
        return "photobrowser";
    }

   function renderFileLine($appMan, $f)
   {
      $filename = "/".$this->_relativeRoot.$this->_dir."/".$f->_url;
      return "<a href=\"".$filename."\">".$f->_title."</a><br/>";
   }

   function fileSorter($a, $b) { 
    return (strcmp (strtolower($a->_title), strtolower($b->_title)));
   }

   function parseDirectory(&$appMan, $dir)
   {
      parent::parseDirectory($appMan, $dir);

      # Use the same function because both MarkedFile and MarkedDirectory
      # have a _title field.
      usort($this->_dirlist, array($this, "fileSorter") );
      usort($this->_filelist, array($this, "fileSorter") );
   }



}

?>


