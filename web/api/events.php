<?php
require_once "request.inc";
require_once "huxley/huxley.inc";

$request = new MinervaAPIRequest("system");
$processor = getProcessor($_GET['fmt']);
$node = $processor->createRootNode("events", "", $request->accessRead);


if ($request->accessRead) {
	$output = array();
	$cmd = $_GET[cmd];
	switch($cmd) {
		case "week":
			$days = 7;
			break;
		case "today":
			$days = 1;
			break;
	}

	exec("$minbin/getcalendar ".$request->user." $days", $output);

  	$data = implode($output, "<br/>\n");

	$n = $processor->createNode($cmd);
	$n->addContent("result", $data);
	$node->addChild($n);
}


echo $node->generate();

?>

