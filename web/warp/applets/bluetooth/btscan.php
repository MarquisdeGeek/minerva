<?php
// Date in the past
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
header("Cache-Control: no-cache");

$result = shell_exec("/usr/sbin/hciconfig");
$hciscan = shell_exec("/usr/bin/hcitool scan");
$result.= $hciscan;

if ($hciscan == "") {
   $result.= "Nothing found...";
}

print str_replace("\n", "<br>", $result);
?>

