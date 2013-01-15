<?php

class APIBrowser {
var $node;

	public function APIBrowser($req, $rootPath) {
		$this->request = new MinervaAPIRequest($req);
		$this->processor = getProcessor($_GET['fmt']);

		$relativePath = $this->request->getParam("path", "");

   	$absoluteRoot = exec(escapeshellcmd("/usr/local/minerva/bin/getalias $rootPath"), $null, $successCode);
		if ($successCode == 0) {
			$fullPath = $absoluteRoot."/".$relativePath;
			$fullPath = preg_replace('/\.+/', '', $fullPath);
			$fullPath = preg_replace('/\/+/', '/', $fullPath);

			$this->fullPath = $fullPath;
			$this->fullFilename = "$fullPath/".$this->request->getParam("file","");
			if (!file_exists($this->fullPath)) {
				$this->fullPath = null;
			}
			if (!file_exists($this->fullFilename)) {
				$this->fullFilename = null;
			}
		}
	}

	private function validateFilename(&$filename) {
		if ($filename != null) {
			$filename = str_replace(" ", "\\ ", $filename);
		}
	}
	
	private function makeRootNode() {
		if ($this->node) {
			return;
		}
		$this->node = $this->processor->createRootNode($this->request->conduit, "", true);
	}	

	public function processCommands($cmd) {
		processReadCommand($cmd);
		processWriteCommand($cmd);
	}

	public function isCommandValid($cmd) {
		return ($this->fullPath == null) ? false : true;
	}

	public function processReadCommand($cmd) {
		if (!$this->request->accessRead || !$this->isCommandValid($cmd)) {
			return false;
		}
		$this->makeRootNode();
		$this->onProcessReadCommand($cmd);
	}

	public function processWriteCommand($cmd) {
		if (!$this->request->accessWrite || !$this->isCommandValid($cmd)) {
			return false;
		}
		if ($this->fullFilename == null) {
			return false;
		}
		$this->makeRootNode();
		$this->onProcessWriteCommand($cmd);
	}

	public function onProcessReadCommand($cmd) {
		if ($cmd == "getdir") {
			$this->generateDirectoryListing($this->node);
		}
	}

	public function onProcessWriteCommand($cmd) {
	}

	public function onProcessDirectoryFolder(&$node, $file) {
		$n = $this->processor->createNode("directory");
		$n->addContent($file);
		$node->addChild($n);
	}

	public function onProcessDirectoryFile(&$node, $file, $fullFilename) {
		$n = $this->processor->createNode("file");
		$n->addParam("size", filesize("$fullFilename"));
		$n->addContent($file);
		$node->addChild($n);
	}

	public function generateDirectoryListing(&$node) {
		if ($handle = @opendir($this->fullPath)) {
   		while (false !== ($file = readdir($handle))) {
				$fullFilename = $this->fullPath."/$file";

				if ($file == "." || $file == "..") {
					// NULL
				} else if (is_dir("$fullFilename")) {
					$this->onProcessDirectoryFolder($node, $file);
				} else {
					$this->onProcessDirectoryFile($node, $file, $fullFilename);
				}
			}

  			closedir($handle);

			return true;
		}
		return false;
	}

	public function generate() {
		if ($this->node == null) {
			return $this->processor->createRootNode($this->request->conduit, "", true)->generate();
		} else {
			return $this->node->generate();
		}
	}
};

?>
