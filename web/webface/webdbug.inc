<?php
/* vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4: */

/**
* This is a WebFace class that stores lists of errors, warning, and info
* text. 
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

class Web_Dbug
{
    var $info = array();
    var $warn = array();
    var $err = array();
   
    /**
     * Placeholder constructor
    */ 
    function Web_Dbug()
    {
    }
    
    /**
     * Adds an error string to the current list.
    */ 
    function error($e)
    {
        $this->err[] = $e;
    }
    
    /**
     * Adds a warning string to the current list.
    */ 
    function warning($w)
    {
        $this->warn[] = $w;
    }
    
    /**
     * Adds an informational string to the current list.
    */ 
    function info($i)
    {
        $this->info[] = $i;
    }
    
    /**
     * Write all errors in an HTML-friendly format.
     *
     * @return string All errors in the list. 
    */ 
    function dumpErrors()
    {
        foreach($this->err AS $e)
        {
            $errlist.= $e."<br>";
        }
        
        return $errlist;
    }
    
    /**
     * Write all warnings in an HTML-friendly format.
     *
     * @return string All warnings in the list. 
    */ 
    function dumpWarnings()
    {
        foreach($this->warn AS $w)
        {
            $warnlist.= $w."<br>";
        }
        
        return $warnlist;
    }
    
    
    /**
     * Write all information in an HTML-friendly format.
     *
     * @return string All informational strings in the list. 
    */ 
    function dumpInfo()
    {
        foreach($this->info AS $i)
        {
            $infolist.= $i."<br>";
        }
        
        return $infolist;
    }
    
    /**
     * Write all data in an HTML-friendly format.
     *
     * @return string All strings present - errors, warnings, and info.
    */ 
    function dumpAll()
    {
        $html = "<p><b>Errors</b><br>" . $this->dumpErrors();
        $html.= "<p><b>Warnings</b><br>" . $this->dumpWarnings();
        $html.= "<p><b>Information</b><br>" . $this->dumpInfo();
        
        return $html;
    }
    
}

?>

