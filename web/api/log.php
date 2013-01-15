<?php
require_once "request.inc";
require_once "huxley/huxley.inc";

/*
Typical log files include:

addminervauser  facebook  monexec     pmedia     tvinfo
announce        lstatus   mp3player   say        tweet
cdplayer        manifest  msgconduit  status     videostream
cosmic          minuser   msgxmit     stopmedia  wavplayer
ctimer          mixer     mstatus     todo       x10control
*/

$request = new MinervaAPIRequest("system");
$processor = getProcessor($_GET['fmt']);
$node = $processor->createRootNode("log", "", $request->accessRead);

if ($request->accessRead) {
	$cmd = $_GET[cmd];
	$filter = $_GET[filter];
	$tail = $_GET[tail];
	$maxLength = $_GET[length];

	$tail = $request->getParam("tail", "100");
	$maxLength = $request->getParam("length", "5");
	
	$cmd = str_replace(".","_",$cmd); // stop .. and cheats hacking in
	$file = "/var/log/minerva/bearskin/$cmd";
	$command = "tail -n $tail $file";

	if ($cmd == "mp3player" && $filter == "play") {
		$command .= " | grep \" play \"  ";
	}

	exec("$command | tail -n $maxLength | $minbin/revline", $output);

	$results = $processor->createNode("result");
	foreach($output as $line) {
		$nodeLine = $processor->createNode("line");
   	$nodeLine->addContent($line);
   	$results->addChild($nodeLine);
	}
	$node->addChild($results);
}

echo $node->generate();

?>
