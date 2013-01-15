<?php
require_once "request.inc";
require_once "huxley/huxley.inc";

$cmd = $_GET[cmd];
$readCommand = ($cmd=="status" || $cmd=="enumdev");
$writeCommand = !$readCommand;

$request = new MinervaAPIRequest("tv");
$readCommand &= $request->accessRead;
$writeCommand &= $request->accessWrite;

$tvinfobin = "$minbin/tvinfo $request->device ";
$tvctrlbin = "$minbin/tvcontrol $request->device ";
$processor = getProcessor($_GET['fmt']);

if ($readCommand) {

	switch($cmd) {
      case "enumdev":
         $node = $processor->createRootNode("tv", "", true);
         $request->enumerateDevices($processor, "tvcontrol");
         break;

		case "status":
			$node = $processor->createRootNode("tv", "", true);
			$channels = exec("$tvinfobin channels");
			for($i=0;$i<$channels;++$i) {
				$n = $processor->createNode("channel");
				$id = exec("$tvinfobin getstation $i");
				$n->addParam("id", $id);
				$n->addParam("name", exec("$tvinfobin getname $i"));
				$n->addContent("onnow", shell_exec("$tvinfobin onnow $id")); 
				$node->addChild($n);
			}
			break;
	}
} else if ($writeCommand) {
	$station = $request->getParam("station", 1);
		print("$tvctrlbin station $station");
	if ($station == "off") {
		print ("$tvctrlbin off");
		$success = exec("$tvctrlbin off");
	} else if ($station == "on") {
		$success = exec("$tvctrlbin on");
	} else {
		$success = exec("$tvctrlbin station $station");
	}
	$node = $processor->createRootNode("tv", "", $success);
	
} else {
	$dpsLastError = "Invalid permissions";
	$node = $processor->createRootNode("tv", "", false);
}

echo $node->generate();
?>

