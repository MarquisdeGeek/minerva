<?php
require_once 'webface/webctrl.inc';

class Applet_Ctrl extends Web_Ctrl
{
    var $_appman;
    
    function init(&$appMan)
    {
        $this->_appman =& $appMan;
    }
    
    function getParameterValue($param)
    {
        $app =& $this->_appman->getCurrentApplet();
        $full = $this->_appman->getParameter($app, $param);
        return $this->_appman->queryArgument($full);
    }
    
    function createAnchorParam($param)
    {
        $app =& $this->_appman->getCurrentApplet();
        return $this->_appman->getParameter($app, $param);
    }
    
    function createAnchor($param, $value, $text)
    {
        return $this->_appman->getCurrentAppletLink($param, $value, $text);
    }
}
?>

