<?php
require_once 'warp/system/setup.conf';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';
require_once 'warp/conf/cookery/filelist.conf';

class Warp_Cookery_Info extends Warp_Static_Text_Applet 
{
    function getDirectory()
    {
        return "cookery";
    }

    function init(&$appMan)
    {
        Warp_Applet::Init($appMan);

        $this->_filelist = Warp_Static_Text_Reader::getFileList();  
        $this->_which = appletUtils::getWrappedIndex($appMan, $this, "current", 0, count($this->_filelist));

        return true;
    }
    
    function renderInfo(&$app_man, $fast=false)
    {
        return "";
    }

}

?>

