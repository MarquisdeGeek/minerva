<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';
require_once 'topprocess.inc';


class Warp_Top_Applet extends Warp_Applet
{
    var $_sort;
    
    function Warp_Top_Applet($caption = "Top")
    {
        parent::__construct($caption);
    }

    function getDirectory()
    {
        return "top";
    }

    function &getTopStats()
    {
        $top = new Top();
        $top->addForbiddenUser("root");
        $top->poll();
        $top->sort = $this->_sort;
        return $top;
    }
    
    function init(&$appMan)
    {
        // get default state, if present. (just a sort order)
        $this->_sort = $appMan->queryParameter($this, "sort");
        if ($this->_sort == "") {
            $this->_sort = 80;
        }
        
        // parse sort argument, if present
        $arg = $appMan->queryParameter($this, "do");
        if ($arg != "") {
            $this->_sort = $arg;
        }
        
        return true;
    }
    
    function renderPanel(&$appMan)
    {
        $top = $this->getTopStats();
        
        $html.= "<font size=-1>";
        $html.= "<b>Load:</b> ".$top->load[1].",".$top->load[2].",".$top->load[3]."<br>";
        $html.= "<b>Tasks:</b> ".$top->tasks[1]." s".$top->tasks[2]." r".$top->tasks[3]." z".$top->tasks[4]." i".$top->tasks[5]."<br>";
        $html.= "<b>CPU:</b> u".$top->cpu[2]." s".$top->cpu[3]." n".$top->cpu[4]." i".$top->cpu[5]."<br>";
        $html.= "<b>Mem:</b> u".$top->mem[2]." f".$top->mem[3]."<br>";
        $html.= "<b>Swap:</b> u".$top->swap[2]." f".$top->swap[3]."<br>";
        $html.= "</font>";

        return $html;
    }
    
    function renderWindow(&$appMan)
    {
        if (!$appMan->getAuth()->isUserValidFor("system", "r")) {
           return "Not available";
        }

        $top = $this->getTopStats();
        
        $top->sortBy($this->_sort);
        
        $col = array("PID","User","Pri","Ni","Size","Rss","Share","Stat","%CPU","%Mem","Time","Command");
        
        $html = "<table width=100% cellspacing=5><tr valign=center bgcolor=#dddddd>";
        
        $_sort = 0;
        foreach($col as $c)
        {
            $html.="<td><b>$c</b><br>";
            $html.= $appMan->getAppletLink($this, "do", $_sort, $appMan->getStyleText("down"));
            $html.= $appMan->getAppletLink($this, "do", ($_sort+1), $appMan->getStyleText("up"));
            $html.="</td>";
            
            $_sort += 10;
        }
        $html.="</tr>";
        
        foreach($top->processes as $p)
        {
            $html.="<tr>";
            $html.= "<td>$p->pid</td>";
            $html.= "<td>$p->user</td>";
            $html.= "<td>$p->pri</td>";
            $html.= "<td>$p->ni</td>";
            $html.= "<td>$p->size</td>";
            $html.= "<td>$p->rss</td>";
            $html.= "<td>$p->share</td>";
            $html.= "<td>$p->stat</td>";
            $html.= "<td>$p->cpu</td>";
            $html.= "<td>$p->mem</td>";
            $html.= "<td>$p->time</td>";
            $html.= "<td>$p->cmd</td>";
            $html.="</tr>";
        }
        $html.="</table>";
        return $html;
    }
    
    function getRefreshParams(&$appMan)
    {
        return $appMan->getArgument($this, "sort", $this->_sort);
    }
    
}

?>

