<?php
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';

class Warp_RSS_Applet extends Warp_Applet
{
var $cmd;
var $head, $body;

    function Warp_RSS_Applet($user, $id) 
    {
        $this->cmd = "/usr/local/minerva/bin/news-read ".$id." ".$user;
        $caption = exec($this->cmd." txt 0 name");
        parent::__construct($caption);
    }

    function getDirectory()
    {
      return "rssreader";
    }

    function getIconURL()
    {
       return "rss.gif";
    }

    function renderWindow(&$appMan)
    {
       $html = "";
       $html.="<ul>";
       $maxItems = 9;
       for($i=0;$i<$maxItems;++$i) {
          $title = exec($this->cmd." html $i headline");
          $url = exec($this->cmd." html $i url");
          $story = exec($this->cmd." html $i story");
          if ($title == "") {
             break;
          }

          $html.="<a name=$i>";
          $html.="<li>";
          #$html.="<a href=$url>$title</a>";
          $html.='<a href="javascript:flipelement('.$i.');">'.$title."</a><br/>";
          $html.="<p id=$i style=display:none>$story</p>";
          $html.="<a href=$url>More...</a>";
          $html.="<p>";
          $html.="</div>";
          $html.="</li>";
       }
       $html.="</ul>";

       return $html;
    }

    function getHeadHTML(&$appMan) 
    {
       return <<<END
<script language="JavaScript">
<!--
function flipelement(id) {
if (document.getElementById(id).style.display == 'block') {
document.getElementById(id).style.display = 'none';
} else {
document.getElementById(id).style.display = 'block';
}
}
//-->
</script>
END;
}

}


