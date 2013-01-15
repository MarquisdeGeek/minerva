<?php
require_once "request.inc";
require_once "huxley/huxley.inc";

$cmd = $_GET[cmd];
$readCommand = ($cmd=="status" || $cmd=="stations" || $cmd=="enumdev");
$writeCommand = !$readCommand;

$request = new MinervaAPIRequest("radio");
$readCommand &= $request->accessRead;
$writeCommand &= $request->accessWrite;

$processor = getProcessor($_GET['fmt']);

if ($readCommand) {

	$node = $processor->createRootNode("radio", "", true);

	if ($cmd == "status") {
		exec("$minbin/fmradio default get", $output);
		$n = $processor->createNode("frequency");
		$n->addContent($output[0]);
		$node->addChild($n);

	} else if ($cmd == "enumdev") {
		$request->enumerateDevices($processor, "fmradio");

	} else if ($cmd == "stations") {

		exec("/usr/local/minerva/bin/fmradio default getstationlist", $output);
		foreach($output as $station) {
			preg_match("/^(.*?)\s+(.*?)\s(.*)$/", $station, $stationInfo);

			$n = $processor->createNode("station");
			$n->addParam("id", $stationInfo[1]);
			$n->addParam("frequency", $stationInfo[2]);
			$n->addContent($stationInfo[3]);
			
			$node->addChild($n);
		}
	}

} else if ($writeCommand) {
	$freq = $_GET['freq'];
	exec("/usr/local/minerva/bin/fmradio default set $freq");
	$node = $processor->createRootNode("radio", "", true);
	
} else {
	$dpsLastError = "Invalid permissions";
	$node = $processor->createRootNode("radio", "", false);
}

echo $node->generate();
?>

