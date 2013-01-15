<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';
require_once 'warp/conf/static/static.conf';
require_once 'recipehelper.conf';

class Warp_Recipes_Applet extends Warp_Static_Text_Applet 
{
    function Warp_Recipes_Applet($caption = "Basic Recipes")
    {
        parent::__construct($caption);
    }

    function getDirectory()
    {
        return "recipes";
    }

    function init(&$appMan)
    {
        Warp_Applet::Init($appMan);

        return true;
    }

    function renderPanel(&$appMan)
    {
        return $html;
    }
   
    function renderWindow(&$appMan)
    {
        $this->bookName = "bk1";
        $ajaxrequest = $this->getPathStub()."/ajaxget.php?book=".$this->bookName;

        $html .= $appMan->getView()->split2V("<div id='recipedata'><h1>Recipe Database</h1></div>", "<div id='recipelst'></div>", 70, 30);
        $html .= <<<EOS
        <script language="Javascript">
           xmlhttpPost("$ajaxrequest", "", "recipelst");
           function readrecipe(num) {
              xmlhttpPost("$ajaxrequest&recipe="+num, "", "recipedata");
           }
        </script>
EOS;
        return $html;
    }
    
    function getRefreshParams(&$appMan)
    {
        return $appMan->getArgument($this, "current", $this->_which);
    }
    
    function renderInfo(&$app_man, $fast=false)
    {
        if ($fast) {
            return "-";
        }

        return <<<EOD
EOD;
    }

}

?>

