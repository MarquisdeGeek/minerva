<?php
function getField($field, $data, $ref=null, $refid=null) {
	$result = "<data field=\"$field\"";
	if ($ref != null) {
		$result.= " reference=\"$ref\"";
		if ($refid != null) {
			$result.= " referenceid=\"$refid\"";
		}
	}
	$result.=">$data</data>\n";
	return $result;
}

function writeField($field, $data, $ref=null, $refid=null) {
	echo getField($field, $data, $ref, $refid);
}

function writeFieldList($list, $tag, $ref) {
	foreach($list AS $entry) {
		writeField($tag, $entry["name"], "$ref", $entry["imdb"]);
	}
}

?>
