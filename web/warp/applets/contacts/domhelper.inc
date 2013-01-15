<?php

class DomHelper {

static function getDateString($xpath, $dateNode) {
   if (!$dateNode || !$dateNode->hasAttributes()) {
      return "";
   }
   $format = "";
   if ($mn = $dateNode->attributes->getNamedItem("month")) {
      $month = $mn->value;
      $format.= "F ";
   }
   if ($dn = $dateNode->attributes->getNamedItem("day")) {
      $day = $dn->value;
      $format.= "jS ";
   }
   if ($yn = $dateNode->attributes->getNamedItem("year")) {
      $year = $yn->value;
      $format .= "Y";
   }

   return date ($format, mktime (0,0,0,$month, $day, $year));
}

static function getAddress($xpath, $addressNode) {
   $addrLine = $addressNode->getElementsByTagName("addr-line");
   foreach($addrLine as $line) {
      $str .= DomHelper::getAttributeString($xpath, $xpath->query('addr', $line), " ");
      $str .= "<br/>";
   }
   return $str;
}


static function getPhoneNumber($xpath, $phoneNode) {
   $c = $phoneNode->attributes->getNamedItem("code");
   if ($c)   $str .= "(".$c->value.") ";
   $c = $phoneNode->attributes->getNamedItem("number");
   if ($c)   $str .= $c->value;
   return $str;
}

static function getAttributeString($xpath, $nodeList, $delim) {
   foreach($nodeList as $n) {
      $str .= $n->attributes->item(0)->value;
      $str .= $delim;
   }
   return $str;
}

}

?>

