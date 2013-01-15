<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';
require_once 'warp/applets/static/static.inc';

class Warp_LiveDepartureBoards_Applet extends Warp_Applet
{
var $route;

    function Warp_LiveDepartureBoards_Applet($caption = "Live Departure Boards")
    {
        parent::__construct($caption);
    }

    function getDirectory()
    {
        return "ldb";
    }

    function init(&$appMan)
    {
        Warp_Applet::Init($appMan);

        $this->route = $appMan->queryParameter($this, "route", "1");

        return true;
    }
    
    function renderWindow(&$appMan)
    {
        if ($this->route == "1") {
            $html .= "<h1>From St Pancras to Luton</h1>";
            $altroute = $appMan->getAppletLink($this, "route", "0", "To London");
            $codes = "STP LUT";
            $url = "http://realtime.nationalrail.co.uk/ldb/sumdep.aspx?T=STP&S=LUT";
        } else {
            $html .= "<h1>From Luton to St Pancras</h1>";
            $altroute = $appMan->getAppletLink($this, "route", "1", "Trains home");
            $codes = "LUT STP";
            $url = "http://realtime.nationalrail.co.uk/ldb/sumdep.aspx?T=LUT&S=STP";
        }
        $path = getMinervaRoot();
        exec("$path/bin/ldbquery gettimes $codes", $results);
        if (count($results) == 0) {
           $html .= "There are no more trains this evening!";
        } else {
            $html .= "<ul>";
            foreach($results AS $line) {
                $html .= "<li>$line</li>";
            }
            $html .= "</ul>";
        }
        $html .= $altroute;
        $html .= "<br/>";
        $html .= "<a href=\"$url\">Main LDB page</a>";
        return $html;
    }

    function renderInfo(&$app_man, $fast=false)
    {
        return "";
    }

}

?>

