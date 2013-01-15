<?php

$minbin = "/usr/local/minerva/bin";
global $minbin;

class MinervaAPIRequest {
var $user;
var $device;
var $cmdBase;
var $conduit;
var $authorized;
var $accessAPI;
var $accessRead;
var $accessWrite;

	function MinervaAPIRequest($conduit) {
		global $minbin;
		
		$this->user = $_GET[user];
		$this->device = $this->getParam("device", "default");
		$this->conduit = $conduit;
		$this->cmdBase = "$minbin/".$_GET[cmd]." ".$this->device;
		
		$cmd = "$minbin/minuser pass ".$this->user." check ".$_GET[pass];
		exec("$cmd", $output, $this->authorized);

		if ($this->authorized) {
			$cmd = "$minbin/minuser auth ".$this->user;

			exec("$cmd api get r", $output, $this->accessAPI);
			exec("$cmd $conduit get r", $output, $this->accessRead);
			exec("$cmd $conduit get w", $output, $this->accessWrite);

			$this->accessRead &= $this->accessAPI;
			$this->accessWrite &= $this->accessAPI;
		}
	}

	function enumerateDevices($processor, $app) {
		global $minbin;
		
		$output = array();
		$node = $processor->getRootNode();
		exec("$minbin/$app default enumdev", $output);
		foreach($output AS $dev) {
      	$n = $processor->createNode("device");
			$n->addParam("device", $dev);
      	$node->addChild($n);
		}
	}

	function getParam($param, $defaultValue) {
		$result = $_GET[$param];
		if ($result == "") {
			$result = $defaultValue;
		}
		return $result;
	}

     function report() {
         $str = "User : ".$this->user."\n";
         $str.= "Authorized on system: ".($this->authorized?"True":"False")."\n";
         $str.= "Access to API : ".($this->accessAPI?"True":"False")."\n";
         $str.= "Access to '".$this->conduit."' conduit:\n";
         $str.= "Read access : ".($this->accessRead?"True":"False")."\n";
         $str.= "Write access : ".($this->accessWrite?"True":"False")."\n";
         return $str;
     }

}

?>
