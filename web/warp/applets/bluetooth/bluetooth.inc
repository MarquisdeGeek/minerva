<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';

class Warp_Bluetooth_Applet extends Warp_Applet
{
    function Warp_Bluetooth_Applet($caption = "Bluetooth scanner")
    {
        parent::__construct($caption);
    }

    function getDirectory()
    {
        return "bluetooth";
    }

    function renderWindow(&$appMan)
    {
        if (!$appMan->getAuth()->isUserValidFor("bluetooth", "r")) {
            return "";
        }

$html = <<< EOF
        <div id="output">Scanning...<br/><img src="icons/ajaxwaitmini.gif" alt=""></div>
        <script type="text/javascript" src="utils/ajax.js"></script>
        <script language="Javascript">
           xmlhttpPost("warp/applets/bluetooth/btscan.php", "", "output");
        </script>
EOF;

        return $html;
    }
}

?>

