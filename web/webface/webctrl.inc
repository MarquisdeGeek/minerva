<?php
/* vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4: */

/**
* This is a WebFace class which builds control anchors to pass parameters
* between one invocation of the script, and the subsequent GET message.
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

class Web_Ctrl
{
    /**
     * Retrieve a parameter from the request. These appear in the browser
     * caption as, for example, page.php?param1=value1&param2=value2
     * Since a Web_Ctrl is rarely generated manually, you will reference 
     * this through a WebFace.
     *
     * <code>
     * $wf = new Web_Face();
     * $curr = $wf->ctrl->getParameterValue("param");
     * </code>
     *
     * @param string The parameter as present in the GET request.
     *
     * @return string The parameter found in the request.
    */
    function getParameterValue($param)
    {
        global $HTTP_GET_VARS;
    
        return $HTTP_GET_VARS[$param];
    }

    /**
     * Retrieve the URL of the current PHP page. This is used when making 
     * anchors back to ourself.
     *
     * @return string The current page.
    */
    function getSelf()
    {
        return $_SERVER[PHP_SELF];
    }

    /**
     * Retrieve a compliant parameter name to use in the GET request. Note
     * that when using some alternate WebFace control mechanisms (e.g. WARP)
     * the parmeters may be prepended with an ID of the current applet, so
     * is it not safe to assume this is will always be a null operation.
    */
    function createAnchorParam($param)
    {
        return "$param";
    }

    /**
     * Build a fully compliant anchor that points back to this page, with the
     * requested parameter and value pair.
     * This will generally appear thus:
     * <code>
     * print $wf->ctrl->createAnchor($wf->ctrl->createAnchorParam("prm"), $curr+1, "Next");
     * </code>
    */ 
    function createAnchor($param, $value, $text)
    {
        return "<a href=".$this->getSelf()."?$param=$value>$text</a>";
    }

}

?>

