<?php
require_once "/usr/local/minerva/bin/inc/utils.inc";

class appletUtils {

   public static function getContents($url, $cachename, $maxAge) {
		return getContents($url, $cachename, $maxAge);
   }

   public static function getParentDirectory($path) {
       if ($path == "" || $path == NULL) {
           return "/";
       }

       $parent = strrpos($path, '/');
       if ($parent == strlen($path)-1) {
          $path = substr($path, 0, $parent);
          $parent = strrpos($path, '/');
       }
     
       if ($parent == NULL) {
          return "/";
       }

       $parent = substr($path, 0, $parent);
       if ($parent == "") {
          $parent = "/";
       }
       return $parent;
    }

    public static function validatePath($path) {
       // we essentially just remove . or .. paths present
       $path = preg_replace("!/\.+!", "/", $path);
       $path = preg_replace("!\.+/!", "/", $path);

       // and then join any double slashes
       $path = preg_replace("!/+!", "/", $path);
       return $path;
    }

   public static function isTextFile($fn) {
      if (is_file($fn) && stristr($fn,".txt") == ".txt") {
         return true;
      }
      return false; 
   }

   public static function isImageFile($fn) {
      if (is_file($fn) && stristr($fn,".png") != "") {
         return true;
      }
      return false; 
   }

   public static function getPagerSelect(&$appMan, $applet, $num, $entries, $pagesize, $pagerArg="page") {

      $total = (int)($entries/$pagesize);
      if ($total == 0) {
         return "";
      }
      if ($total * $pagesize != $entries) {
         ++$total;  // there's a few extra on the last page
      }

      for($page=0;$page<$total;++$page) {
         if ($num == $page) {
            $bar .= "<b>".($page+1)."</b>"; 
         } else {
            $bar .= $appMan->getAppletLink($applet, $pagerArg, $page, $page+1);
         }
         $bar .= " ";
      }
      $prev = max($num-1, 0);
      $next = min($num+1, $total-1);

      $bar .= "&nbsp;&nbsp;";
      if ($num == 0) {
         $bar .= "&lt;&lt;";
      } else {
         $bar .= $appMan->getAppletLink($applet, $pagerArg, $prev, "&lt;&lt;");
      }
      $bar .= "&nbsp;";
      if ($num == $total-1) {
         $bar .= "&gt;&gt;";
      } else {
         $bar .= $appMan->getAppletLink($applet, $pagerArg, $next, "&gt;&gt;");
      }

      return $bar;
   }
 
   public static function getWrappedIndex(&$appMan, $applet, $currentArgument, $low, $highPlusOne) {

        $current = $appMan->queryParameter($applet, $currentArgument);

        if ($current == "") {
            $current = $low;
        }

        if ($appMan->queryParameter($applet, "cmd") == "next") {
            ++$current;
        } else if ($appMan->queryParameter($applet, "cmd") == "prev") {
            --$current;
        }

        $forceIndex = $appMan->queryParameter($applet, "setidx");
        if ($forceIndex != "") {
		$current = $forceIndex;
	}

        if ($current < $low) {
            $current += ($highPlusOne-$low);
        } else if ($current >= $highPlusOne) {
            $current -= ($highPlusOne-$low);
        }

    return $current;
  }

  public static function getGridedContent($entries, $columns) {
     if (count($entries) == 0) {
        return "";
     }
     if ($columns <= 0) {
        $columns = count($entries);
     }

     $rows = Floor(count($entries) / $columns);
     $grid = "<table border=1 cellspacing=2>";

     for($i=0;$i<count($entries);) {
        if (($i % $columns) == 0) {
            $grid .= "<tr>";
        }
        $grid .= "<td>".$entries[$i]."</td>";
        ++$i;
        if (($i % $columns) == 0) {
           $grid .= "</tr>";
        }
     }

     if ($columns*$rows != count($entries)) {
         for(;$i<=count($entries);++$i) {
             $grid .= "<td></td>";
         }
         $grid .= "</tr>";
     }

     $grid .= "</table>";
 
     return $grid; 
  }

private static $uid="0";
  public static function getAnimatedText($text, $length) {

      if (strlen($text) <= $length) {
          return $text;
      }
      $uid = self::$uid++;

      return <<<EOD
<div id="gat$uid">...</div>
<script language="javascript">
<!--
var gattext$uid = "$text...";
var curr$uid = 0;
var delay$uid = -1;
var timer$uid = 500;
var maxDelay$uid = 10;

function cbAnimText$uid() {
 
   if (delay$uid == 0) {	// no delay, just scroll
       do {
           if (++curr$uid > gattext$uid.length) { 
               curr$uid = 0;
               delay$uid = 1;
               break;
           }
           end = curr$uid + $length;
           if (end >= gattext$uid.length) {
              end -= gattext$uid.length;
           }
           lastLetter = gattext$uid.charAt(end-1);
            
       } while(lastLetter == ' ');
 
   } else if (delay$uid == -1) {  // self-initialize
      delay$uid = 1;

   } else {
       if (++delay$uid >= maxDelay$uid) {
          delay$uid = 0;
          curr$uid = 0;
       } else {
          setTimeout('cbAnimText$uid()',timer$uid)
          return;
       }
   }

   end = curr$uid + $length;
   if (end < gattext$uid.length) {
        document.getElementById('gat$uid').innerHTML = gattext$uid.substring(curr$uid, end-1);

   } else { // overruns :(
       document.getElementById('gat$uid').innerHTML = gattext$uid.substring(curr$uid) + gattext$uid.substring(0,(end-gattext$uid.length)-1);
   }

   setTimeout('cbAnimText$uid()',timer$uid)
}

cbAnimText$uid();
//-->
</script>
EOD;
  }

  public static $ANIMTIME_STYLE_TIME_OF_TOTAL = 1;
  public static $ANIMTIME_STYLE_TIME_ONLY = 2;

  public static function getAnimatedTime($curr, $total, $style) {
      $uid = self::$uid++;

      return <<<EOD
<div id="gam$uid">...</div>
<script language="javascript">
<!--
var gamtime$uid = $curr;
var gamtotal$uid = $total;
var gamstyle$uid = $style;

function cbAnimTime$uid() {

   t = gamtime$uid;
   secs = t % 60; t=(t-secs)/60; if (secs < 10) secs = "0"+secs;
   mins = t % 60; t=(t-mins)/60; if (mins < 10) mins = "0"+mins;
   txt = t+":"+mins+"."+secs;

   if (gamstyle$uid == 1) {
      t = gamtotal$uid;
      secs = t % 60; t=(t-secs)/60; if (secs < 10) secs = "0"+secs;
      mins = t % 60; t=(t-mins)/60; if (mins < 10) mins = "0"+mins;
      txt += " / ";
      txt += t+":"+mins+"."+secs;
   }

   document.getElementById('gam$uid').innerHTML = txt;

   if (gamtime$uid < gamtotal$uid) {
     ++gamtime$uid;
     setTimeout('cbAnimTime$uid()', 1000);
   }

}

cbAnimTime$uid();
//-->
</script>
EOD;
   }

}

?>

