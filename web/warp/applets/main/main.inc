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

    function getDirectory()
    {
       return "main";
    }

    function renderPanel(&$appMan) 
    {
       if (class_exists('MasterBar') && $appMan->getMaximisedAppletID() == $this->getID($appMan)) {
          $main .= $appMan->getMasterBarPanel(MasterBar::getPages());
       }
       return $main;
    }

    function renderWindow(&$appMan)
    {
        $idx = 0;
        $links = array();
        foreach($appMan->_applets as $key => $value)
        {
            $app =& $value->getApplet();
            if ($app != $this) {
                $id = $app->getID($appMan);
                $main  = "<div align=center>";
                $main .= $appMan->getLinkBase().$appMan->getMaximiseURL($id);

                $icon = $app->getIconFullURL();
                $icon = "<img src='".$icon."' border=0/>";

                $main .= $appMan->getEndLinkBaseText($icon."<br/>".$app->getIconCaption());
                $main .= "</div>";
                $links[$idx] = $main;
                ++$idx;
            }
        }

        $main = "<div align=center>".appletUtils::getGridedContent($links, 3)."</div>";
         if (class_exists('MasterBar')) {
            $main .= "<br/>";
            $main .= $appMan->getMasterBar(MasterBar::getPages());
         }
         return $main;
    }
}

?>

