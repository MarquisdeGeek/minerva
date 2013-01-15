<?php

function getServerName() {
   return $_SERVER['SERVER_NAME'];
}

function getServerPort() {
   return $_SERVER['SERVER_PORT'];
}

function getRemotePort() {
   return $_SERVER['REMOTE_PORT'];
}

function getIPAddress()
{
    if (!empty($_SERVER['HTTP_X_FORWARDED_FOR'])) {
       $ip=$_SERVER['HTTP_X_FORWARDED_FOR'];
    } else if (!empty($_SERVER['HTTP_CLIENT_IP'])) {
       $ip=$_SERVER['HTTP_X_FORWARDED_FOR'];
    } else {
       $ip=$_SERVER['REMOTE_ADDR'];
    }
    return $ip;
}


?>

