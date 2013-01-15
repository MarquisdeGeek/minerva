<?php
require_once "request.inc";
require_once "huxley/huxley.inc";
require_once "browser.inc";

$cmd = $_GET[cmd];

class PhotoBrowser extends APIBrowser {

   public function onProcessReadCommand($cmd) {
		$webRoot = exec("/usr/local/minerva/bin/getalias webroot");
		$webRelative = substr($this->fullFilename, strlen($webRoot));

		if ($cmd == "getdir") {
			parent::onProcessReadCommand($cmd);

		} else  if ($cmd == "geturl") {
			$this->node->addContent($webRelative);

		} else  if ($cmd == "getimage" || $cmd == "getimg") {
			print "<meta http-equiv='REFRESH' content='0;url=\"$webRelative\"'>";
		}
   }
};

$browser = new PhotoBrowser("photos", "photodir");
$browser->processReadCommand($cmd);
echo $browser->generate();
?>

