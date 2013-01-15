<?php
require_once "request.inc";
require_once "huxley/huxley.inc";

$cmd = $_GET[cmd];
$readCommand = ($cmd=="tracks") || ($cmd=="status" || $cmd == "enumdev");
$writeCommand = !$readCommand;

$request = new MinervaAPIRequest("cdplayer");
$readCommand &= $request->accessRead;
$writeCommand &= $request->accessWrite;


$cdbin = "$minbin/cdplayer $request->device ";
$processor = getProcessor($_GET['fmt']);

if ($readCommand) {
	$node = $processor->createRootNode($cmd, "", true);
	switch($cmd) {

		case "enumdev":
			$request->enumerateDevices($processor, "fmradio");
			break;

		case "tracks":
			//$track = 
			$n = $processor->createNode("track-list");
			$count = shell_exec("$cdbin count");
			for ($i=1;$i<=$count;++$i) {
				$trk = $processor->createNode("track");
				$trk->addParam("idx", $i);
				$trk->addParam("name", shell_exec("$cdbin trackname $i"));
				$n->addChild($trk);
			}
			$node->addChild($n);
			// fall through

		case "status":
			$n = $processor->createNode("status");
			$n->addParam("discname", shell_exec("$cdbin discname"));
			$n->addParam("isplaying", shell_exec("$cdbin isplaying"));
			$n->addParam("current", shell_exec("$cdbin current"));
			$n->addParam("currentname", shell_exec("$cdbin currentname"));
			$n->addParam("isdiscpresent", shell_exec("$cdbin isdiscpresent"));
			$n->addParam("isdraweropen", shell_exec("$cdbin isdraweropen"));
			$node->addChild($n);
			break;
	}

} else if ($writeCommand) {
	$node = $processor->createRootNode($cmd, "", true);
	$play = $request->getParam("play", "1");
	if ($cmd == "play") {
		$r = shell_exec("$cdbin play $play");
	} else if ($cmd == "stop") {
		$r = shell_exec("$cdbin stop");
	}
	$node->addParam("result", $r);
} else {
	$dpsLastError = "Invalid permissions";
	$node = $processor->createRootNode($cmd, "", false);
}

echo $node->generate();

?>

