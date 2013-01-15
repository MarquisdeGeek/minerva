<?php
require_once "request.inc";
require_once "huxley/huxley.inc";

$cmd = $_GET[cmd];
switch($cmd) {
	case "media":
		$conduit = "mp3jukebox";
		break;
	default:
		$conduit = "system";
}

$request = new MinervaAPIRequest($conduit);

$processor = getProcessor($_GET['fmt']);
$node = $processor->createRootNode("status", "", $request->accessRead);

if ($request->accessRead) {
	$filter = $request->getParam("filter", "life,ir,media,weather,net,todo");
	$report = split("\s*,\s*", $filter);

	foreach($report as $cmdNode) {
		$output = array();
		switch($cmdNode) {
			case "life":
				exec("$minbin/lstatus", $output);
				break;

			case "sunrise":
				exec("$minbin/sunrise", $output);
				break;

			case "sunset":
				exec("$minbin/sunset", $output);
				break;

			case "ldbstations":
				exec("$minbin/ldbquery stations", $output);
				break;

			case "ldb":
				$station1 = $request->getParam("station1", "LUT");
				$station2 = $request->getParam("station2", "STP");
				array_push($output, "From $station1 to $station2");
				exec("$minbin/ldbquery get $station1 $station2 30", $output);
				array_push($output, "From $station2 to $station1");
				exec("$minbin/ldbquery get $station2 $station1 30", $output);
				break;

			case "ir":
				exec("$minbin/irinfo", $output);
				break;
	
			case "media":
				exec("$minbin/mstatus ".$request->device." full", $output);
				break;
	
			case "tv":
				exec("$minbin/tvonnow ".$request->device." all", $output);
				break;
	
			case "net":
				exec("$minbin/netstatus", $output);
				break;
	
			case "todo":
				exec("$minbin/todo list ".$request->user, $output);
				break;
	
			case "weather":
				exec("$minbin/weatherstatus", $output);
				break;
	
			// Special variation, because I use it more than mstatus
			case "mp3jukebox":
			$cmdBase = "$minbin/mp3player ".$request->device;
	
			$report = array("current"=>"Current track", "artist"=>"Artist", "album"=>"Album name", "length"=>"Length", "fullname"=>"Full path");

			foreach($report as $arg => $display) {
				$out = array();
				exec("$cmdBase $arg", $out);
				array_push($output, "$display: $out[0]");
			}
			break;
	}

  	$data = implode($output, "<br/>\n");
	$n = $processor->createNode($cmdNode);
	$n->addContent("result", $data);
	$node->addChild($n);
	}

}

echo $node->generate();

?>

