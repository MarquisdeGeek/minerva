<?php

function getAddBookmarkCode($url,$bm = NULL) {
        $html = "<table border=1 bgcolor=#cccccc align=right>";
        $html.= "<tr><td colspan=2><h2>Add a bookmark...</h2></td></tr>";
        $html.= "<form method=post action=\"$url\">";
        $html.= "<tr><td>URL</td><td><input type=text name=url size=50 value=".($bm==NULL?"http://":$bm->url)."></td></tr>";
        $html.= "<tr><td>Description</td><td><textarea name=desc rows=2 cols=40>".($bm==NULL?"":$bm->description)."</textarea></td></tr>";
        $html.= "<tr><td>Notes</td><td><textarea name=notes rows=2 cols=40>".($bm==NULL?"":$bm->notes)."</textarea></td></tr>";
        $html.= "<tr><td>Metatags</td><td><input type=text name=meta size=40>".($bm==NULL?"":$bm->metaList)."</td></tr>";
        $html.= "<tr><td></td><td align=right><input type=submit value='Add URL'
></td></tr>";
        $html.= "</form>";
        $html.= "</table>";
        return $html;
}


?>


