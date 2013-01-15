<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';
require_once 'warp/conf/diskfree/diskfree.conf';

class Warp_Disk_Space_Applet extends Warp_Applet
{
    var $_which = 0;
    var $_dirlist = "";
   
    function Warp_Disk_Space_Applet($caption = "Free Space") 
    {
       parent::__construct($caption);
       $this->_dirlist = Warp_Disk_Free_Config::getDirectories();
    }
 
    function getDirectory()
    {
        return "diskfree";
    }

    function init(&$appMan)
    {
        Warp_Applet::Init($appMan);
  
        $this->_which = appletUtils::getWrappedIndex($appMan, $this, "current", 0, count($this->_dirlist));

        return true;
    }
    
    function renderPanel(&$appMan)
    {
        if (!$appMan->getAuth()->isUserValidFor("system", "r")) {
           return "Not available";
        }

        // This converts the associated array into a
        // numerically indexable array. We can then
        // reference elements directly.
        $dv =& array_values($this->_dirlist);
        $dk =& array_keys($this->_dirlist);
        
        $dirName = $dk[$this->_which];
        $dirPath = $dv[$this->_which];
        
        $free = $this->doBytes(disk_free_space($dirPath));
        $html = $dirName.":&nbsp;$free<br>";
        
        $anchor = $appMan->getStyleText("prev");
        $html.= $appMan->getAppletLink($this, "cmd", "prev", $anchor);
        
        $anchor = $appMan->getStyleText("next");
        $html.= $appMan->getAppletLink($this, "cmd", "next", $anchor);
        
        return $html;
    }
    
    function renderWindow(&$appMan)
    {
        if (!$appMan->getAuth()->isUserValidFor("system", "r")) {
           return "Not available";
        }

        $html.= "<table width=100% cellspacing=0>";
        $html.= "<tr bgcolor=#aaaaaa><td><b>Drive name</b></td><td><b>Path</b></td><td><b>Free</b></td><td><b>Used</b></td><td><b>Total</b></td><td>&nbsp;</td></tr>";

        $num = 0;
        foreach($this->_dirlist AS $name => $path)
        {
            $free = disk_free_space($path);
            $total = disk_total_space($path);
            $col = "bgcolor=".(($num & 1)?"#eeeeee":"#ffffff");
            $html.= "<td $col>$name</td>";
            $html.= "<td $col>$path</td><td $col>".$this->doBytes($free)."</td>";
            $html.= "<td $col>".$this->doBytes($total-$free)."</td>";
            $html.= "<td $col>".$this->doBytes($total)."</td>";
            $html.= "<td $col><table border=1 width=128><tr><td width=".(100-(100*$free)/$total)."% bgcolor=red></td><td bgcolor=lgray></td></table></td>";
            $html.= "</tr>";
            $num++;
        }
        $html.= "</table>";
        
        return $html;
    }
    
    function getRefreshParams(&$appMan)
    {
        return $appMan->getArgument($this, "current", $this->_which);
    }
    
    // This code taken from PHP on-line manual, but modified to use
    // &nbsp; instead of a normal space.
    function doBytes($size)
    {
        $kb = 1024;
        // Kilobyte
        $mb = 1024 * $kb;
        // Megabyte
        $gb = 1024 * $mb;
        // Gigabyte
        $tb = 1024 * $gb;
        // Terabyte
        if ($size==0) {
            return "n/a";
        }
        if ($size < $kb) {
            return $size." Bytes";
        } else if ($size < $mb) {
            return round($size/$kb,2)."&nbsp;kb";
        } else if ($size < $gb) {
            return round($size/$mb,2)."&nbsp;mb";
        } else if ($size < $tb) {
            return round($size/$gb,2)."&nbsp;gb";
        } else {
            return round($size/$tb,2)."&nbsp;tb";
        }
    }
    
    function renderInfo(&$app_man, $fast=false)
    {
        if ($fast) {
            return "-";
        }

        return <<<EOD
<p>Reports the free and used space on the configured drives, just like
<i>df</i>.</p>
<p>The configuration file, <tt>/warp/conf/diskfree/diskfree.conf</tt>
lets you specific which drives are to be reported.

EOD;
    }

}

?>

