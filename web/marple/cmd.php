<?php 

function getCommand($cmd, $args) {

   # in case someone tries exec'ing other programs, in different directories
   # we'll try and stop them.
   $cmd = str_replace("/", "", $cmd); 
   $cmd = str_replace("..", "", $cmd); 

   $minervaPath = "/usr/local/minerva";
   $fullCommand ="$minervaPath/bin/$cmd $args";

   return $fullCommand;
}

function marpleback($cmd, $args) {
   $fullCommand = getCommand($cmd, $args);
   $fullCommand.= " >/dev/null 2>&1 &";

   $result = array();
   exec($fullCommand, $result);
   return ""; // no output for background tasks
}

function marple($cmd, $args) {
   $fullCommand = getCommand($cmd, $args);
   $result = array();

   exec($fullCommand, $result);

   $rts = "";
   foreach ( $result as $v ) {
     $rts .= "$v\n";
   }
   return $rts;
}

function help() {
   return "One day there'll be help here...";
}

   $server = new SoapServer(null, 
      array('uri' => "urn://www.minervahome.net/marple"));
   $server->addFunction("marple");
   $server->addFunction("marpleback");
   $server->addFunction("help"); 
   $server->handle(); 
?>

