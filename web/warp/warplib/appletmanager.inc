<?php
// Applet webface
require_once 'PEAR.php';
require_once 'appface.inc';
require_once 'applet.inc';

// Minerva-specific
require_once 'minerva.conf';

// Prepare URL params for AppletManager functionality
define("WARP_MAXIMISE",         "max");
define("WARP_WINCONTENT",       "content");
define("WARP_WINTYPE",          "wintype");
define("WARP_MAIN",             "main");
define("WARP_INFO",             "info");
define("WARP_CONFIG",           "cfg");
define("WARP_TLD",              "WRP");
define("WARP_HTTP_MAX_REFRESH", 999999);

class Warp_Applet_Handle
{
    var $_applet;
    var $_id;   // for future
    var $_name;	// for future
    
    function &getApplet()
    {
        return $this->_applet;
    }
    
    function setApplet(&$applet)
    {
        $this->_applet =& $applet;
    }
    
    
}

class Warp_Applet_Manager
{
    var $webface;
    var $mainName;

    var $_applets;
    var $_maximisedAppId;
    var $_maximisedAppIdSet;
    var $_currentApplet;
    var $_cur = 0;
    var $sidebarwidth = 90;

    // for unique IDs
    var $_style = array("maximise" => array("img" => "up.gif", "alt" => "^"),
    "focused" => array("img" => "left.gif",      "alt" => "&lt;"),
    "refresh" => array("img" => "back.gif",      "alt" => "@"),
    "config"  => array("img" => "comp.blue.gif", "alt" => "C"),
    "close"   => array("img" => "burst.gif",     "alt" => "X"),
    "next"    => array("img" => "right.gif",     "alt" => "&gt;"),
    "prev"    => array("img" => "left.gif",      "alt" => "&lt;"),
    "up"      => array("img" => "up.gif",        "alt" => "^"),
    "down"    => array("img" => "down.gif",      "alt" => "v"),
    "help"    => array("img" => "hand.up.gif",   "alt" => "?")
    );
    
    // Public
    //	Initialisation
    function Warp_Applet_Manager($title = "WARP: PHP Applets by Steven Goodwin")
    {
        global $HTTP_GET_VARS;
        
        $this->mainName = $title;
        $this->webface = new Applet_Face();
 
        $this->_maximisedAppIdSet = false;
        foreach($HTTP_GET_VARS as $key => $value)
        {
            if ($key == WARP_MAXIMISE) {
                $this->_maximisedAppId = $value;
                $this->_maximisedAppIdSet = true;
                break;
            }
        }
    }
    
    function init()
    {
        $this->webface->init($this);
    }
   
    //	Panel & window rendering
    function getStyleText($button)
    {
        $butt =& $this->_style[$button];
        
        if ($butt["img"]) {
            $r = WarpSetup::getURLStub()."icons/";
            return '<img border =0 src ="'.$r.$butt['img'].'" alt ="'.$butt['alt'].'">';
        }
        
        return "[&nbsp;".$butt['alt']."&nbsp;]";
    }
    
    function renderAppletPanel(&$app)
    {
        $this->_currentApplet = &$app;
        $ctrl = $app->renderPanelControl($this, true);
        $main = $app->renderPanel($this);
        
        if ($ctrl != "" || $main != "") {
            $html .= "<table width=100% border=1 cellpadding=0 cellspacing=0 class=panel_table>";
            
            if ($ctrl != "") {
                $html .= "<tr><td class=panel_caption>$ctrl</td></tr>";
            }
            if ($main != "") {
                $html .= "<tr><td class=panel_body>$main</td></tr>";
            }
            
            $html .= "</table>\n";
        }
        
        return $html;
    }
    
    function renderAppletWindow(&$app)
    {
        $this->_currentApplet = &$app;
 
        $main .= "<table width=100% border=1 cellpadding=0 cellspacing=0 class=main_table><tr>";
        $main .= "<td class=main_caption width=100%>".$app->getWindowCaption()."</td>\n";
        $main .= "<td align=right width=1 class=main_control>";
        $main .= $this->getAppletRefreshLink();
        $main .= "</td>\n";
        
        $main .= "<td align=right width=1 class=main_control>";
        if ($app->renderConfig($this) != "") {
            $main .= $this->getAppletConfigLink($app);
        }
        $main .= "</td></tr>";
        $main .= "<tr><td colspan=4 class=main_body>";
        $main .= $app->renderWindow($this);
        $main .= "</td></tr></table>\n";

        $html = $this->webface->view->border($main, "main");

        return $html; 
    }
    
    function renderAppletConfig(&$app)
    {
        $this->_currentApplet = &$app;
        
        $main  = "<table width=100% border=1 class=cfg_table cellpadding=0 cellspacing=0><tr><td class=cfg_caption>";
        $main .= "Configuration</td><td class=cfg_caption2>".$app->getConfigCaption()."</td>\n";
        $main .= "<td align=right width=1 class=cfg_control>".$this->getAppletCloseLink($app)."</td></tr>\n";
        $main .= "<tr><td colspan=3 class=cfg_body>";
        $main .= $app->renderConfig($this);
        $main .= "</td></tr></table>\n";
        
        return $main;
    }
    
    function renderAppletInfo(&$app)
    {
        $this->_currentApplet = &$app;
        
        $main  = "<table width=100% border=1 class=happlet cellpadding=0 cellspacing=0><tr><td class=info_caption>";
        $main .= "Information</td><td class=info_caption2>".$app->getInfoCaption()."</td>\n";
        $main .= "<td align=right width=1 class=info_control>".$this->getAppletCloseLink($app)."</td></tr>\n";
        $main .= "<tr><td colspan=3 class=info_body>";
        $main .= $app->renderInfo($this, false);
        $main .= "</td></tr></table>\n";
        
        return $main;
    }
    
    
    function renderConfig()
    {
        if ($this->_applets == null) {
            return "";
        }
        
        $cfg  = $this->getWindowContents();
        
        foreach($this->_applets as $key => $value)
        {
            if ($key == $cfg) {
                $html .= $this->renderAppletConfig($value->getApplet());
            }
        }
        return $html;
    }
    
    function renderHeader()
    {
        $title = $this->mainName;
        $maxRefresh = WARP_HTTP_MAX_REFRESH;
       
        $app =& $this->getAppletFromID($this->getWindowContents());
        //if ($app.is_object()) {
        if (is_object($app)) {
            $title .= " - ".$app->getName();
        }
        
        if ($this->_applets != null) {
            $params = array();
            
            foreach($this->_applets as $key => $value)
            {
                $app =& $value->getApplet();
                $newest = $app->getHeadHTML($this);
                if ($newest != "") {
                    $headhtml .= $newest."\n";
                }
                
                $app->getBodyParams($this, $params);
                
                // How often do we automatically refresh the page
                $refresh = $app->getRefreshTime($this);
                if ($refresh != 0 && $refresh < $maxRefresh) {
                    $maxRefresh = $refresh;
                }
            }
            
            foreach($params as $fngroup => $callback)
            {
                $bodyparm .= $fngroup.'="';
                foreach($callback as $fn)
                {
                    $bodyparm .= $fn.'; ';
                }
                $bodyparm .='"';
            }
        }
        
        $bodyparm .= "class=main ";

        $headhtml .= '<script type="text/javascript" src="utils/ajax.js"></script>';
        
        if ($maxRefresh != WARP_HTTP_MAX_REFRESH) {
            $this->webface->dbug->info("Meta refresh given as $maxRefresh seconds");
            
            $link = $this->getRefreshURL();
            $headhtml .= "<META HTTP-EQUIV='Refresh' CONTENT='".$maxRefresh.";URL=$link'>\n";
        }
        
        $headhtml .= '<LINK rel="stylesheet" href="warp.css" type="text/css">';
        
        return $this->webface->view->htmlStart($title, $headhtml, $bodyparm);
    }
    
    //	render functions
    function renderWindow()
    {
        if ($this->_applets == null) {
            return "";
        }
        
        $win = "";
        // Maximised version of _current applet
        foreach($this->_applets as $key => $value)
        {
            if ($this->getWindowType() != WARP_MAIN) {
                if ($this->getWindowContents() == $key) {
                    switch ($this->getWindowType()) {
                    case	WARP_CONFIG:
                        $win .= $this->renderAppletConfig($value->getApplet());
                        break;
                        
                    case	WARP_INFO:
                        $win .= $this->renderAppletInfo($value->getApplet());
                        break;
                    }
                }
            } else if ($key == $this->getMaximisedAppletID()) {
                $win .= $this->renderAppletWindow($value->getApplet());
            }
        }
        return $win;
    }
    
    function renderPanels()
    {
        if ($this->_applets == null) {
            return "";
        }
        
        $panels = "";
        foreach($this->_applets as $key => $value)
        {
            $app =& $value->getApplet();
            $panels .= $this->renderAppletPanel($app);
        }
        return $panels;
    }
    
    function renderAll()
    {
        $win  = $this->renderWindow();
        $panels  = $this->renderPanels();
        
        $html  = "<table cellspacing=5 cellpadding=0 border=0 width=100%>";
        $html .= "<tr><td valign=top width=$this->sidebarwidth%>\n$win\n</td>";
        $html .= "<td valign=top width=$this->sidebarwidth%>\n$panels\n</td></tr>";
        $html .= "</table>";

        if (class_exists('MasterBar')) {
            $html .= MasterBar::generate($this);
        }

        return $html;
    }
    
    function renderEnd()
    {
        return $this->webface->view->htmlEnd();
    }

    function renderPage()
    {
        $app =& $this->getAppletFromID($this->getWindowContents());

        if (is_object($app)) {
            $app->renderPage($this);
        } else {
            $this->renderDefaultPage();
        }
    }

    function renderDefaultPage()
    {
        echo $this->renderHeader();
        echo $this->renderAll();
        echo $this->renderEnd();
    }


    // Public
    //	Maintenance
    function addApplet(&$applet)
    {
        $id  = $applet->getID($this);
        $name  = $applet->getName($this);
        
        $id  = strtoupper($id);
        
        if ($this->getAppletFromID($id) != null) {
            $this->webface->dbug->error("Applet $id already registered");
            return PEAR::raiseError("Applet $id already registered");
        }
        
        $this->webface->dbug->info("adding applet $id, $name");
        
        $nw = new Warp_Applet_Handle();
        $nw->setApplet($applet);

        $this->_applets[$id] = &$nw;
        
        // Make it maximised, if no others set
        if ($this->_maximisedAppIdSet == false) {
            $this->webface->dbug->warning("Maximising the first applet, since no default was given");
            $this->_maximisedAppId = $id;
            $this->_maximisedAppIdSet = true;
        }
        
        // Send message to say 'you're now registered'
        $this->_currentApplet = &$applet;
        if (!$applet->init($this)) {
            return PEAR::raiseError("Applet $id failed to initialise");
        }

    }
    
    function setDefaultApplet(&$applet)
    {
        $this->_maximisedAppId = $applet->getID($this);
        $this->_maximisedAppIdSet = true;
    }

    function queryArgument($argname)
    {
			global $HTTP_GET_VARS;
			$result = $HTTP_GET_VARS[$argname];
			$result= preg_replace("#\\\\(.)#e", "'\${1}'", $result);
			return $result;
    }
    
    function queryParameter(&$applet, $argName, $defaultValue="")
    {
        $prm = $this->queryArgument($this->getParameter($applet, $argName));
        if ($prm == "") {
           return $defaultValue;
        }
        return $prm;
    }
   
    function getParameterExist(&$applet, $argName)
    {
        $prm = $this->queryArgument($this->getParameter($applet, $argName));
        if ($prm == "") {
           return false;
        }
        return true;
    }

 
    function getMaximisedAppletID()
    {
        return $this->_maximisedAppId;
    }
    
    function isMaximisedAppletIDSet()
    {
        return $this->_maximisedAppIdSet;
    }
    
    function getParameter(&$applet, $argName)
    {
        // It is possible to have identical applets in a single page,
        // using different parameters.
        // In theory it's possible to use only applet's index and argument
        // but it kept for time being in case we decide to permit one applet
        // to affect another, on a different page.
        // i.e. return $idx."_".$argName;
        $idx = $this->getAppletIndex($applet);
        $argName = $this->fixHtml($argName);
        return $applet->getID($this).$idx."_".$argName;
    }
    
    function getArgument(&$applet, $argName, $argParam)
    {
        $argParam = $this->fixHtml($argParam);
        return $this->getParameter($applet, $argName)."=".$argParam;
    }
    
    function getArgumentString(&$applet, $argList)
    {
       $code = "";
       foreach($argList as $param => $value) {
           if ($code != "") {
               $code .= "&";
           }
           $code .= $this->getArgument($applet, $param, $value);
       }
       return $code;
    }

    function &getAppletFromID($id)
    {
        if ($this->_applets == null) {
            return null;
        }
        
        foreach($this->_applets as $key => $value)
        {
            $app =& $value->getApplet();
            if ($key == $id) {
                return $app;
            }
        }
        return null;
    }
   
    function getAppletIndex($applet) 
    {
        if ($this->_applets == null) {
            return -1;
        }

        $idx = 0;
        foreach($this->_applets as $key => $value)
        {
            $app =& $value->getApplet();
            if ($app == $applet) {
                return $idx;
            }
            ++$idx;
        }
        return -2;
    }

 
    // Private
    //	Maintenance: link creation
    function getRefreshArgumentList()
    {
        $args = "";
        
        if ($this->_applets == null) {
            $this->webface->dbug->warning("No applets, but we still try and create an argument list");
            return;
        }
        
        foreach($this->_applets as $key => $value)
        {
            $app =& $value->getApplet();
            $params = $app->getRefreshParams($this);
            if ($params != "") {
                $args .= $params;
                $args .= "&";
            }
        }
        return $args;
    }
    
    function getBaseURL()
    {
        return $_SERVER[PHP_SELF]."?".$this->getRefreshArgumentList();
    }

    function getMaximiseLink($maxApplet)
    {
        return WARP_MAXIMISE."=$maxApplet";
    }

    function getLinkBase()
    {
        return "<a href=";
    }
    
    function getEndLinkBase($style)
    {
        $text = $this->getStyleText($style);
        return ">$text</a>";
    }

    function getEndLinkBaseText($text)
    {
        return ">$text</a>";
    }
    
    function getWindowContents()
    {
        global $HTTP_GET_VARS;
        
        if ($HTTP_GET_VARS[WARP_WINCONTENT]) {
            return strtoupper($HTTP_GET_VARS[WARP_WINCONTENT]);
        }
        
        return $this->_maximisedAppId;
    }
    
    function getWindowType()
    {
        global $HTTP_GET_VARS;
        
        if ($HTTP_GET_VARS[WARP_WINTYPE]) {
            return $HTTP_GET_VARS[WARP_WINTYPE];
        }
        
        return WARP_MAIN;
    }
    
    function getContentParams($appId, $type)
    {
        if ($appId == "") {
            return "";
        }
        return WARP_WINTYPE."=$type&".WARP_WINCONTENT."=$appId&";
    }
    
    function getContentLink()
    {
        $content = $this->getWindowContents();
        $wintype = $this->getWindowType();
        
        return $this->getContentParams($content, $wintype);
    }
    
    // Public
    //	Accessing the webface components
    function &getCtrl()
    {
        return $this->webface->ctrl;
    }
    function &getView()
    {
        return $this->webface->view;
    }
    function &getAuth()
    {
        return $this->webface->auth;
    }
    function &getDbug()
    {
        return $this->webface->dbug;
    }
    
    // Public
    // 	Helpers for useful refresh times
    function getMinuteRefreshTime()
    {
        return 60;
    }
    function getHourRefreshTime()
    {
        return 60*60;
    }
    function getDayRefreshTime()
    {
        return 60*60*24;
    }
    function getWeekRefreshTime()
    {
        return 60*60*24*7;
    }
    
    // Public
    // 	Information about the applet manager
    
    function getCurrentApplet()
    {
        return $this->_currentApplet;
    }
    
    function getCurrentAppletID()
    {
        if ($this->_currentApplet == null) {
            return "";
        } else {
            return $this->_currentApplet->getID($this);
        }
    }
   
    // A collection of URLs for use in forms or other such links 
    function getStandardURL($id, $type="", $maximised="")
    {
        if ($maximised == "") {
            $maximised = $this->getMaximisedAppletID();
        }

        $url  = $this->getBaseURL();
        if ($type != "") {
            $url .= $this->getContentParams($id, $type);
        }
        $url .= $this->getMaximiseLink($maximised);
        return $url;
    }

    function getCloseURL()
    {
        return $this->getStandardURL("");
    }

    function getRefreshURL()
    {
        $id = $this->getWindowContents();
        $type = $this->getWindowType();
        return $this->getStandardURL($id, $type);
    }

    function getConfigURL($id)
    {
        return $this->getStandardURL($id, WARP_CONFIG);
    }

    function getInfoURL($id)
    {
        return $this->getStandardURL($id, WARP_INFO);
    }
 
    function getMaximiseURL($id)
    {
        return $this->getStandardURL($id, WARP_MAIN, $id);
    }

    // A collection of standard links, wrapping the URL calls
    function getAppletCloseLink()
    {
        $anchor  = $this->getLinkBase();
        $anchor .= $this->getCloseURL();
        $anchor .= $this->getEndLinkBase("close");
        return $anchor;
    }

    function getAppletRefreshLink()
    {
        $anchor  = $this->getLinkBase();
        $anchor .= $this->getRefreshURL();
        $anchor .= $this->getEndLinkBase("refresh");
        return $anchor;
    }

    function getAppletConfigLink(&$applet)
    {
        $id = strtoupper($applet->getID($this));
        $anchor  = $this->getLinkBase();
        $anchor .= $this->getConfigURL($id);
        $anchor .= $this->getEndLinkBase("config");
        return $anchor;
    }
   
    function getAppletInfoLink(&$applet)
    {
        $id = strtoupper($applet->getID($this));
        $anchor  = $this->getLinkBase();
        $anchor .= $this->getInfoURL($id);
        $anchor .= $this->getEndLinkBase("help");
        return $anchor;
    }
    
    function getAppletMaximiseLinkURL(&$applet, $text)
    {
        $id = strtoupper($applet->getID($this));
        $anchor  = $this->getLinkBase();
        $anchor .= $this->getMaximiseURL($id);
        $anchor .= $this->getEndLinkBaseText($text);
        return $anchor;
    }

    function getAppletMaximiseLink(&$applet)
    {
        return $this->getAppletMaximiseLinkURL($applet, $this->getStyleText("maximise"));
    }
    
    function getAppletFocusLink(&$applet)
    {
        $id = strtoupper($applet->getID($this));
        $anchor  = $this->getLinkBase();
        $anchor .= $this->getMaximiseURL($id);
        $anchor .= $this->getEndLinkBase("focused");
        return $anchor;
    }
    
    function getCurrentAppletLink($param, $value, $text)
    {
        $app =& $this->getCurrentApplet();
        return $this->getAppletLink($app, $param, $value, $text);
    }
    
    function getAppletLinkURL(&$applet, $param, $value)
    {
        // The anchor consists of four name parts
        // 1. The link to the page
        // 2. The _currently maximised applet (and other system args)
        // 3. Refresh tags for all _applets
        // 4. The specific instructions from this applet
        $anchor = $this->getBaseURL();
        $anchor .= WARP_MAXIMISE."=".$this->getMaximisedAppletID()."&";
        $anchor .= $this->getContentLink();
        $anchor .= $this->getArgument($applet, $param, $value);
        return $anchor;
    }

    function getAppletLink(&$applet, $param, $value, $text)
    {
        // The anchor consists of four name parts
        // 1. The link to the page
        // 2. The _currently maximised applet (and other system args)
        // 3. Refresh tags for all _applets
        // 4. The specific instructions from this applet
        $anchor  = $this->getLinkBase();
        $anchor .= $this->getAppletLinkURL($applet, $param, $value);
        $anchor .= ">$text</a>";
        return $anchor;
    }
    
    function getUniqueID()
    {
        while (true) {
            $id = sprintf("%s%03dX", WARP_TLD,$this->_curr);
            
            if ($this->getAppletFromID($id) == null) {
                return $id;
            }
            
            if (++$this->_curr == 1000) {
                $this->_curr = 0;
            }
        }
    }

    // Generation of the master bar
    function getMasterBarStart()
    {
        return '<table border=1 cellpadding=0 cellspacing=0 width=100%><tr width=100%>';
    }

    function getMasterBarEntry($title, $url, $width="")
    {
        $entry = '<td bgcolor="white" align="center" '.${width}.'>';
        if ($url == "") {
            $entry .= "<b>$title</b>";
        } else {
            $entry .= "<a href=$url>$title</a>";
        }
        return $entry.'</td>';
    }

    function getMasterBarEnd()
    {

        return "</tr></table>";
    }

    function getMasterBar($array_entries)
    {
        $bar = $this->getMasterBarStart();
        $width = "width=\"".(100 / count($array_entries))."%\"";
        foreach($array_entries as $title => $url) {
            $bar.= $this->getMasterBarEntry($title, $url, $width);
        }
        $bar.= $this->getMasterBarEnd();
        return $bar;
    }

    function getMasterBarPanel($array_entries)
    {
        foreach($array_entries as $title => $url) {
            $bar.= "<a href=$url>$title</a><br/>";
        }
        return $bar;
    }

    function fixHtml($str) 
    {
      // $str = str_replace(" ", "%20", $str);
       //$str = str_replace("&", "%26", $str);
		$str = preg_replace("/([^\/0-9a-zA-Z])/e", "fixEncode('\\1')", $str);
       return htmlentities($str);
    }
 
}
	function fixEncode($c) {
		return "%".dechex(ord($c));
	}


class Warp_Applet_ManagerFullScreen extends Warp_Applet_Manager
{
   function renderAll()
    {
        $html = $this->renderWindow();

        if (class_exists('MasterBar')) {
            $html .= MasterBar::generate($this);
        }

        return $html;
    }
}

class Warp_Applet_ManagerTouch extends Warp_Applet_Manager
{
    function renderPanels()
    {
        if ($this->_applets == null || count($this->_applets) == 1) {
            return "";
        }

        return parent::renderPanels();
    }


    function renderAppletPanel(&$app)
    {
        $this->_currentApplet = &$app;
        $ctrl = $app->renderPanelTitle($this, true);
        $url = $this->getMaximiseURL($app->getID($this));
        $icon = $app->getIconFullURL();

        $html .= "<table width=100% border=1 cellpadding=0 cellspacing=0 class=panel_table onclick=\"window.location='".$url."'\">";

        $iconWidth = $iconHeight = 128;

        $browser = getBrowser();
        if ($browser->knownSize()) {
           if ($browser->getWidth() <= 480) {
              $iconWidth = $iconHeight = 32;
           } else if ($browser->getWidth() <= 640) {
              $iconWidth = $iconHeight = 64;
           }
        }

        $html .= "<tr><td class=panel_caption>$ctrl</td></tr>";
        $html .= "<tr><td class=panel_icon><img src=$icon width=$iconWidth height=$iconWidth border=0></td></tr>";
        $html .= "</table>\n";

        return $html;
    }


}

?>

