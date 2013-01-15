<?php
require_once "request.inc";
require_once "huxley/huxley.inc";

$request = new MinervaAPIRequest("system");

$processor = getProcessor($_GET['fmt']);
$node = $processor->createRootNode("system", "", $request->accessRead);


if ($request->accessRead) {
	$cmd = $_GET[cmd];

   	switch($cmd) {
	case "ping":
		$output = "ack";
		break;

	case "auth":
		$output = $request->report();
		break;

	}

  	$data = str_replace("\n", "<br>\n", $output);

	$n = $processor->createNode($cmd);
	$n->addContent("result", $data);
	$node->addChild($n);
}

echo $node->generate();

?>

