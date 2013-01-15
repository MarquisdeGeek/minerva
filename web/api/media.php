<?php
require_once "request.inc";
require_once "huxley/huxley.inc";

$cmd = $_GET[cmd];
switch($cmd) {
        case "mp3player":
        case "manifest":
                $conduit = "mp3jukebox";
                break;
        default:
                $conduit = "system";
}

$request = new MinervaAPIRequest($conduit);

$cmd = $_GET[cmd];

if ($request->accessWrite) {
	$output = array();
	switch($cmd) {
		case "manifest":
			$type = $request->getParam("type", "music");
			$items = $request->getParam("items", "5");
			$control = $request->getParam("control", "play");

			if ($control == "play") {
				shell_exec($request->cmdBase." start $type $items >/dev/null &");
			} else if ($control == "next") {
				exec($request->cmdBase." next", $output);
			}
			break;

		case "mp3player":
			break;
	}

  	$data = implode($output, "<br>\n");
  	print $data;
}

?>

