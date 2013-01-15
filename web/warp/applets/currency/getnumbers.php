<?php

$fromCurrency = "eur";
$toCurrency = "gbp";

try {
	@$client = new SoapClient("http://xurrency.com/api.wsdl");

	$currencyList = $client->getCurrencies();

	$fromName = $client->getName($fromCurrency);
	$toName = $client->getName($toCurrency);
	$toTarget = $client->getValue(1, $fromCurrency, $toCurrency);
	$fromTarget = $client->getValue(1, $toCurrency, $fromCurrency);
	
	$info = "1 ".$fromName."=".round($toTarget,2)." ".$toName;
	$info.= "<br/>";
	$info.= "1 ".$toName."=".round($fromTarget,2)." ".$fromName;
	
	print $info;

} catch (Exception $e) {
	print "Exceptional problem with currency handler...";
}

?>

