<?php
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';
require_once 'sunrise.php';
require_once 'clockscript.inc';
require_once 'cal.php';

include_once 'warp/conf/calendar/gcal.conf';

class Warp_Calendar_Applet extends Warp_Applet
{
var $cal;
var $showMonth;
var $showYear;

	function Warp_Calendar_Applet($caption = "Calendar")
	{
            parent::__construct($caption);
	}

        function getID() { return "MIN001A"; }

        function getDirectory()
        {
            return "calendar";
        }

        function init(&$appMan)
        {
            Warp_Applet::Init($appMan);

				$this->cal = new Calendar($appMan, $this);
            $this->showMonth = $appMan->QueryParameter($this, "month");
            $this->showYear  = $appMan->QueryParameter($this, "year");
        }

        function renderPanel(&$app_man)
        {
        $date =  date("l")."<br><b>".date("jS F")."</b><br>".date("Y");
	$date.= "<br/>";
        $date.= "<div id='theClock'>   </div>";
	return "<div align=center>$date</div>";
        }

        function renderWindow(&$app_man)
        {
			$clock = getClockScript();

	$d = getdate(time());
        $month = $this->showMonth;
        $year = $this->showYear;
	if ($month == "") { $month = $d["mon"]; }
	if ($year == "") { $year = $d["year"]; }

        $user = $app_man->getAuth()->getUser();

        $monthView = $this->cal->getMonthViewApp($app_man,$this, $month, $year);
        $monthView.="<hr>";
        $monthView.=$this->getLifeStatus($user);
        $monthView.="<p><hr>";
        $monthView.=$this->getWeeklyEvents($user);
        $monthView.="<p><hr>";
        $monthView.=$this->getGCalLinks($user);

			$info = "<p><hr>";
			$info.= "Sunrise:<b>".getSunRise()."</b><br/>";
			$info.= "Sunset:<b>".getSunSet()."</b>";

			$info = "<p><hr><b>Also today</b><br/>";
			$output = array();

			exec("/usr/local/minerva/bin/hacal culture 0", $output);

			$info .= "<ul>";
			foreach($output AS $entry) {
				$info.= "<li>".substr($entry,7)."</li>";
			}
			$info .= "</ul>";

        $calendar = $app_man->getView()->split2H($monthView, $info);

	return $app_man->getView()->split2V($clock, $calendar);
        }

        function renderConfig(&$app_man, $fast=false)
        {
           if ($fast) { return "-"; }

           return "What sort of configuration are you expecting for a calender? Are you nuts? :)";
        }

        function getLifeStatus($user){
				exec("/usr/local/minerva/bin/hacal life 0", $result);
				return $result[0];
        }

        function getICalEvents($user, $idx) {
			exec("/usr/local/minerva/bin/hacal get $user $idx 0 7", $output);

			$header = "<i>from ".$output[0]."</i><br/>";

			for($i=0;$i<sizeof($output)-3;$i += 4) {
				$all .= $header;
				$all .= "<li>".$output[$i+1].".".$output[$i+3]." (".$output[$i+2].")</li>";
				$header = "";
			} 
         return $all;
        }

        function getWeeklyEvents($user) 
        {
           $icals = Warp_GoogleCalendar_Config::getICalList($user);
           if (sizeof($icals) > 0) {
              $events = "<b>Forthcoming events:</b><br/>";
					for($i=0;$i<sizeof($icals);++$i) {
                 $ev = $this->getICalEvents($user, $i);
						if ($ev != "") {
							$events .= "<ul>$ev</ul>";
						}
              }
           }

           return $events;
         }

        function getGCalLinks($user) 
        {
           $gcals = Warp_GoogleCalendar_Config::getGCalendarList($user);
           if (sizeof($gcals) > 0) {
              $cal .= "<b>View complete calendar:</b><br/>";
              for($i=0;$i<sizeof($gcals);$i+=2) {
                 $cal .= "&nbsp;&nbsp;";
                 $cal .= "<a href={$gcals[$i+1]}>{$gcals[$i]}</a><br/>";
              }
           }
           return $cal;
        }

	function getHeadHTML(&$app_man)
	{
           return <<< HERE
           <LINK rel="stylesheet" href="warp/applets/calendar/calendar.css" type="text/css">

<script language="JavaScript">

<!--

var clockID = 0;

function UpdateClock()
{
	if(clockID) 
	{
		clearTimeout(clockID);
		clockID  = 0;
	}

var date = new Date();
var hr = ""+date.getHours();
var min= ""+date.getMinutes();
var sec= ""+date.getSeconds();

	if (hr.length ==1) hr="0"+hr;
	if (min.length==1) min="0"+min;
	if (sec.length==1) sec="0"+sec;

	document.getElementById('theClock').innerHTML = hr+":"+min+":"+sec;

	clock = setTimeout("UpdateClock()", 1000);
}

function StartClock()
{
   clock = setTimeout("UpdateClock()", 500);
}

function KillClock()
{
	if(clock)
	{
		clearTimeout(clock);
		clock = 0;
	}
}
//-->
</script>
HERE;
	}

	function getBodyParams(&$app_man, &$params)
	{
		$params["onload"][] = "UpdateClock()";
		$params["onunload"][] = "KillClock()";
	}

function getRefreshParams(&$app_man)
{
   if ($this->showMonth != "") {
      $ref.= $app_man->getArgument($this, "month", $this->showMonth);
   }

   if ($this->showYear != "") {
      $ref.= $app_man->getArgument($this, "year", $this->showYear);
   }

   return $ref;
}


    function renderInfo(&$app_man, $fast=false)
    {
        if ($fast) {
            return "-";
        }

        return <<<EOD
<p>A standard applet with a clock and calendar. Events are taken from
Google calendar, by adding the publically accessible URL to <tt>warp/conf/calendar/gcal.conf</tt>. The clock uses an external Java applet, and can be removed
to save time.</p>
EOD;
    }

}

?>

