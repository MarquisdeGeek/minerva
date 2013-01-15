<?php
/* vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4: */

/**
* This is a WebFace class which constructs the basic WebFace interfaces.
*
* PHP versions 4 and 5
*
* LICENSE: This source file is subject to version 2.1 of the LGPL license.
* It is available from http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html 
*
* @category   HTML
* @package    WebFace 
* @author     Steven Goodwin <goodwin_steven@hotmail.com>
* @copyright  2007 Steven Goodwin
* @license    http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html LGPL 2.1
* @version    CVS: $Id:$
* @link       http://pear.php.net/package/webface
* @since      File available since Release 0.1.0
*/

require_once 'webview.inc';
require_once 'webctrl.inc';
require_once 'webauth.inc';
require_once 'webdbug.inc';

class Web_Face 
{
    var $view;
    var $ctrl;
    var $auth;
    var $dbug;

    function Web_Face()
    {
        $this->view = new Web_View;
        $this->ctrl = new Web_Ctrl;
        $this->auth = new Web_Auth;
        $this->dbug = new Web_DBug;
    }

}

?>

