<?php

require "resultbase.inc";

global $dpsLastError;

function getProcessor($type) {
switch($type) {
	case "json":
		require "resultjson.inc";
		return new dpsResultJSON();

	case "text":
	case "txt":
	case "plain":
		require "resultplain.inc";
		return new dpsResultPlain();

	default:
		require "resultxml.inc";
		return new dpsResultXML();
	}
}

?>

