<?php
ini_set('include_path', '/var/www/minerva/:'.ini_get('include_path'));

require_once 'warp/warplib/applet.inc';
require_once 'warp/applets/calendar/calendar.inc';

           $gcals = Warp_GoogleCalendar_Config::getICalList();
           $c = new Warp_Calendar_Applet();
           foreach($gcals AS $name => $url) {
              $total .= $c->getTodaysEvents($name, $url);
           }
           if ($total == "") {
             $total = "There are no events today";
           }
           print $total;

?>

