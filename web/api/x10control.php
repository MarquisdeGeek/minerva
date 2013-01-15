<?php
require_once "request.inc";
require_once "huxley/huxley.inc";

$cmd = $_GET[cmd];
$readCommand = ($cmd=="codes") || ($cmd=="status");
$writeCommand = !$readCommand;

$request = new MinervaAPIRequest("x10");
$readCommand &= $request->accessRead;
$writeCommand &= $request->accessWrite;


$x10bin = "$minbin/x10control $request->device ";
$processor = getProcessor($_GET['fmt']);

if ($readCommand) {
	$node = $processor->createRootNode($cmd, "", true);
	switch($cmd) {
      case "enumdev":
         $request->enumerateDevices($processor, "x10control");
         break;

		case "codes":
			$n = $processor->createNode("code-list");

			exec("$x10bin getcodelist", $codeList);
			exec("$x10bin getcodetype", $codeType);

			for($i=0;$i<count($codeList);++$i) {
				$codeNode = $processor->createNode("code");
				if ($codeType[$i] == "StdLM" || $codeType[$i] == "StdAM") {
					$codeNode->addParam("name", $codeList[$i]);
					$codeNode->addParam("type", $codeType[$i]);
					$codeNode->addParam("state", chop(shell_exec("$x10bin get $codeList[$i]")));
					$n->addChild($codeNode);
				}
			}
			$node->addChild($n);
			break;

		case "status":
			$n = $processor->createNode("status");
			$n->addContent(shell_exec("$x10bin status"));
			$node->addChild($n);
			break;
	}

} else if ($writeCommand) {
	$node = $processor->createRootNode($cmd, "", true);

	$unit = $request->getParam("unit", "");
	$value = $request->getParam("value", "50");

	exec("$x10bin getcodelist", $codeList);
	foreach($codeList as $code) {
		if ($code == $unit) {
			switch($cmd) {
				case "on":
				case "off":
					shell_exec("$x10bin $cmd $code");
					break;
				case "set":
					shell_exec("$x10bin set $code $value");
					break;
			}
		}
	}

} else {
	$dpsLastError = "Invalid permissions";
	$node = $processor->createRootNode($cmd, "", false);
}

echo $node->generate();

?>

