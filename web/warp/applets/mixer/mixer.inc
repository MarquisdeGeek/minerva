<?php

require_once 'warp/warplib/applet.inc';
require_once 'webface/utils.inc';
require_once 'zinc/system/mixer.inc';
require_once 'zinc/conf/mixer.conf';

class Warp_Mixer_Applet extends Warp_Applet
{
    function Warp_Mixer_Applet($caption = "Mixer")
    {
        parent::__construct($caption);
    }
    
    function getID()
    {
        return "MIN002A";
    }

    function getDirectory()
    {
        return "mixer";
    }

    function init(&$appMan)
    {
        Warp_Applet::Init($appMan);

        if ($appMan->getAuth()->isUserValidFor("mixer", "w")) {
            $miniMixVolume = $appMan->queryParameter($this, "rvol");
            if ($miniMixVolume != "") {
                Mixer::setVolume(Mixer::$MASTER, $miniMixVolume);
            }
            $incVol = $appMan->queryParameter($this, "inc");
            if ($incVol != "") {
                Mixer::incVolume($incVol, 1);
            }
            $decVol = $appMan->queryParameter($this, "dec");
            if ($decVol != "") {
                Mixer::decVolume($decVol, 1);
            }
            $slider = $appMan->queryParameter($this, "slider");
            if ($slider != "") {
               if (preg_match('/([A-Za-z]+)([12])\?([0-9]*),([0-9]*)/', $slider, $res)) {
                   $curr = Mixer::getVolume($res[1]);
                   if ($res[2] == '1') {   // top half
                       $vol = 200-$res[4];
                       $vol /= 2;
                   } else {
                       $vol = $curr-$res[4]/2;
                   }
                   Mixer::setVolume($res[1], $vol);
                }
            }
        }

        return true;
    }
    
    function renderPanel(&$appMan)
    {
        if ($appMan->getAuth()->isUserValidFor("mixer", "r")) {
            $staticSlider = !$appMan->getAuth()->isUserValidFor("mixer", "w");
            $vol = Mixer::getVolume(Mixer::$MASTER);
            $dir = WarpSetup::getURLStub()."icons/";
            $images[] = "$dir/left.gif";
            $images[] = "$dir/right.gif";
            $images[] = "$dir/ball.red.gif";
            $images[] = "$dir/ball.gray.gif";
     
            $html .= WebFace_Utils::renderSliderBar($appMan->webface, 
                    $vol, 0, 100, 5, "rvol", $images, 
                    WebFace_Utils::$WARP_SLIDER_ARROWS_RIGHT, 
                    WebFace_Utils::$WARP_SLIDER_NUMBER_NONE, $staticSlider);
            $html .= "<br>";
         }

       return $html;
    }
    
    function renderWindow(&$appMan)
    {
        if ($appMan->getAuth()->isUserValidFor("mixer", "r")) {
            $main = "<table bgcolor=#777777 width=100% cellspacing=5 cellpadding=0 border=0><tr>";

            $numChannels = Mixer::getNumChannels();
            for($i=0;$i<$numChannels;++$i) {
                $main .= $this->MixBar($appMan, Mixer::getChannelID($i));
            }

            $main .= "</tr></table>";
        }

        return $main;
    }


    function img($fn) {
         return "'".$this->getImageURL($fn)."'";
    }

    function getBlankBar($bar)
    {
        return "<IMG SRC=".$this->img("blankbar.jpg")." WIDTH=32 HEIGHT=".(200-$bar)." ISMAP BORDER=0><BR>";
    }

    function getSolidBar($bar)
    {
        return "<IMG SRC=".$this->img("mixerbar.jpg")." WIDTH=32 HEIGHT=$bar ALT=$bar ISMAP BORDER=0><BR>";
    }


    function MixBar(&$appMan, $channel) {
        $vol = Mixer::getVolume($channel);
        $name = Mixer::getName($channel);
        $prop = $channel;

        $webface = $appMan->webface;
        $webauth = $appMan->getAuth();

        // Replace spaces with non-breaking ones
        $name = str_replace(" ", "&nbsp;", $name);
        $name = "<font size=1>$name</font>";

        $bar = $vol*2;
        
        $h.= "<TD WIDTH=10% ALIGN=RIGHT><DIV ALIGN=TOP>";
        $h.= "<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0 WIDTH=100%>";

        $h.= "<TR><TD COLSPAN=3 ALIGN=CENTER>$name</TD></TR>";
        $h.= "<TR><TD ALIGN=RIGHT><img src=".$this->img("scale.gif")."></TD><TD>";

        if ($bar != 200) {
            if ($webauth->isUserValidFor("mixer", "w"))
            {
                $h.= $webface->ctrl->createAnchor("slider","${prop}1", $this->getBlankBar($bar));
            } else {
                $h.= $this->getBlankBar($bar);
            }
        }

        if ($bar) {
            if ($webauth->isUserValidFor("mixer", "w"))
            {
                $h.= $webface->ctrl->createAnchor("slider","${prop}2", $this->getSolidBar($bar));
            } else {
                $h.= $this->getSolidBar($bar);
            }
        }

        $h .= "</TD>";

        $h.= "<TD ALIGN=RIGHT><img src=".$this->img("mixspacer.gif")."</TD>";
        $h.= "</TR>";
        $h.= "<TR><TD ALIGN=CENTER COLSPAN=3>";

        if ($webauth->isUserValidFor("mixer", "w"))
        {
            $h.= $webface->ctrl->createAnchor("dec", "${prop}", "<img alt=Down border=0 src=".$this->getImageURL("down.gif").">");
            $h.= $webface->ctrl->createAnchor("inc", "${prop}", "<img alt=Up border=0 src=".$this->getImageURL("up.gif").">");
        }

        $h .= "<br>$vol%</TD>";
        $h.="</TR></TABLE>";

        return $h;
    }

    function renderInfo(&$app_man, $fast=false)
    {
        if ($fast) {
            return "-";
        }

        return <<<EOD
<p>This provides full control over the volume of each channel (pcm, wave,
line in,etc) of the soundcards mixer. It uses <i>aumix</i> under the hood.
</p>
<p><i>Dependencies: </i>aumix</p>
<p><i>Future Plans: </i>AJAX controls would be nice!<br/></p>

EOD;
    }

}
?>

