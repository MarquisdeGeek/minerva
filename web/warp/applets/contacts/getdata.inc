<?php

class PersonalData {

   static function getSortedNameIndex($xpath) {
      # Get surname index
      $surnames = $xpath->query('//minerva-person/minerva-personal/full-name/name/@surname');
      $idx = 0;
      foreach($surnames as $s) {
         $snList[$idx++] = $s->value;
      }

      asort($snList);
      return $snList;
      }

   static function getNameList($xpath) {
      $idx = 0;
      foreach ($xpath->query('//minerva-person') as $personNode) {
         $nameNodes = $xpath->query('minerva-personal/full-name/name', $personNode);
         $nameList[$idx] = DomHelper::getAttributeString($xpath, $nameNodes, " ");
         ++$idx;
      }
      return $nameList;
   }

   static function getPersonNodeList($xpath, $imageDir) {
      $idx = 0;
      foreach ($xpath->query('//minerva-person') as $personNode) {
         $nodeList[$idx] = PersonalData::getPersonNode($xpath, $imageDir, $personNode);
         ++$idx;
      }
      return $nodeList;

   }

   static function row($a, $b, $c) {
      return "<tr><td width=20%>$a</td><td valign=top width=30>$b</td><td valign=top width=70%>$c</td></tr>";
   }

   static function getPersonNode($xpath, $imageDir, $personNode) {
      $str = "";
      $dob = $xpath->query('minerva-personal/dob', $personNode);
      if ($dob) {
         $dob = $dob->item(0);
         $str .= "<tr><td colspan=3 bgcolor=#eeeeee>".DomHelper::getDateString($xpath, $dob)."</td></tr>";
      }

      $locations = $xpath->query('minerva-contact/location', $personNode);
      foreach($locations as $loc) {
         $str .= PersonalData::row("<b>".ucfirst($loc->attributes->getNamedItem("at")->value)."</b>", "", "");
                                                                                          if ($phone = $loc->getElementsByTagName("phone")) {
            foreach($phone as $p) {
               $phoneNumber = DomHelper::getPhoneNumber($xpath, $p);
               $str .= PersonalData::row("","<img src=$imageDir/phone.png alt=Phone>", "<a href='skype:$phoneNumber?call'>$phoneNumber</a>");
            }
         } // fi phone                                                              
         $addr = $loc->getElementsByTagName("address");
         foreach($addr as $a) {
               $str .= PersonalData::row("","<img src=$imageDir/addr.png alt=Address>", DomHelper::getAddress($xpath, $a));
         }

         $email= $loc->getElementsByTagName("email");
         foreach($email as $e) {
             $emailAddress = $e->attributes->item(0)->value;
             $str .= PersonalData::row("","<img src=$imageDir/email.png alt=Email>", "<a href=mailto:$emailAddress>$emailAddress</a>");
         }

         if ($web = DomHelper::getAttributeString($xpath, $xpath->query('web', $loc), " ")) {
             if (substr($web, 0, 7) != "http://") $web = "http://$web";
             $str .= PersonalData::row("","<img src=$imageDir/web.png alt=Web", "<a href=$web>$web</a>");
         }
      }

   return $str;
   }

}

?>

