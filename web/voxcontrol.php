<?php

$cmd = $HTTP_GET_VARS['cmd'];
$auth = $HTTP_GET_VARS['auth'];

if ($auth == "") {
  $auth = "public";
}

system("/usr/local/minerva/bin/msgrcv vox $auth $cmd &");

?>

