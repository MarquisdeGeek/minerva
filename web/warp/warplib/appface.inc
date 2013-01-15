<?php

// Standard webface access
require_once 'webface/webface.inc';

// Applet specific webface
require_once 'warp/warplib/appauth.inc';
require_once 'warp/warplib/appctrl.inc';
require_once 'warp/warplib/appview.inc';
require_once 'warp/warplib/appdbug.inc';

class Applet_Face extends Web_Face 
{
    function Applet_Face()
    {
        $this->auth = new Applet_Auth;
        $this->ctrl = new Applet_Ctrl;
        $this->view = new Applet_View;
        $this->dbug = new Applet_Dbug;
    }

    function init(&$app)
    {
        $this->ctrl->init($app);
    }
}

?>

