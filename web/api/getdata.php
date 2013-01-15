<?php
require_once "request.inc";
require_once "huxley/huxley.inc";

$request = new MinervaAPIRequest("system");
$processor = getProcessor($_GET['fmt']);

if ($request->accessRead) {

	$source = $_GET[source];
	$reference = $_GET[ref];
	$referenceID = $_GET[id];

	$enum = $_GET[enum];

	$source = str_replace("..", "", $source);
	$reference = str_replace("..", "", $reference);

	if ($referenceID == "") $referenceID = ".";

	$cmd = "/usr/local/minerva/bin/getdata $source $reference \"$referenceID\" xml";
	$out = array();
	exec("$cmd", $out, $result);

	if ($result == 0) {
		$node = $processor->createRootNode("datastream", "", true);

		foreach($out as $line) {
			$content .= "$line\n";
		}
	
		$node->addContent($content);
	} else {
		$dpsLastError = "Invalid arguments to getdata";
		$node = $processor->createRootNode("datastream", "", false);
	}

} else {
	$dpsLastError = "Invalid permissions";
	$node = $processor->createRootNode("mixer", "", false);
}

echo $node->generate();
?>

