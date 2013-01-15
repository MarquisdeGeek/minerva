<?php
require_once 'system/setup.conf';
require_once 'minerva.conf';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';

class Warp_Manifest_Applet extends Warp_Applet
{
    var $_command = "";
   
    function Warp_Manifest_Applet($caption = "Manifest") 
    {
       parent::__construct($caption);
    }
 
    function getDirectory()
    {
        return "simulate/manifest";
    }

    function init(&$appMan)
    {
        Warp_Applet::Init($appMan);

        if ($appMan->getAuth()->isUserValidFor("system", "w")) {
           $start = $appMan->queryParameter($this, "start"); 
           $doprocess = $appMan->queryParameter($this, "do"); 
   
           if ($start != "") {
              $this->_command = "manifest default start $start 5";
           } else if ($doprocess != "") { 
              $this->_command = "manifest default $doprocess";
           }
   
           $cmd = getMinervaRoot()."/bin/".$this->_command;
           $out = system("($cmd &>/dev/null) &>/dev/null  &");
        }

        return true;
    }
    
    function renderPanel(&$appMan)
    {
        return exec(getMinervaRoot()."/bin/mstatus");
    }
    
    function renderWindow(&$appMan)
    {
        foreach (new DirectoryIterator(getMinervaRoot().'/etc/manifest') as $dir) {
          if (!$dir->isDot()) {
             $manifestList[] = $appMan->getAppletLink($this, "start","$dir","$dir");
          }
        }

        $html.= "Start Manifest : ";
        $html.= appletUtils::getGridedContent($manifestList, count($manifestList));
        $html.= "<br/>";

        $curr = exec(getMinervaRoot()."/bin/manifest default current");
        if ($curr == "") {
           $html.= "No current manifest is active...";
        } else {
           $controlList[] = $appMan->getAppletLink($this, "do","next","Next");
           $controlList[] = $appMan->getAppletLink($this, "do","more","More");
           $controlList[] = $appMan->getAppletLink($this, "do","stop","Stop");

           $html.= "Process $curr : ";
           $html.= appletUtils::getGridedContent($controlList, count($controlList));
        }

        if ($this->_command != "") {
           $html.= "<br/>Executing: <i>".$this->_command."</i><br/>";
        }

        return $html;
    }
    
    function getRefreshParams(&$appMan)
    {
    	return  
             $appMan->getArgument($this, "user", $this->_viewuser)."&".
             $appMan->getArgument($this, "sort", $this->_sortlist);

    }
    
    function renderInfo(&$app_man, $fast=false)
    {
        if ($fast) {
            return "-";
        }

        return "Instructions...";
    }
}

?>

