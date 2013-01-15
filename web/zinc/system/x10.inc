<?php
require_once 'zinc/zinclib/device.inc';
require_once 'zinc/conf/x10control.conf';

class X10 extends ZincDevice
{
    public static $device_off = "off";
    public static $device_on = "on";

    public static $type_appliance = "appliance";
    public static $type_dimmer = "dimmer";
    public static $type_lamp = "lamp";
    public static $type_cm11 = "cm11";

    protected static function cmd($cmd)
    {
        $c = X10ControlDevice::$binary." default ".$cmd;
        return parent::cmd($c);
    }


    public static function getDeviceType($device)
    {
        return $type_appliance;
    }

    public static function getRange($device)
    {
        if (true || self::getDeviceType($device) == $type_dimmer) {
            return 22;
        }
        return 1;
    }

    public static function getBrightness($device) 
    {
        if (true || self::getDeviceType($device) == $type_dimmer) {
            return self::cmd("dimlevel $device");
        }
        return 0;
    }

    public static function setBrightness($device, $value) 
    {
        $curr = self::getBrightness($device);

        $curr = (int)$curr;
        $value = (int)$value;

        if ($value > $curr) {
            $delta = $value-$curr;
            self::$last_cmd = "bright ".$device;
        } else {
            $delta = $curr-$value;
            self::$last_cmd = "dim ".$device;
        }
        self::$last_cmd .= " ".(int)(($delta * 21)/100);
        self::$last_cmd .= self::cmd(self::$last_cmd);
    }

    public static function getStatus($device) 
    {
        return (int)self::cmd("ison $device");
    }


    public static function setStatus($device, $setting)
    {
        if ($setting == self::$device_off) {
            self::cmd("off $device");
 
        } else if ($setting ==  self::$device_on) {
            self::cmd("on $device");
        }
    }

    public static function getControllerStatus() 
    {
        return self::cmd("status");
    }

    public static function getControllerInfo() 
    {
        return self::cmd("info");
    }

    public static function allOn($houseCode) 
    {
        self::cmd("lightson ".$houseCode);
    }

    public static function allOff($houseCode) 
    {
        self::cmd("lightsoff ".$houseCode);
    }
}

?>

