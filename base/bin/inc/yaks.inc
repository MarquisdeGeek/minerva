<?php
$scriptsURL = "/usr/local/minerva/etc/x10/scripts/";
$configURL = "/usr/local/minerva/etc/x10/controls";

class YaksHandler {
var $code;
var $index;

	function getScript() {
		global $scriptsURL;
		return "$scriptsURL/$this->code/$this->index";
	}

	function trace() {
		return ("$this->index => calls ".$this->getScript()."\n");
	}

}

class YaksController
{
var $name;
var $housecode;
var $first_index;
var $count;
var $handlesr;

    function YaksController($name, $code, $first, $num)
    {
		$this->name = $name;
		$this->housecode = $code;
		$this->first_index = $first;
		$this->count = $num;
		$this->handlers = array();

		for($i=0;$i<16;++$i) {	// TODO: check the files
			$this->addHandler();
		}

    }

    function addHandler() {
		$h = new YaksHandler();
		$h->code = $this->name;
		$h->index = $this->first_index+count($this->handlers);
		array_push($this->handlers, $h);
	}

	function getHandler($unit, $param) {	# unit is the X10 code, which counts from 1
		$index = ($unit*2) - 2;
		if ($param == "off") {
			++$index;
		}

		if ($index < 0 || $index >= count($this->handlers)) {
			return null;
		}
		return $this->handlers[$index];
	}

	function trace() {
		foreach ($this->handlers as $h) {
			return $h->trace();
		}
	}

}

class YaksConfig {
var $controllers;

	function YaksConfig() {
		$this->controllers = array();
	}

	function addController($controller) {
		array_push($this->controllers, $controller);
		return $controller;
	}

	function getHandler($code, $param) {
	# rem: e1 on = 1, e1 off = 2
		$housecode = substr($code, 0, 1);
		$unitcode = substr($code, 1, 1);
		foreach($this->controllers as $c) {
			if ($c->housecode == $housecode) {
				$h = $c->getHandler($unitcode, $param);
				if ($h) {
					return $h;
				}
			}
		}
	}

	function createNewController($device, $param, $house, $unit) {
		global $scriptsURL;
		$url = "$scriptsURL/$device";
		$count = 16;	// TODO: vary based on $param, i.e. ss13
		mkdir($url, 0755, true);
		for($i=1;$i<=$count;++$i) {
			$script = "$url/$i";
			file_put_contents($script, <<<EOD
 #!/bin/bash

if [ "$1" == "query" ]; then
  echo "$device button $i"
  exit 0
fi

 # Default paths
MINBASE=/usr/local/minerva
MINBIN=\$MINBASE/bin

## Code goes here
EOD
);
		chmod($script, 0755);
		}

		// Now update the config
		global $configURL;
		file_put_contents($configURL, <<<EOD
\$c = \$config->addController(new YaksController("$device", "$house", $unit, $count));

EOD
		, FILE_APPEND);
			
	}

}

?>
