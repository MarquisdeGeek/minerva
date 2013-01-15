<?php

require_once 'appauth.inc';

class Applet_Auth extends Web_Auth
{
    function getUser()
    {
       return $_SERVER['PHP_AUTH_USER'];
    }

    function isUserValidFor($purpose, $access = "w")
    {
        $user = $_SERVER['PHP_AUTH_USER'];
        $exitCode = 0;

        system("/usr/local/minerva/bin/minuser auth $user $purpose get $access", $exitCode);

        return $exitCode == 1 ? true : false;
    }

}

?>

