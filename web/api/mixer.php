<?php
require_once "request.inc";
require_once "huxley/huxley.inc";

$cmd = $_GET[cmd];
$readCommand = ($cmd=="status" || $cmd=="enumdev");
$writeCommand = !$readCommand;

$request = new MinervaAPIRequest("mixer");
$readCommand &= $request->accessRead;
$writeCommand &= $request->accessWrite;

$mixbin = "$minbin/mixer $request->device ";
$processor = getProcessor($_GET['fmt']);

$report = array("wave"=>"Wave audio", "speaker"=>"Speaker", "cd"=>"CD Player", "linein1"=>"Line in (1)", "linein2"=>"Line in (2)", "linein3"=>"Line in (3)");

if ($readCommand) {

	switch($cmd) {
      case "enumdev":
			$node = $processor->createRootNode("mixer", "", true);
         $request->enumerateDevices($processor, "mixer");
         break;

		case "status":
			$node = $processor->createRootNode("mixer", "", true);
			foreach($report as $mixdevice => $display) {
				$out = array();
				exec("$mixbin get $mixdevice", $out);
				$n = $processor->createNode("vol");
				$n->addParam("channel", $mixdevice);
				$n->addParam("display", $display);
				$n->addParam("value", $out[0]);
				$node->addChild($n);
			}
			break;
	}
} else if ($writeCommand) {
	$success = false;
	$channel = $request->getParam("channel", "");
	$volume = $request->getParam("volume", "100");

	foreach($report as $mixdevice => $display) {
		if ($mixdevice == $channel) {
			exec("$mixbin set $channel $volume");
			$success = true;
		}
	}
	$node = $processor->createRootNode("mixer", "", $success);

} else {
	$dpsLastError = "Invalid permissions";
	$node = $processor->createRootNode("mixer", "", false);
}

echo $node->generate();
?>

