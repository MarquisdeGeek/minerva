<?php

require_once 'warp/warplib/applet.inc';
require_once 'warp/conf/x10/x10.conf';
require_once 'webface/utils.inc';
require_once 'zinc/system/x10.inc';
require_once 'zinc/conf/x10floorplan.conf';
//require_once 'zinc/conf/x10control.js';
//require_once 'zinc/conf/x10devices.js';

class Warp_X10_Applet extends Warp_Applet
{
var $last_cmd;

    function Warp_X10_Applet($caption = "X10 House Control")
    {
        parent::__construct($caption);
    }
    
    function getDirectory()
    {
        return "x10";
    }

    function init(&$appMan)
    {
        $this->floor = appletUtils::getWrappedIndex($appMan, $this, "floor", 0, count(X10FloorPlan::getFloorList()));

        // Default to any mood settings
        $prm = $appMan->queryParameter($this, "mood");
        if ($prm != "") {
            Warp_X10_Config::goMood($prm);
        }

        // pickup any control parameters passed in 
        $prm = $appMan->queryParameter($this, "on");
	if ($prm != "") {
            X10::setStatus($prm, X10::$device_on);
	    $this->last_cmd = X10::getLastCommand();
	} 

        $prm = $appMan->queryParameter($this, "off");
	if ($prm != "") {
            X10::setStatus($prm, X10::$device_off);
	    $this->last_cmd = X10::getLastCommand();
	} 

        $devlist = X10FloorPlan::getFloorPlan($floor);
        foreach($devlist AS $dev => $ui) {
           $prm = $appMan->queryParameter($this, $dev);
           if ($prm != "") {
              X10::setBrightness($dev, $prm);
           }
        }

        return true;
    }
    
    function renderPanel(&$appMan)
    {
        return $html;
    }

    function getImage($appMan, $dev, $ui, $status) 
    {
       $stdImage = "<img width=$w height=$w style='position:relative; TOP:".$ui->y."px; LEFT:".$ui->x."px;' border=0 src=";
        $dir = $this->getPathStub()."img";

       switch($ui->type)
       {
       case X10DeviceUI::$TYPE_APPLIANCE:
          return $stdImage.($status==0 ? "$dir/off.png" : "$dir/on.png").">";

       case X10DeviceUI::$TYPE_LIGHT:
          return $stdImage.($status==0 ? "$dir/light0.png" : "$dir/light100.png").">";

       case X10DeviceUI::$TYPE_DIMMER:
return WebFace_Utils::renderSliderBar($appMan->webface,
                    $status, 0, 100, 4, $dev, null,
                    WebFace_Utils::$WARP_SLIDER_ARROWS_RIGHT,
                    WebFace_Utils::$WARP_SLIDER_NUMBER_NONE);
 
       }
    }
 
    function renderWindow(&$appMan)
    {
        $devlist = X10FloorPlan::getFloorPlan($floor);
        $floorList = X10FloorPlan::getFloorList();
        $dir = $this->getPathStub()."img";

        $html.= "Plan for ".$floorList[$this->floor]."<br/>";
        $html .= "<div align=center>";
        $html.= "<img src='$dir/floor".$this->floor.".png' border=0 />";

        foreach($devlist AS $dev => $ui) {
           if ($ui->floor != $this->floor) {
             continue;
           }

           $w = 64;
           if ($ui->type == X10DeviceUI::$TYPE_DIMMER) {
              $status = X10::getBrightness($dev);
           } else {
              $status = X10::getStatus($dev); 
           }
           $img = $this->getImage($appMan, $dev, $ui, $status);
           if ($status == 0) {
              $cmd="on";
           } else {
              $cmd="off";
           }

           $html .= $appMan->getAppletLink($this, $cmd, $dev, $img);
        //   $html.=$dev." = $status<br/>";
        }
        $html.="<hr>";
        $moodList = Warp_X10_Config::getMoodSettings();
        $moods = array();
        for($i=0;$i<count($moodList);++$i) {
           $moods[$i] = $appMan->getAppletLink($this, "mood", $i, $moodList[$i]);
       }

        $html.= appletUtils::getGridedContent($moods, 8);

       // $html.= X10::getLastCommand();
        //$html.= X10::getLastOutput();
        //$html.= X10::getLastError();
        //$html.= X10::getControllerStatus();
        $html.="<hr>";

       // Draw the floor selector
       $floors = array();
       for($i=0;$i<count($floorList);++$i) {
          $floors[$i] = $appMan->getAppletLink($this, "floor", $i, "<img src=$dir/floor".$i.".png width=200 border=0>");
          $floors[$i] .= "<br/>".$floorList[$i];
       }
       $html.= appletUtils::getGridedContent($floors, 3);

$html .="</div>";

       return $html;
    }

    function getHeadHTML(&$app_man)
    {
       $head = "<script src='zinc/conf/x10control.js'></script>";
//$head.="<script> function tst() { alert('kl'); } </script>";
       return $head;
    }
 
    function getRefreshParams(&$appMan)
    {
        $refresh  = $appMan->getArgument($this, "floor", $this->floor);
        return $refresh;
    }

    function renderConfig(&$appMan)
    {
        return $html;
    }

    function renderInfo(&$app_man, $fast=false)
    {
        if ($fast) {
            return "-";
        }

        return <<<EOD
<p>Control of X10 appliances and lighting through a web interface. This
is shown extensively in the <a href="http://resources.zdnet.co.uk/articles/video/0,1000002009,39291488,00.htm">zdnet homebrew video</a>. </p>
<p>Configuration for this applet is extensive, because of the differences
between houses. The files in question are:
<ul>
<li><tt>/warp/conf/x10/x10.conf </tt>
<p>This controls individual programs for the 'mood' settings which appear
underneath the main X10 control map. They include 'all lights off' and
'film mood'.
</p>
<li><tt>/zinc/conf/x10floorplan.conf</tt>
<p>This contains the list of X10 devices in the house, and their repsective
X,Y positions on the floorplan maps. These names must match the devices in
either Hu format (e.g. E3) or by alias names, as provided in the heyu
configuration file, <tt>/etc/heyu/x10.conf</tt>. x10floorplan.conf also
contains the names for each floor in the house.</p>
</li>
<li><tt>/warp/applets/x10/img</tt>
<p>In addition to the standard X10 graphics, this directory should also contain
the floorplan maps, stored as <tt>floor0.png</tt>, <tt>floor1.png</tt>, and so
on. It knows the number of floors because of <tt>x10floorplan.conf</tt>, covered
above.</p>
</li>
</ul>

<p><i>Known Bugs: </i>Some browsers (e.g. Firefox) have a problem if there
isn't room to display the whole image. This can happen if the window isn't
large enough for the graphics (which can be common since the floor
plans are quite large).<br/></p>
<p><i>Dependencies: </i>heyu, with confirming <tt>/etc/heyu/x10.conf</tt>.</p>

EOD;
    }

}

?>
