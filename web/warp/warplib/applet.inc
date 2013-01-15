<?php

class Warp_Applet
{
    var $format;
    var $mainTitle;
    var $id;
    var $_name;

    function Warp_Applet($captionName = "Warp Applet")
    {
        $this->_name = $captionName;
    }
    
    function init(&$appMan)
    {
        return true;
    }

    function getID(&$appMan)
    {
        if ($this->id == "") {
            $this->id = $appMan->getUniqueID();
        }
        
        return $this->id;
    }
    
    
    function getName()
    {
        return $this->_name; 
    }
   
    function getPanelCaption()
    {
        return $this->getName();
    }
 
    function getWindowCaption()
    {
        return $this->getName();
    }
 
    function getConfigCaption()
    {
        return $this->getName();
    }
 
    function getInfoCaption()
    {
        return $this->getName();
    }
 
    function getIconCaption()
    {
        return $this->getName();
    }
 
    function getDirectory()
    {
        return "";
    }
    
    function getIconURL()
    {
        return "icon.png";
    }
   
    function getIconFullURL()
    { 
        $icon = $this->getIconURL();
        if ($icon != "") {
           if (file_exists($this->getFilesystemPathStub().$icon)) {
              $icon = $this->getPathStub().$icon;
           } else {
              $icon = WarpSetup::getURLStub()."/icons/defaulticon.png";
           }
        }
        return $icon;
    }

    function getPathStub()
    {
        return WarpSetup::getURLStub()."/warp/applets/".$this->getDirectory()."/";
    }

    function getFilesystemStub()
    {
       return WarpSetup::getPathRoot()."/".WarpSetup::getURLStub()."/warp/applets/".$this->getDirectory()."/";
    }
 
    function getAppletFileName($filename)
    {
        return $this->getPathStub().$filename;
    }
 
    function getConfFileName($filename)
    {
        return WarpSetup::getURLStub()."/warp/conf/".$this->getDirectory()."/".$filename;
    }

    function getFilesystemPathStub()
    {
        return WarpSetup::getPathRoot().$this->getPathStub();
    }

    function getStdImageURL($img)
    {
        return WarpSetup::getURLStub()."/icons/$img";
    }

    function getImageURL($img)
    {
        return $this->getPathStub()."images/$img";
    }
 
    function getRefreshParams(&$appMan)
    {
        return "";
    }
    
    // How long until the whole page does an automatic refresh
    // Measured in seconds. 0 means no refresh.
    function getRefreshTime(&$appMan)
    {
        return 0;
    }
    
    function getHeadHTML(&$appMan)
    {
        return "";
    }
    
    function getBodyParams(&$appMan)
    {
        return "";
    }
    
    // Override this parameter to change (or even remove) the caption bar
    function renderPanelControl(&$appMan, $fast=false)
    {
        $add = "";
        if ($this->renderInfo($appMan, true)) {
            $add.= "<td align=right width=1 class=panel_control>";
            $add.= $appMan->getAppletInfoLink($this);
            $add.= "</td>";
        }
        
        if ($this->renderConfig($appMan, true)) {
            $add.= "<td align=right width=1 class=panel_control>";
            $add.= $appMan->getAppletConfigLink($this);
            $add.= "</td>";
        }
        
        $add.= "<td align=right width=1 class=panel_control>";
        if ($appMan->getMaximisedAppletID() == $this->getID($appMan)) {
           $add.= $appMan->getAppletFocusLink($this);
        }
        else {
           $add.= $appMan->getAppletMaximiseLink($this);
        }

        return $this->renderPanelTitle($appMan, $fast, $add);
    }
 
    function renderPanelTitle(&$appMan, $fast=false, $additions="")
    {
        $bar = "<table cellspacing=0 cellpadding=0 width=100% border=0><tr>";
        $bar.= "<td align=left class=panel_caption>".str_replace(" ", "&nbsp;", $this->getPanelCaption())."</td>";

        $bar .= $additions;
        $bar.= "</td>";
        $bar.= "</tr></table>";

        return $bar;
    }

    
    // The main text inside the panel on the RHS
    function renderPanel(&$appMan)
    {
        return "";
    }
    
    // The large window. Include main web page here
    function renderWindow(&$appMan)
    {
        return "";
    }
    
    // Configuration forms. Return "" for no configuration, and
    // no icon will be generated. If $fast=true, you can return
    // either an empty string, or an arbitrary non-empty string, as
    // these cases are used solely to test whether to render an
    // 'config icon?' button.
    function renderConfig(&$appMan, $fast=false)
    {
        return "";
    }
    
    // For the help/about page(s)
    function renderInfo(&$appMan, $fast=false)
    {
        return "";
    }

    function renderPage(&$appMan)
    {
        $appMan->renderDefaultPage();
    }
    
}

?>

