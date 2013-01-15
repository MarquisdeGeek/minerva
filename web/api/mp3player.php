<?php
require_once "request.inc";
require_once "huxley/huxley.inc";
require_once "browser.inc";

$cmd = $_GET[cmd];

class MP3Browser extends APIBrowser {
	public function MP3Browser($req, $rootPath) {
		parent::__construct($req, $rootPath);

		$this->mp3Player = "/usr/local/minerva/bin/mp3player";
	}

	public function isCommandValid($cmd) {
		if ($cmd == "getrecent") {
			return true;
		}
		return parent::isCommandValid($cmd);
	}

   public function onProcessDirectoryFile(&$node, $file, $fullFilename) {
		if (strstr($file, "jpg") ||strstr($file, "png") || strstr($file, "gif")) {
			$n = $this->processor->createNode("image");
		} else {
			$n = $this->processor->createNode("file");
		}
      $n->addParam("size", filesize("$fullFilename"));
      $n->addContent($file);
      $node->addChild($n);
   }

   public function onProcessReadCommand($cmd) {
		if ($cmd == "getrecent") {
			$output = array();
			exec("/usr/local/minerva/bin/recentmp3 recent |sort -r | head -n 25", $output);
			foreach($output AS $recentmp3) {
				$n = $this->processor->createNode("file");
				$n->addParam("date", substr($recentmp3,0,19));
				$n->addContent(substr($recentmp3,20));
				$this->node->addChild($n);
			}

		} else if ($cmd == "enumdev") {
			$this->request->enumerateDevices($this->processor, "mp3player");

		} else {
			parent::onProcessReadCommand($cmd);
		}
	}

   public function onProcessWriteCommand($cmd) {
		if ($cmd == "play") {
			$systemCommand = "$this->mp3Player default play ";
			$systemCommand.= "\"$this->fullFilename\"";
			$out = system("($systemCommand 2>&1 >/dev/null) >/dev/null 2>&1  &");
		} else if ($cmd == "stop") {
			exec("$this->mp3Player default stop");
		}
   }
};

$browser = new MP3Browser("mp3jukebox", "mp3dir");
$browser->processReadCommand($cmd);
$browser->processWriteCommand($cmd);
echo $browser->generate();
?>

