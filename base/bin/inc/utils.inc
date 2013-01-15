<?php


   function getContents($url, $cachename, $maxAge) {
		$output = array();
		exec("/usr/local/minerva/bin/getalias cache", $output);
		$cacheRoot = $output[0];

      $fn = "$cacheRoot/$cachename";
      if (file_exists($fn)) {
         $mt = filemtime($fn);

         if ($mt + $maxAge > time()) {
            return file_get_contents($fn);
         }
      }
      // fyi, @ stops errors being output
      $url = trim($url);   // spaces can throw off the PHP loader
      $contents = @file_get_contents($url);
      @file_put_contents($fn, $contents);
      return $contents;
   }
?>
