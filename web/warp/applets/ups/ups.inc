<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';

class Warp_UPSMonitor_Applet extends Warp_Applet
{
    function Warp_UPSMonitor_Applet($caption = "UPS Monitor")
    {
        parent::__construct($caption);
    }

    function getDirectory()
    {
        return "ups";
    }

    function renderWindow(&$appMan)
    {
        if (!$appMan->getAuth()->isUserValidFor("system", "r")) {
           return "Not available";
        }

        $upsUnits = shell_exec("/bin/upsc -l");
        $upsUnits = split("\n", $upsUnits);

        foreach($upsUnits as $ups) {
           if ($ups == "") continue;
           $html.= "<h1>$ups</h1>";
           $result = shell_exec("/bin/upsc ".$ups);

           $html.= "<pre>";
           $html.= str_replace("\n", "<br>", $result);
           $html.= "</pre>";
        }

        return $html;
    }
}

?>

