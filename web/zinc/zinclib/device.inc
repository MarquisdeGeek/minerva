<?php

class ZincDevice
{
protected static $last_cmd;
protected static $last_out;
protected static $last_err;

    // System handling

    public static function getLastCommand()
    {
        return self::$last_cmd;
    }

    public static function getLastError()
    {
        return self::$last_err;
    }

    public static function getLastOutput()
    {
        return self::$last_out;
    }

    public static function getDeviceStatus()
    {
        return "OK";
    }

    protected static function cmd($cmd)
    {
        self::$last_cmd = escapeshellcmd($cmd);
        self::$last_out = shell_exec(self::$last_cmd);
        return self::$last_out;
    }


}

?>

