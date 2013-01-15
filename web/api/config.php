<?php
require_once "request.inc";
require_once "huxley/huxley.inc";

$processor = getProcessor($_GET['fmt']);

$node = $processor->createRootNode("config", "", true);

// Train stations for live departure boards
$n = $processor->createNode("ldb");
$n->addParam("station1", "LUT");
$n->addParam("station2", "STP");
$node->addChild($n);

echo $node->generate();

?>

