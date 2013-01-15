<?php

class OAuthDetails {
	var $ckey;
	var $csecret;
	var $oatoken;
	var $oasecret;
}

function getDefaultUser() {
   return "steev";
}

function getTwitterDetails($user) {
   $contents = @file("/usr/local/minerva/etc/users/$user/external/twitter");
	$oauth = new OAuthDetails();
	$oauth->ckey = trim($contents[0]);
	$oauth->csecret = trim($contents[1]);
	$oauth->oatoken = trim($contents[2]);
	$oauth->oasecret = trim($contents[3]);
   return $oauth;
}

function getTwitterUser($user) {
   $contents = @file("/usr/local/minerva/etc/users/$user/external/twitter");
   return trim( $contents[0]);
}

function getTwitterPass($user) {
   $contents = @file("/usr/local/minerva/etc/users/$user/external/twitter");
   return trim($contents[1]);
}

?>
