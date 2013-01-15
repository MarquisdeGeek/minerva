<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletmanager.inc';
require_once 'warp/warplib/appletutils.inc';
require_once 'warp/conf/photoframe/photoframe.conf';

class Warp_PhotoFrame_Applet extends Warp_Applet
{
var $_current;
var $_photoPath;
var $_photo_list;

    function Warp_PhotoFrame_Applet($caption = "Photos")
    {
        parent::__construct($caption);
    }

    function init(&$appMan) 
    {
        $this->_photo_list = get_photo_list();
        $this->_current = appletUtils::getWrappedIndex($appMan, $this, "current", 0, count($this->_photo_list));
    }

    function getDirectory()
    {
        return "photoframe";
    }

    function getPhotoImage()
    {
        return $this->getConfFileName("photos/".$this->_photo_list[$this->_current]);
    }

    function getPhotoName()
    {
        return $this->_photo_list[$this->_current];
    }

    function getPhotoCaption()
    {
        return "";
    }

    function getRefreshParams(&$appMan)
    {
        return $appMan->getArgument($this, "current", $this->_current);
    }

    function renderPanel(&$appMan)
    {
        if (!$appMan->getAuth()->isUserValidFor("photos", "r")) {
            return "Unavailable";
        }

        $img = '<img width=64 src="'.$this->getPhotoImage().'" border=0>';
        if ($appMan->getMaximisedAppletID() == $this->getID($appMan)) {
            $thumb = $appMan->getAppletLink($this, "fullscreen","true",$img);
        } else {
            $thumb  = $appMan->getAppletMaximiseLinkURL($this, $img);
        }           
        $thumb .= $appMan->getAppletLink($this, "cmd", "prev", $appMan->getStyleText("prev"));
        $thumb .= $appMan->getAppletLink($this, "cmd", "next", $appMan->getStyleText("next"));
        return $thumb;
    }

    function renderWindow(&$appMan)
    {
        if (!$appMan->getAuth()->isUserValidFor("photos", "r")) {
            return "";
        }

        $img = '<img width=600 src="'.$this->getPhotoImage().'" border=0>';
        $full  = $appMan->getAppletLink($this, "fullscreen","true",$img);
        $full .= $this->getPhotoCaption();
        $full .= $appMan->getAppletLink($this, "cmd", "prev", $appMan->getStyleText("prev"));
        $full .= $appMan->getAppletLink($this, "cmd", "next", $appMan->getStyleText("next"));

        $photo_img = $this->getPhotoImage();
        list($width, $height, $type, $attr) = getimagesize(WarpSetup::getPathRoot().$this->getPhotoImage());
        $info = "<b>".$this->getPhotoName()."</b><br><br/>";
        $info.= "<i>Dimensions</i> : $width x $height<br>";
        $info.= "<i>Type</i> : $type<br>";
        $info.= "<i>Attributes</i> : $attr<br>";

        return $appMan->webface->view->split2V($full, $info);
    }

    function renderPage(&$app_man)
    {
        $_fullscreen =  $app_man->queryParameter($this, "fullscreen");
        if (!$_fullscreen) {
             parent::renderPage($app_man);
             return;
        }

        $photo_img = $this->getPhotoImage();
        $photo_caption = $this->getPhotoCaption();
        $photo_title = "";
        $photo_main_url = $app_man->getBaseURL();

        $fs = $app_man->getArgument($this, "fullscreen", "true");
        $photo_next = $app_man->getAppletLinkURL($this, "cmd", "next")."&$fs";
        $photo_prev = $app_man->getAppletLinkURL($this, "cmd", "prev")."&$fs";

        list($width, $height, $type, $attr) = getimagesize($this->getPhotoImage());

echo <<< HERE

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<html>
<head>

<meta http-equiv="content-type" content="text/html; charset=ISO-8859-1" />

<title>$photo_title</title>

<style>
.overall {
    position : relative;
    left : 0;
    top : 0;
}
.overlay {
    position : absolute;
    left : 3;
    top : 4;
 text-shadow: red 5px 0px 5px;
 }
</style>


<script type="text/javascript">

function setVal()
{
   win = new Array ("w","h");
   avail = new Array ("w","h");

   if (window.innerWidth)
   {
      win["w"] = window.innerWidth;
      win["h"] = window.innerHeight;
   }

   else if (document.body.clientWidth)
   {
      win["w"] = document.body.clientWidth;
      win["h"] = document.body.clientHeight;
   }

   else
   {
      win["w"] = screen.width - 25;
      win["h"] = screen.height - 115;
   }


   avail["w"] = win["w"];// - obso["w"];
   avail["h"] = win["h"];// - obso["h"];
	 }

function sizeImg(imgW, imgH)
{
   imgRatio = imgW / imgH;

   function scaleWB() { tWidth = avail["w"]; tHeight = tWidth / imgRatio; }
   function scaleBW() { tHeight = avail["h"]; tWidth = tHeight * imgRatio; }

   if (imgRatio >= 1)        // determination of oriontation of image
   {
      scaleWB();
      if (tHeight > avail["h"]) { scaleBW(); }

   }
   else
   {
      scaleBW();
      if (tWidth > avail["w"]) { scaleWB(); }
   }
   // A small white border. This stops some browsers placing the scroll bars
   // on the image as the numbers are very close/overlap
   //tWidth -= 8;
   //tHeight -= 8;
}

window.focus();
</script>
</head>
<body id="slide" marginwidth="0" marginheight="0" bgcolor="black">
<!-- Image with link to next -->
<table width="100%" height="100%" cellspacing=0 cellpadding=0><tr><td align="center">

<div class="overall">
<script type="text/javascript">
<!--
setVal();
sizeImg($width,$height); //note: from server side

document.write('<MAP NAME="slidemap">');
document.write('<AREA SHAPE="Rect" HREF="$photo_next" COORDS="'+2*tWidth/3 + ',0 '+tWidth+','+tHeight+'">');
document.write('<AREA SHAPE="Rect" HREF="$photo_prev" COORDS="0,0 '+tWidth/3 + ','+tHeight+'">');
document.write('<AREA SHAPE="Rect" HREF="$photo_main_url" COORDS="'+tWidth/3 + ','+2*tHeight/3+' '+2*tWidth/3+','+tHeight+'">');
document.write('</MAP>');
//-->
</script>

<script type="text/javascript">
document.write('<img src="$photo_img" width="' + tWidth + '" height="' + tHeight + '" border="0" alt="" usemap="#slidemap">');
</script>
    <div class="overlay">$photo_caption</div>

</td></tr></table>
</div>
</body>
</html>
HERE;
        }


    function renderInfo(&$app_man, $fast=false)
    {
        if ($fast) {
            return "-";
        }

        return <<<EOD
<p>An applet to display single images into the main window, or full-screen
so it can be used as a screen-saver, or image in a digital photo frame. Clicking
on the main picture will open the picture to occupy the full browser window. You can then click on the left and right of the picture to scroll through them.
Return to the main window by clicking in the bottom central third of the image.
</p>
<p>TIP: Install <i>fullerscreen</i> in Firefox browsers to run this as a
real-life digital photo frame.
<p>
The images must be explicitly specified in the configuration file, as listed
in <tt>warp/conf/photoframe/photoframe.conf</tt>. The files themselves are
then copied in the <tt>photos</tt> subdirectory in the same place.

</p><p>Developers should look to this as an example of how to override the
whole WARP system to take-over the full browser frame.</p>

EOD;
    }

}

?>
