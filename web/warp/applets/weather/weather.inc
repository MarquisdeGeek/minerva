<?php

require_once 'warp/warplib/applet.inc';
require_once 'warp/conf/weather/weather.conf';

class WeatherInfo
{
var $day;
var $high;
var $low;
var $description;
var $image;

var $applet;
var $dayRequest;

   function WeatherInfo(&$a, $day) 
   {
      $this->applet = $a;
		$this->dayRequest = $day;
   
		$cmd = "/usr/local/minerva/bin/weatherinfo $this->dayRequest ";
      $this->day = exec("$cmd day");
		$this->high = exec("$cmd high");
		$this->low = exec("$cmd low");
		$this->description = exec("$cmd description");
		$this->image = exec("$cmd image");
   }

    function img($fn) {
         return "<img src='".$this->applet->getImageURL($fn)."' border=0/>";
    }

   function getHtml() 
   {
      $img = $this->img("$this->image.png");

      $html = "<table border=1>";
      $html .= "<tr><td colspan=3 align=center><h1>".$this->day."</h1></td></tr>";
      $html .= "<tr><td rowspan=2>$img<br/>".$this->description."</td>";
      $html .= "<td align=left>High:<b>".$this->high."</b><br/><br/>Low:<b>".$this->low."</b></td>";
      $html .= "</tr>";
      $html .= "<tr><td></td></tr>"; // put image here
      $html .= "</table>";
      return $html;
   }
}


class Warp_Weather_Applet extends Warp_Applet
{
    function Warp_Weather_Applet($caption = "Weather")
    {
        parent::__construct($caption);
    }

    function getDirectory()
    {
        return "weather";
    }

    function renderWindow(&$app_man)
    {
		$data = file(WeatherReader::getInfoFileName());
		$html = "<table align=center><tr>";
		$info = new WeatherInfo($this, "today");
		$html .= "<td>".$info->getHtml()."</td>";
		$info = new WeatherInfo($this, "tomorrow");
		$html .= "<td>".$info->getHtml()."</td>";
		$html .= "</tr></table>";
		return $html;
    }
    
    function renderInfo(&$app_man, $fast=false)
    {
        if ($fast) {
            return "-";
        }
        
return <<<EOD
<p>In combination with a daily crontab job (run from <tt>~minerva/update/weather</tt>, this presents the weather in a simple form.</p>

<p><i>Graphics are by <a href="http://jonatan7.deviantart.com/art/Weather-images-4-93409563">jonatan7</a>. <br/></p>

EOD;

    }
    
}

?>

