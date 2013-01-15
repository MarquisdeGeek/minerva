<?php
/* vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4: */

/**
* This is a WebFace stub class that checks that a particular user is
* permitted to carry out a particular task.
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

class Web_Auth
{
    /**
     * Determine the user name of the currently logged in user.
     * @return string The user name, of "" if no one is logged in.
    */
    function getUser()
    {
       return "";
    }


    /**
     * Determines whether a user is carry out a particular task. Since these
     * tasks are application-specific, most developers will create their
     * own implementation of this method.
     * This version allows everything.
     * @$purpose string What task is to be queried. This can be as descriptive
     *                  as your code is prepared to parse. e.g. 'play_cd'
     * @$access  string It is suggested that this follows Unix filesystem
     *                  conventions. w=write(i.e. change) access is valid
     *                  r=read (or query) is valid. all=both are valid.
     *
     * @return boolean Indicates if the action is permitted.
    */
    function isUserValidFor($purpose, $access = "w")
    {
        return true;
    }
}

?>

