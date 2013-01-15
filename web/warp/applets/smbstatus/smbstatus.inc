<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';

class Warp_SambaStatus_Applet extends Warp_Applet
{
    function Warp_SambaStatus_Applet($caption = "Samba Status")
    {
        parent::__construct($caption);
    }

    function getDirectory()
    {
        return "smbstatus";
    }

    function renderWindow(&$appMan)
    {
        if (!$appMan->getAuth()->isUserValidFor("system", "r")) {
           return "Not available";
        }

        $html.= "<p><b>SMB Mounts</b><br>";
        
        $cmd = "smbstatus";
        $result = shell_exec($cmd);

        $html.= "<pre>";
        $html.= str_replace("\n", "<br>", $result);
        $html.= "</pre>";

        return $html;
    }
}

?>

