<?php

class dpsResultNodeXML extends dpsResultNode {
	function generate() {
		$s = "<$this->tag";
		if ($this->params) {
			foreach($this->params as $key=>$value) {
				$s .= " $key=\"$value\"";
			}
		}
		$s .= ">\n";
		if ($this->content != "") {
			if ($this->description == "") {
				$s .= "$this->content\n";
			} else {
				$s .= "<$this->description>$this->content</$this->description>\n";
			}
		}
		$s .= $this->generateChildren();
		$s .= "</$this->tag>";
		return $s;
	}
};
 
class dpsResultXML extends dpsResult {
        function createNode($tag) {
                return new dpsResultNodeXML($tag);
        }
};

?>


