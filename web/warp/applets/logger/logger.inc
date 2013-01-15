<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';

class Warp_Logger_Line
{
    static $reverse = 1;

    var $text;
    
    function Warp_Logger_Line($text)
    {
        $this->text = $text;
    }
    
    function setReverseSort($r)
    {
        self::$reverse = $r ? 1 : -1;
    }

    function compareColumn1($a, $b)
    {
       return self::compareAlpha($a->text, $b->text);
    }
 
    // This comparison functions are static, so $this doesn't
    // make sense, we therefore use $a->before & $a->after.
    // However, we could equally well use $b->, since it's
    // static.
    static function compareNumeric($a, $b)
    {
        if ($a == $b) {
            return 0;
        }
        $v = ($a < $b) ? -1 : 1;
	return $v * self::$reverse;
    }
    
    static function compareAlpha($a, $b)
    {
        $al = strtolower($a);
        $bl = strtolower($b);
        if ($al == $bl) {
            return 0;
        }
        $v = ($al < $bl) ? -1 : 1;
	return $v * self::$reverse;
    }

    function getEntry($col)
    {
       return ($col == 0) ? $this->text : "";
    }
}

class Warp_Logger_Logger extends Warp_Logger_Line
{
    var $num;

    function Warp_Logger_Logger($a, $b)
    {
       parent::__construct($b);
       $this->num = $a;
    }

    function getEntry($col)
    {
       return ($col == 0) ? $this->num : $this->text;
    }

    function compareColumn1($a, $b)
    {
       return self::compareNumeric($a->num, $b->num);
    }

    function compareColumn2($a, $b)
    {
       return self::compareAlpha($a->text, $b->text);
    }
};

class WLAL_Datestamp extends Warp_Logger_Line
{
var $dateText;
var $cmdText;

   function WLAL_Datestamp($line)
   {
      preg_match("/^(.*?\s.*?)\s(.*?)\s(.*)$/", $line, $matches);
      $this->dateText = $matches[1];
      $this->cmdText = $matches[2];
      $this->text = $matches[3];
   }

    function getEntry($col)
    {
       switch($col) {
         case 0:  return $this->dateText;
         case 1:  return $this->cmdText;
         default: return $this->text;
       }
    }

    function compareColumn1($a, $b)
    {
       return self::compareAlpha($a->dateText, $b->dateText);
    }

    function compareColumn2($a, $b)
    {
       return self::compareAlpha($a->cmdText, $b->cmdText);
    }

    function compareColumn3($a, $b)
    {
       return self::compareAlpha($a->text, $b->text);
    }

}


class Warp_Logger_Applet extends Warp_Applet
{
public static $STYLE_PLAIN = 0;
public static $STYLE_LOGGER = 1;
public static $STYLE_DATESTAMP = 2;

    var $_maxEntries = 20;	// changeable with config
    var $_sort;
    var $_options;
    var $_replace;
    var $_style;
   
    function Warp_Logger_Applet($description, $logFile, $style=1)
    {
        parent::__construct($description);

        $this->_logDescription = $description;
        $this->_logFile = $logFile;
        $this->_style = $style;
    }
 
    function getName()
    {
        return $this->_logDescription;
    }
   

    function getDirectory()
    {
        return "logger";
    }

    function init(&$appMan)
    {
        // pickup any control parameters passed in 
        $max = $appMan->queryParameter($this, "mx");
        if ($max != "" && $max > 0) {
            $this->_maxEntries = $max;
        }

        $opt = $appMan->queryParameter($this, "opt");
        if ($opt != "") {
            $this->_options = $opt;
        }


        if ($_SERVER['REQUEST_METHOD'] == 'POST') {
            // Clear any existing options
            $this->_options = 0;

            // was any config information posted to this page?
            $this->_maxEntries = $_POST["max"];

            if ($_POST["optdot"] != "") { $this->_options += 1; }
            if ($_POST["optund"] != "") { $this->_options += 2; }
            if ($_POST["optmin"] != "") { $this->_options += 4; }

            // Validate
            $this->_maxEntries = max(1, $this->_maxEntries);
        }

        // get default state, if present. (just a sort order)
        $this->_sort = $appMan->queryParameter($this, "sort");
        if ($this->_sort == "") {
            $this->_sort = 10;
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
        return exec("tail -n 1 ".$this->_logFile);
    }

    function processLogFileLine($idx, $line, $regexOptions)
    {
       // Remove odd characters, as requested
       if ($regexOptions != "/[]/") {
          $line = preg_replace($regexOptions, " ", $line);
       }

       // Generate an appropriate line
       switch($this->_style) {
          case self::$STYLE_PLAIN:
              return new Warp_Logger_Line($line);

          case self::$STYLE_LOGGER:
              return new Warp_Logger_Logger($idx,$line);

          case self::$STYLE_DATESTAMP:
              return new WLAL_Datestamp($line);
       }
    }
   
    function getColumns()
    {
       switch($this->_style) {
          case self::$STYLE_PLAIN:
                return $this->getColumnsStdNumbered();
          case self::$STYLE_LOGGER:
                return $this->getColumnsStdLogged();
          case self::$STYLE_DATESTAMP:
                return $this->getColumnsStdDateStamp();
       }
       return array("");
    }

    function getColumnsStdNumbered()
    {
       return array("Information");
    }
 
    function getColumnsStdLogged()
    {
       return array("#", "Line");
    }
 
    function getColumnsStdDateStamp()
    {
       return array("Date", "Info", "Line");
    }
 
    function renderWindow(&$appMan)
    {
        if (!file_exists($this->_logFile)) {
            return "No log file...";
        }
        
        $lines = file($this->_logFile);
        $loglist = array();
        
        // build a regex to remove duff charcaters
        $regex = "/[";
        if ($this->_options & 1) { $regex .= "\."; }
        if ($this->_options & 2) { $regex .= "_"; }
        if ($this->_options & 4) { $regex .= "-"; }
        $regex .= "]/";

        $total = 1;
        foreach($lines as $num => $line)
        {
           $logline = $this->processLogFileLine($total, $line, $regex);
           if ($logline != null)
           {
              $loglist[] = $logline;
              ++$total;
           }
        }
        
        $loglist[0]->setReverseSort(($this->_sort & 1)?true:false);
        
        $columns = $this->getColumns(); 

        // Which column are we sorting?
        $sortParam = "compareColumn";
        $columnIndex = (int)($this->_sort / 10);
        if ($columnIndex < 1 || $columnIndex > count($columns)) {
           $columnIndex = 1;
        }
        $sortParam .= $columnIndex;

        // And sort them!
        usort($loglist, array(get_class($loglist[0]), $sortParam));
       
        $html = "<table width=100% cellspacing=5><tr valign=center bgcolor=#dddddd>";
        $sortID = 10;
        foreach($columns as $col) 
        {
           $html.= "<td><b>$col</b>&nbsp;";
           $html.= $appMan->getAppletLink($this, "do", $sortID, $appMan->getStyleText("down"));
           $html.= $appMan->getAppletLink($this, "do", $sortID+1, $appMan->getStyleText("up"));
           $html.= "</td>";
           $sortID += 10;
        }
        $html.= "</tr>\n";
        
        $numEntries = min($this->_maxEntries, $total-1);
        for($count=0;$count<$numEntries;++$count)
        {
            $log=$loglist[$count];
            $html.="<tr>";
            $colidx = 0;
            foreach($columns as $col) 
            {
               $html.="<td>".$log->getEntry($colidx)."</td>";
               ++$colidx;
            }
            $html.="</tr>\n";
        }
        $html.="</table>";
        
        
        return $html;
    }
    
    function getRefreshParams(&$appMan)
    {
        $refresh  = $appMan->getArgument($this, "sort", $this->_sort);
        $refresh .= "&".$appMan->getArgument($this, "mx", $this->_maxEntries);
        $refresh .= "&".$appMan->getArgument($this, "opt", $this->_options);
        return $refresh;
    }

    function renderConfig(&$appMan)
    {
        //$html  = "<form action=".$appMan->getRefreshURL();
        $html  = "<form action=".$appMan->getMaximiseURL($this->getID($appMan));
        $html .= " method=post enctype=application/x-www-form-urlencoded>";
        $html .= "<br>Visible Entries: <input type=text name=max value=".$this->_maxEntries.">";
        $html .= "<br><br>";
        $html .= "Use spaces instead of...<br>";
        $html .= "&nbsp;dots : <input type=checkbox name=optdot ".($this->_options&1?"checked":"").">";
        $html .= "&nbsp;underscores: <input type=checkbox name=optund ".($this->_options&2?"checked":"").">";
        $html .= "&nbsp;hypens: <input type=checkbox name=optmin ".($this->_options&4?"checked":"").">";

        $html .= "<br><br><INPUT TYPE=submit VALUE=Apply>";
        $html .= "<br></form><br>";
        return $html;
    }

    function renderInfo(&$app_man, $fast=false)
    {
        if ($fast) {
            return "-";
        }

        return <<<EOD
<p>This reads the information in a given file, and lists each line on a
separate row. You can sort on the row index, or the text on the line. Good
for logs, as the name suggests.</p>
<p>The specific log file is passed as an argument when constructing the applet.
e.g.</p>
<pre>
$appman->AddApplet(new Warp_Logger_Applet("X10 Heyu", "/var/log/heyu/heyu.log.ttyS0"));
</pre>
EOD;
    }

}

?>
