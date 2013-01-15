<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';

class Warp_Main_Applet extends Warp_Applet
{

    function Warp_Main_Applet($caption = "Minerva")
    {
        parent::__construct($caption);
    }

    function renderWindow(&$appMan)
    {
        $main = "<div align=center><img src=index.png></div>";
        $main .= "<div align=center>".appletUtils::getGridedContent($links, 3)."</div>";
         if (class_exists('MasterBar')) {
            $main .= "<br/>";
            $main .= $appMan->getMasterBar(MasterBar::getPages());
         }
         return $main;
    }
}

?>

