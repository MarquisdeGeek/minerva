<?php
require_once 'system/setup.conf';
require_once 'system/utils.inc';
require_once 'warp/warplib/applet.inc';

class Warp_UA_Applet extends Warp_Applet
{
    function Warp_UA_Applet($caption = "UA")
    {
        parent::__construct($caption);
    }
   
    function getDirectory()
    {
        return "useragent";
    }
 
    function renderPanel(&$appMan)
    {
        $server = getIPAddress();
        $server.= "(".getServerName().")";
        $port = getServerPort()." / ".getRemotePort();
        
        $html = "<b>IP:</b> $server <i>($port)</i>";
        
        return $html;
    }
    
    function renderWindow(&$appMan)
    {
        $html.= "<p><b>Session object</b><br>";
        
        if ($_SESSION != null)
        foreach($_SESSION AS $key => $value) {
            $html.= "$key => $value<br>";
        }
        
        $html.= "<p><b>Get parameters</b><br>";
        foreach($_GET AS $key => $value) {
            $html.= "$key => $value<br>";
        }
        
        $html.= "<p><b>Post parameters</b><br>";
        foreach($_POST AS $key => $value) {
            $html.= "$key => $value<br>";
        }
        
        $html.= "<p><b>Cookies</b><br>";
        foreach($_COOKIE AS $key => $value) {
            $html.= "$key => $value<br>";
        }
        
        return $html;
    }
    
    
    function renderInfo()
    {
        return <<< HERE
<p>
This simple utility will display the IP(and port number) of the web browser in use.
</p>
<p>
It will also produce a list of all the session, post, get, and cookie information present.
</p>
HERE;
    }
    
}

?>

