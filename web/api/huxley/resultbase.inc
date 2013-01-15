<?php

class dpsResultNode {
var $params;
var $description;
var $content;
var $children;
var $tag;

	function dpsResultNode($tag) {
		$this->children = array();
		$this->tag = $tag;
	}

	function addParam($prm, $value) {
		$this->params[$prm] = $value;
	}

	function addContent($descrip, $content="") {
		if ($content == "") {	// No extra tag is given, so place this in root
			$this->description = "";
			$this->content = $descrip;
		} else {
			$this->description = $descrip;
			$this->content = $content;
		}
	}

	function addChild($subnode) {
		array_push($this->children, $subnode);
	}

	function generate() {
	}

        function generateChildren() {
		$s = "";
		foreach($this->children as $child) {
			$s .= $child->generate();
		}
		return $s;
	}
};

class dpsResult {
var $root;

	function createNode() {
		return new dpsResultNode();
	}

	function getRootNode() {
		return $this->root;
	}

	function createRootNode($cmd,$args,$result) {
		$this->root = $this->createNode("dpsResult");
   		$this->root->addParam("cmd", "$cmd");
			$args = str_replace("&", "&amp;", $args);
   		$this->root->addParam("args", "$args");
   		$this->root->addParam("success", $result?"true":"false");

		if (!$result) {
			global $dpsLastError;
			$this->root->addContent("error", "Internal error: $dpsLastError");
		}

		return $this->root;
	}

	function generate() {
		return $this->root->generate();
	}

};

?>

