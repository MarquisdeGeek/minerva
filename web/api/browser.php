<?php
require_once "request.inc";
require_once "huxley/huxley.inc";
require_once "browser.inc";

$cmd = $_GET[cmd];

$browser = new APIBrowser("browser", $_GET[root]);
$browser->processReadCommand($cmd);
echo $browser->generate();

?>

