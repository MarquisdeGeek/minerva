<?php
$minbin="/usr/local/minerva/bin";
$mp3bin="$minbin/mp3player";

$homedir = trim(shell_exec("$minbin/getalias webroot"));
require_once "$homedir/minerva/minerva.conf";

$format = $_GET["curr"];

$track = shell_exec("$mp3bin default current");
$track = str_ireplace(".mp3", "", $track);
$track = str_ireplace(".ogg", "", $track);
$track = str_ireplace(".flac", "", $track);

if ($format == "short") {
   $artist = shell_exec("$mp3bin default artist");
   if ($artist != "") {
      $track.="&nbsp;-&nbsp;$artist";
   }
   print $track;
} else if ($format == "full") {
   if (shell_exec("$mp3bin default isplaying") == 1) {
      $info .= "<hr/>";

      $info .= "<h3>$track</h3>";

      $info .= "<table border=0><tr><td valign=top>";
      $info .= "<i>Artist:</i> ".shell_exec("$mp3bin default artist")."<br/>";
      $info .= "<i>Album:</i> ".shell_exec("$mp3bin default album")."<br/>";
      $info .= "<i>Duration:</i> ".shell_exec("$mp3bin default length")."<br/>";

		$path = shell_exec("$mp3bin default full");
      $info .= "<i>Path:</i> $path<br/>";
		$weburl = getWebRelativePath($path);
		if ($weburl != "") {
			$info .= "<i>Link:</i> <a href='$weburl'>$weburl</a><br/>";
		}
      $info .= "</td><td valign=bottom>";
      $info .= "</td></tr></table>";
   }
   print $info;
}


?>


