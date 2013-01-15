<?php

class dpsResultNodeJSON extends dpsResultNode {
	function generate() {
		$s = "{ \"$this->tag\": {\n";
		if ($this->params) {
			foreach($this->params as $key=>$value) {
				$s .= "{ \"$key\": \"$value\" }, ";
			}
		}
		$s .= "},\n";
		$s .= " \"content\": \"".$this->content."\" }";
		$s .= $this->generateChildren();
		$s .= "}\n";
		return $s;
	}
};
 
class dpsResultJSON extends dpsResult {
        function createNode($tag) {
                return new dpsResultNodeJSON($tag);
        }
};

?>


