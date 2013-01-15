<?php
require_once 'system/setup.conf';
require_once 'minerva.conf';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';

class Warp_Cosmic_Applet extends Warp_Applet
{
    var $_command = "";

    function Warp_Cosmic_Applet($caption = "Cosmic")
    {
       parent::__construct($caption);
    }

    function getDirectory()
    {
        return "simulate/cosmic";
    }

    function init(&$appMan)
    {
        Warp_Applet::Init($appMan);

        if ($appMan->getAuth()->isUserValidFor("system", "w")) {

           $base = $appMan->queryParameter($this, "base"); 
           $cosmic = $appMan->queryParameter($this, "cx"); 

           if ($base != "") {
              $this->_command = "cosmic default base$base";
           } else if ($cosmic != "") { 
              $this->_command = "cosmic default c$cosmic";
           }
   
           $cmd = getMinervaRoot()."/bin/".$this->_command;
           $out = system("($cmd &>/dev/null) &>/dev/null  &");
        }

        return true;
    }
    
    function renderPanel(&$appMan)
    {
        return shell_exec(getMinervaRoot()."/bin/cosmic text modestatus");
    }
 
    function renderWindow(&$appMan)
    {
       $cosmicList[] = $appMan->getAppletLink($this, "base","1","Quieter");
       $cosmicList[] = $appMan->getAppletLink($this, "base","2","Louder");
       $cosmicList[] = $appMan->getAppletLink($this, "base","3","Mute/Unmute");
       $cosmicList[] = $appMan->getAppletLink($this, "base","4","Pause/Resume");

       $cosmicList[] = "";
       $cosmicList[] = "";

       $mode = shell_exec(getMinervaRoot()."/bin/cosmic default mode");
       $mode = chop($mode); // remove CR
       $base = getMinervaRoot()."/etc/cosmic/$mode";

       for($i=1;$i<9;++$i) {
          if (file_exists("$base/c$i")) {
             $cosmicList[] = $appMan->getAppletLink($this, "cx","$i","$i");
          } else {
             $cosmicList[] = "";
          }
       }

       $cosmicList[] = "";
       $cosmicList[] = "";

       $cosmicList[] = $appMan->getAppletLink($this, "base","5","Mode Status");
       $cosmicList[] = $appMan->getAppletLink($this, "base","6","Next Mode");
       $cosmicList[] = $appMan->getAppletLink($this, "base","7","Status");
       $cosmicList[] = $appMan->getAppletLink($this, "base","8","Reset");

        $html.= appletUtils::getGridedContent($cosmicList, 2);

        if ($this->_command != "") {
           $html.= "<br/>Executing: <i>".$this->_command."</i><br/>";
        }

        return $html;
    }
    
}

?>

