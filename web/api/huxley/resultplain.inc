<?php

class dpsResultNodePlain extends dpsResultNode {
	function generate() {
		$s = "$this->tag:\n";
		if ($this->params) {
			foreach($this->params as $key=>$value) {
				$s .= "$key=\"$value\"\n";
			}
		}
		$s .= $this->description." ".$this->content."\n";
		$s .= $this->generateChildren();
		return $s;
	}
};
 
class dpsResultPlain extends dpsResult {
        function createNode($tag) {
                return new dpsResultNodePlain($tag);
        }
};

?>


