<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';

require_once 'warp/conf/moonbeam/config.php';

require_once 'structs.php';
require_once 'database.php';

class Warp_Moonbeam_Applet extends Warp_Applet
{
    var $metaNameList;
    var $metaMain;
    var $metaFilter;
   
    function Warp_Moonbeam_Applet($caption = "Moonbeam")
    {
        parent::__construct($caption);
    }

    function getDirectory()
    {
        return "moonbeam";
    }

    function getIconURL() 
    {
       return "mblogo_minerva.png";
    }
    
    function init(&$appMan)
    {
        Warp_Applet::Init($appMan);

	$this->prepareGlobals();
        $this->prepareArguments($appMan);
        $this->executeArguments($appMan);
        return true;
    }
    
    function renderPanel(&$appMan)
    {
        return $html;
    }
   
    function renderMetaList($appMan) {
        $html .= $appMan->getAppletLink($this, "showall", "1", "Show All");
        $html .= "<br/><br/>\n";
        foreach($this->metaNameList as $id=>$m) {
            $html .= $appMan->getAppletLink($this, "dometa", $id, $m)."<br/>";
        }
        return $html;
    }

    function prepareGlobals() 
    {
        $this->metaNameList = getMetaList();
    }

    function prepareArguments($appMan) {

        // if there's no POST message, we want to switch all filters to
        // 'accept'. Otherwise, we use whatever was sent.
        if (count($_POST) == 0) {
            $filterClear = 1;
        } else {
            $filterClear = 0;
        }
        //
        $this->mainMeta = $appMan->queryParameter($this, "meta");
        $cmd = $appMan->queryParameter($this, "dometa");
        if ($cmd != "") {
            $this->mainMeta = $cmd;
        }
        $this->metaFilter = $this->metaNameList;	// copy array of all filters
        foreach($this->metaFilter as &$mf) {	// note: & to get references to array
            $mf = $filterClear;
        }
        //
        foreach($_POST as $id=>$check) {
            if ($check = "on") {
                $this->metaFilter[$id] = 1;
            }
        }
        //
        if ($appMan->queryParameter($this, "showall")) {
            $this->mainMeta = -1;
        }
    }

    function executeArguments($appMan) {

	if ($appMan->queryParameter($this, "addurl")) {
		$meta = $_POST['meta'];
		if ($meta == "") $meta = "unassigned";
 		addUrl(0, $_POST['url'], $_POST['desc'], $_POST['notes'], $meta);

		// refresh globals so the metatag list etc is up-to-date
		// because addUrl can affect it.
		$this->prepareGlobals();
	
		$this->metaFilter = $this->metaNameList;
		foreach($this->metaFilter as &$mf) {
			$mf = 1;
		}

		$ml = parseMetaList($meta);
		$this->mainMeta = getMetaTag($ml[0]);
	}
    }

    function renderMetaOptionsForm($appMan) {
	$html = "<form method=post action=".$appMan->getRefreshURL().">";
	foreach($this->metaNameList as $id=>$m) {
		$c = ($this->metaFilter[$id]==1) ? "checked" : "";
		$html .= "| <input type=checkbox name=$id $c>$m |";
	}
	$html.= "<input type=submit value=Filter>";
	$html.= "</form>\n";
	return $html;
    }

    function renderBookmarkList($appMan) {

	$tagList = array();
	$tagList[] = $this->mainMeta;

	$bmList = getBookmark(0, $this->mainMeta==-1?NULL:$tagList, $this->metaFilter);
	$html="<table border=0 cellborder=0 cellspacing=0 width=100%>";
	$bgcol="#ddddff";
	foreach($bmList as $bm) {
		$row = "<tr bgcolor=$bgcol><td><a href=".$bm->url.">".($bm->description==""?$bm->url:$bm->description)."</a></td>";
		$row.= "<td>".$bm->notes."</td></tr>";
		$row.= "<tr><td></td><td bgcolor=$bgcol align=right>Meta: ";

		foreach($bm->metaList as $id) {
			$row.= $appMan->getAppletLink($this, "dometa",$id, $this->metaNameList[$id])." ";
		}

		$row.= "</td></tr>";
		$row.= "<tr><td colspan=3></td></tr>";

		$html .= $row;
	}
	$html .= "</table>";
	return $html;
    }

    function renderAddBookmark($appMan) {
	$html = "<table border=1 bgcolor=#cccccc align=right>";
	$html.= "<tr><td colspan=2><h2>Add a bookmark...</h2></td></tr>";
	$html.= "<form method=post action=".$appMan->getAppletLinkURL($this, "addurl","432","add").">";
	$html.= "<tr><td>URL</td><td><input type=text name=url size=50 value=http://></td></tr>";
	$html.= "<tr><td>Description</td><td><textarea name=desc rows=2 cols=40></textarea></td></tr>";
	$html.= "<tr><td>Notes</td><td><textarea name=notes rows=2 cols=40></textarea></td></tr>";
	$html.= "<tr><td>Metatags</td><td><input type=text name=meta size=40></td></tr>";
	$html.= "<tr><td></td><td align=right><input type=submit value='Add URL'></td></tr>";
	$html.= "</form>";
	$html.= "</table>";
	return $html;
    }


    function renderWindow(&$appMan)
    {
        $filterPane = $this->renderMetaOptionsForm($appMan);
        $filepane = $this->renderBookmarkList($appMan);
        $dirpane = $this->renderMetaList($appMan);
        $topPane = $appMan->getView()->split2V($dirpane, $filepane);
        $bottomPane = $this->renderAddBookmark($appMan);
        return $filterPane.$appMan->getView()->split2H($topPane, $bottomPane);
    }
 
    function getRefreshParams(&$appMan)
    {
        return $appMan->getArgument($this, "meta", $this->mainMeta);
    }

}

?>

