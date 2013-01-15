<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';

class MarkedFile
{
  var $_url;
  var $_title;
  var $_description;
  var $_idx;
  var $_dpage;
  var $_fpage;
  var $_dpageSize;
  var $_fpageSize;

  function MarkedFile($idx, $url, $title, $description) {
     $this->_idx = $idx;
     $this->_url = $url;
     $this->_title = $title;
     $this->_description = $description;
  }

  function getRenderLine() {
     return "<a href='".$this->_url."'>$this->_title</a>";
  }

}

class MarkedDirectory
{
  var $_dir;
  var $_title;

  function MarkedDirectory($dir, $title) {
     $this->_dir = $dir;
     $this->_title = $title;
  }

  function getRenderLine($appMan, $app) {
     return $appMan->getAppletLink($app, "dir", $this->_dir, $this->_title);
  }

}


class Warp_Browser_Applet extends Warp_Applet
{
    var $_dir;
    var $_dirlist;
    var $_filelist;
    var $_roopath;
    var $_sortFilenames;
    var $_sortDirnames;

    function Warp_Browser_Applet($caption = "Browser", $rootpath = NULL)
    {
        parent::__construct($caption);
        if ($rootpath == NULL || $rootpath == "") {
           $this->_rootpath = $this->getConfFileName($this->_dir);
        } else {
           # Absolute paths are used as is,
           # relative paths are treated relative to our root
           if (substr($rootpath, 0, 1) == "/") {
              $this->_rootpath = $rootpath;
           } else {
              $this->_rootpath = WarpSetup::getPathRoot()."/".$rootpath;
           }
        }
        $this->_dpageSize = 20;
        $this->_fpageSize = 15;
        $this->_sortFilenames = $this->_sortDirnames = true;
    }

    function getDirectory()
    {
        return "browser";
    }

    function init(&$appMan)
    {
        Warp_Applet::init($appMan);

        $this->_dpage = $appMan->queryParameter($this, "dpset");
        $this->_fpage = $appMan->queryParameter($this, "fpset");

        if ($this->_dpage == "") {
           $this->_dpage = $appMan->queryParameter($this, "dpage");
        }
        if ($this->_fpage == "") {
           $this->_fpage = $appMan->queryParameter($this, "fpage");
        }

        if ($this->_dpage == "") $this->_dpage = 0;
        if ($this->_fpage == "") $this->_fpage = 0;

        $dir = $appMan->queryParameter($this, "dir");
        $curr = $appMan->queryParameter($this, "current");

        if ($dir != "") {
           $this->_dir = $dir;
           // If we specify a directory, reset the page counters to zero
           $this->_dpage = 0;
           $this->_fpage = 0;
        } else { 
           $this->_dir = $curr;
        }
        if ($this->_dir == "") {
           $this->_dir = "/";
        }

        $this->_dir = appletUtils::validatePath($this->_dir);
        $this->parseDirectory($appMan, $this->_dir);

        return true;
    }

    function renderPanel(&$appMan)
    {
        return $html;
    }

    function getDirectoryPane($appMan)
    {
        $parentDir = appletUtils::getParentDirectory($this->_dir);
        if ($parentDir != $this->_dir) {
           $dirpane = $this->renderParentDirectoryLine($appMan, $parentDir);
        }

        $size = count($this->_dirlist);
        $first = $this->_dpage * $this->_dpageSize;
        $last = $first + $this->_dpageSize;
        $first = max($first, 0);
        $last = min($last, $size);

        for($i=$first;$i<$last;++$i) {
           $name = $this->_dirlist[$i];
           $dirpane .= $this->renderDirectoryLine($appMan, $name);
        }

        $dirpane .= appletUtils::getPagerSelect($appMan, $this, $this->_dpage, $size, $this->_dpageSize, "dpset");

        return $dirpane;
    }

    function getFilePane(&$appMan)
    {
        $filepane = "";

        $size = count($this->_filelist);
        $first = $this->_fpage * $this->_fpageSize;
        $last = $first + $this->_fpageSize;
        $first = max($first, 0);
        $last = min($last, $size);

        for($i=$first;$i<$last;++$i) {
           $name = $this->_filelist[$i];
           $filepane .= $this->renderFileLine($appMan, $name);
        }

        $filepane .= appletUtils::getPagerSelect($appMan, $this, $this->_fpage, $size, $this->_fpageSize, "fpset");

        return $filepane;
    }
  
    function getDirectoryBrowser(&$appMan, $dir)
    {
        $dirs = split("/", $dir);
        $top = $appMan->getAppletLink($this, "dir", "/", "[Root]");
        $path = "";
        foreach($dirs AS $d) {
           if ($d == "") continue;
           $path.= "/$d";
           $top .= "&nbsp;/&nbsp;";
           $top .= $appMan->getAppletLink($this, "dir", $path, "$d");
        }
        $top .= "<br/><br/>";
        return $top;
    }

    function renderWindow(&$appMan)
    {
        $top = $this->getDirectoryBrowser($appMan, $this->_dir);
        $filepane = $this->getFilePane($appMan);
        $dirpane = $this->getDirectoryPane($appMan);
        return $top . $appMan->getView()->split2V($dirpane, $filepane);
    }

    function renderInfo(&$appMan)
    {
       return "This is the generic browser applet, it is not usually used on its own, but rather as a base class. See MP3Play and Explorer for examples. The root directory in each case should be specified when you construct (via new) the applet.";
    }

    function renderFileLine(&$appMan, $f)
    {
       return $f->_title."<br/>";
       // We don't give links by default; but if we did - this is how
       // we'd do it!
       #return $f->getRenderLine()."<br/>";
    }

    function renderDirectoryLine($appMan, $d)
    {
       return $d->getRenderLine($appMan, $this)."<br/>";
    }

    function renderParentDirectoryLine($appMan, $p) 
    {
       return $appMan->getAppletLink($this, "dir", $p, "[Parent]")."<br/>";
    }

    function getRefreshParams(&$appMan)
    {
        return 
          $appMan->getArgument($this, "current", $this->_dir)."&".
          $appMan->getArgument($this, "dpage", $this->_dpage)."&".
          $appMan->getArgument($this, "fpage", $this->_fpage);
    }

//
	
	function sortFilename($a, $b) {
		return $a->_url > $b->_url;
	}

	function sortDirectory($a, $b) {
		return $a->_dir > $b->_dir;
	}

    function parseDirectory(&$appMan, $dir) {

       $fullPath = $this->_rootpath."/".$dir;

       $this->_dirlist = array();
       $this->_filelist = array();
			$this->getDirectoryList($appMan, $dir, $this->_dirlist, $this->_filelist);
	}

    function getDirectoryList(&$appMan, $dir, &$dirlist, &$filelist) {
       $fullPath = $this->_rootpath."/".$dir;

       if (@$handle = opendir($fullPath)) {
			 $fileList = array();
          while (false !== ($file = readdir($handle))) {
              if (substr($file, 0, 1) != ".") {
                  if (is_dir("$fullPath/$file")) {
                     if (is_file("$fullPath/$file/.name")) {
                        $name = file_get_contents("$fullPath/$file/.name");
                     } else {
                        $name = $file;
                     }

                    $file = appletUtils::validatePath("$dir/$file");
                    $name = appletUtils::validatePath($name);
                    $dirlist[] = new MarkedDirectory($file, $name);

                  } else if (is_file("$fullPath/$file")) {
							$fileList[] = $file;
                  }
             }
          }
          closedir($handle);

			if ($this->_sortFilenames) {
				sort($fileList);//, array("Warp_Browser_Applet","sortFilename"));
			}
			$fileIdx = 0;
			foreach($fileList as $file) {
				$filelist[] = new MarkedFile($fileIdx, $file, $file, "");
				++$fileIdx;
			}

			if ($this->_sortDirnames) {
				usort($dirlist, array("Warp_Browser_Applet","sortDirectory"));
			}
      }
  }


}

?>


