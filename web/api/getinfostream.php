<?php
require_once "request.inc";
require_once "huxley/huxley.inc";

$request = new MinervaAPIRequest("system");
$processor = getProcessor($_GET['fmt']);

if ($request->accessRead) {

	$node = $processor->createRootNode("information", "", true);

	$filter = $_GET[filter];
	if ($filter == "") {
		$filter = "all";
	}

	$gis = "/usr/local/minerva/bin/getinfostream";
	$out = array();
	exec("$gis action $filter", $out);
	exec("$gis status $filter", $out);
	exec("$gis event $filter", $out);

	foreach($out as $line) {
		$content .= "$line\n";
	}

	$node->addContent($content);

} else {
	$dpsLastError = "Invalid permissions";
	$node = $processor->createRootNode("mixer", "", false);
}

echo $node->generate();
?>

