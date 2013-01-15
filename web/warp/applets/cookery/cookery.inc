<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';
require_once 'warp/applets/static/static.inc';
require_once 'warp/conf/cookery/filelist.conf';

class Warp_Cookery_Info_Applet extends Warp_Static_Text_Applet 
{
    function getDirectory()
    {
        return "cookery";
    }

    function init(&$appMan)
    {
        Warp_Applet::Init($appMan);

        $this->_filelist = Warp_Cookery_Text_Reader::getFileList($this);
        $this->_which = appletUtils::getWrappedIndex($appMan, $this, "current", 0, count($this->_filelist));

        return true;
    }
    
    function renderWindow(&$appMan)
    {
        $html = Warp_Static_Text_Applet::renderFileListBar($appMan, 6);
        $html .= Warp_Static_Text_Applet::renderWindow($appMan);
        return $html;
    }

    function renderInfo(&$app_man, $fast=false)
    {
        return "";
    }

}

?>

