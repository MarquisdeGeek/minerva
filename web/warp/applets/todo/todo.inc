<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';

class Warp_TODO_Applet extends Warp_Applet
{
    var $_viewuser = "public";
    var $_thisuser = "public";
    var $_sortlist = false;
   
    function Warp_TODO_Applet($caption = "TODO List") 
    {
       parent::__construct($caption);
    }
 
    function getDirectory()
    {
        return "todo";
    }

    function init(&$appMan)
    {
        Warp_Applet::Init($appMan);
        $this->_thisuser = $appMan->getAuth()->getUser();

        // get state parameters 
        $this->_sortlist = $appMan->queryParameter($this, "sort", false); 
        $this->_viewuser = $appMan->queryParameter($this, "user", "public"); 

        // apply command parameters
        // Note: we use the existing results as default arguments, for
        // those cases where the command doesn't exist.
        $this->_sortlist = $appMan->queryParameter($this, "dosort", $this->_sortlist); 
        $this->_viewuser = $appMan->queryParameter($this, "douser", $this->_viewuser); 

        // protect against other users trying to impersonate someone
        // else by reverting to public.
        if ($this->_thisuser != $this->_viewuser) {
           $this->_viewuser = "public";
        }
        return true;
    }
    
    function renderPanel(&$appMan)
    {
        return exec("/usr/local/minerva/bin/todo list ".$this->_viewuser." |head -n 1");
    }
    
    function renderWindow(&$appMan)
    {
        $user = $this->_thisuser;
        $html = "Show: ";
        $html.= $appMan->getAppletLink($this, "dosort", "0", "Chronologically")." ";
        $html.= $appMan->getAppletLink($this, "dosort", "1", "Alphabetically");

        $html.= "   For: ";
        $html.= $appMan->getAppletLink($this, "douser", $user, $user)." ";
        $html.= $appMan->getAppletLink($this, "douser", "public", "Public");

        $html.= "<hr/>";

        exec("/usr/local/minerva/bin/todo list ".$this->_viewuser, $todolist);

        if ($this->_sortlist) {
            sort($todolist); 
        }

        $html .= "<ul>";
        foreach($todolist AS $item) {
          $html .= "<li>$item</li>";
        }
        $html .= "</ul>";

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

