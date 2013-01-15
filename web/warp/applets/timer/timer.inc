<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';
require_once 'warp/conf/static/static.conf';

class Warp_Timer_Applet extends Warp_Applet
{
    function Warp_Timer_Applet($caption = "Timer")
    {
        parent::__construct($caption);
    }

    function getDirectory()
    {
        return "timer";
    }

    function init(&$appMan)
    {
        Warp_Applet::Init($appMan);

        return true;
    }
    
    function renderPanel(&$appMan)
    {
        $html = "<div id='PanelClock'></div>";
        return $html;
    }

    function getHeadHTML(&$app_man)
    {
       $stub = $this->getStdImageURL("numbers");
       return <<< EOS
<script language="JavaScript">
<!--
function UpdateClock()
{
var date = new Date();

   document["t1"].src = "$stub/"+Math.floor(date.getHours()/10)+".png";
   document["t2"].src = "$stub/"+Math.floor(date.getHours()%10)+".png";
   document["t3"].src = "$stub/"+Math.floor(date.getMinutes()/10)+".png";
   document["t4"].src = "$stub/"+Math.floor(date.getMinutes()%10)+".png";
   document["t5"].src = "$stub/"+Math.floor(date.getSeconds()/10)+".png";
   document["t6"].src = "$stub/"+Math.floor(date.getSeconds()%10)+".png";

   var hr = ""+date.getHours();
   var min= ""+date.getMinutes();
   var sec= ""+date.getSeconds();
   
   if (hr.length ==1) hr="0"+hr;
   if (min.length==1) min="0"+min;
   if (sec.length==1) sec="0"+sec;

   document.getElementById("PanelClock").innerHTML = hr+":"+min+":"+sec;

   clock = setTimeout("UpdateClock()", 1000);
}

function StartClock()
{
   clock = setTimeout("UpdateClock()", 500);
}

</script>
EOS;
    }

    function renderAnalogueClock(&$appMan)
    {
       $imageURL = "../SiemensClock-256x256.png";

$clock = <<<END
<applet
code="SiemensClock.class"
codebase="http://oskamp.dyndns.org/SiemensClock/Java/"
archive="SiemensClock.jar"
width="256" height="256">
<param name="type" value="application/x-java-applet;version=1.4">
<param name="scriptable" value="false">
<param name="centerX"    value="128">
<param name="centerY"    value="128">
<param name="handScale"  value="88">
<param name="BGImage" value="$imageURL">
</applet>

END
;
      return $clock;
    }

    function renderKeypad(&$appMan)
    {
        $keypad .= <<<EOS
<script type="text/javascript" src="utils/ajax.js"></script>
<script language="JavaScript">
function keypadPress(value) {
  result=document.getElementById("keypadresult").innerHTML;
  result *= 10;
  result += value;
  document.getElementById("keypadresult").innerHTML = result;
}

function clearKeypadTimer() {
  document.getElementById("keypadresult").innerHTML = "0";
}

function startKeypadTimer() {
  duration=document.getElementById("keypadresult").innerHTML; 
  clearKeypadTimer();

  xmlhttpPostConcat("warp/applets/timer/starttimer.php?d="+duration, "", "output");
}

</script>
EOS;
      $keys = array("7","8","9", "4","5","6", "1","2","3", "0");
      foreach ($keys as $i=>$k) {
        $keys[$i] = "<div onClick='keypadPress($k)'><img src=".$this->getImageURL("key$k.png")."></div>";
      }
      $keys[] = "<div onClick='clearKeypadTimer()'><img src=".$this->getImageURL("keyclear.png")."></div>";
      $keys[] = "<div onClick='startKeypadTimer()'><img src=".$this->getImageURL("keystart.png")."></div>";
      $keypad .= appletUtils::getGridedContent($keys, 3);

      $result = "<b>Duration: </b><table border=1 width=64><tr><td><div id=keypadresult>0</div></td></tr></table>";
      $result.= "<h2><div id=output></div></h2>";
 
      return "<h1>Remind me in...</h1>".$appMan->getView()->split2H($keypad, $result);
    }

    function renderClock()
    {
        $colon = $this->getStdImageURL("symbols/colon.png");
        $html .= "<img name=t1 src=''>";
        $html .= "<img name=t2 src=''>";
        $html .= "<img src='$colon'>";
        $html .= "<img name=t3 src=''>";
        $html .= "<img name=t4 src=''>";
        $html .= "<img src='$colon'>";
        $html .= "<img name=t5 src=''>";
        $html .= "<img name=t6 src=''>";
        return $html;
    }

    function renderWindow(&$appMan)
    {
        $clock1= $this->renderClock();
        $clock2 = $this->renderAnalogueClock($appMan);
        $clock = $appMan->getView()->split2H($clock1, $clock2);

        $keypad = $this->renderKeypad($appMan);
        $html .= $appMan->getView()->split2V($keypad, $clock);

        $html .= <<<EOS
<SCRIPT language="JavaScript">
StartClock();
</script>
EOS;
        return $html;
    }
    
    function renderInfo(&$app_man, $fast=false)
    {
        if ($fast) {
            return "-";
        }

        return "";
    }

}

?>

