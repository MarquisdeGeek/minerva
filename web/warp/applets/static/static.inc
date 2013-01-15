<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';
require_once 'warp/conf/static/static.conf';

class Warp_Static_Text_Applet extends Warp_Applet
{
    var $_which = 0;
    var $_filelist;
   
    function Warp_Static_Text_Applet($caption = "Information")
    {
        parent::__construct($caption);
    }

    function getDirectory()
    {
        return "static";
    }

    function init(&$appMan)
    {
        Warp_Applet::Init($appMan);

        $this->_filelist = Warp_Static_Text_Reader::getFileList();  
        $this->_which = appletUtils::getWrappedIndex($appMan, $this, "current", 0, count($this->_filelist));

        return true;
    }
    
    function renderFileListBar(&$appMan, $width)
    {
        $num = 0;
        $barGrid = array();
        foreach($this->_filelist AS $name => $file)
        {
		$barGrid[] = $appMan->getAppletLink($this, "setidx", $num, $name);
        	++$num;
        }
        return "<div align=center>".appletUtils::getGridedContent($barGrid, $width)."</div>";
    }

    function renderPanel(&$appMan)
    {
        $num = 0;
        foreach($this->_filelist AS $name => $file)
        {
            if ($num == $this->_which) {
                $html.= $name;
                $anchor = $appMan->getStyleText("prev");
                $html.= $appMan->getAppletLink($this, "cmd", "prev", $anchor);
                
                $anchor = $appMan->getStyleText("next");
                $html.= $appMan->getAppletLink($this, "cmd", "next", $anchor);
            }
            ++$num;
        }

        return $html;
    }
    
    function renderWindow(&$appMan)
    {
        $num = 0;
        foreach($this->_filelist AS $name => $file)
        {
            if ($num == $this->_which) {
                $html.= "<h1>$name</h1>";
                $html.= file_get_contents($file);
            }
            ++$num;
        }
        
        return $html;
    }
    
    function getRefreshParams(&$appMan)
    {
        return $appMan->getArgument($this, "current", $this->_which);
    }
    
    function renderInfo(&$app_man, $fast=false)
    {
        if ($fast) {
            return "-";
        }

        return <<<EOD
<p>Simply renders a file (with HTML tags, if supplied) into the window. Useful
for help pages, and static reports. The default install includes a short
commentary on WebFace and WARP. These files are held in the
<tt>/warp/conf/static/</tt> directory. It multiple files exist, you can
scroll through them with the left and right arrows.
EOD;
    }

}

?>

