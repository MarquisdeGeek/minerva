<?php
/* vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4: */

/**
* This is a WebFace class to generate HTML that contains visual elements.
* e.g. boxes with borders and split panes.
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

class Web_View
{
    /**
     * Creates the initial HTML head and body blocks
     * <code>
     * $webface = new Web_View();
     * print $webface->htmlStart("My Page", "", "");
     * </code>
     *
     * @param string $title The title for your page.
     * @param string $headHTML   HTML code to embed with the <head> block. e.g.
     *                           javascript code.
     * @param string $bodyParams Any (and all) arguments for the <body> 
     *                           element. e.g. onLoad
     *
     * @return string The resultant HTML code for output to the client.
    */
    function htmlStart($title, $headHTML, $bodyParams)
    {
        return "<html><head>\n<title>$title</title>\n$headHTML</head>\n<body $bodyParams>\n";
    }
   
    /*
     * Closes the HTML page. Once this has been written, no more HTML should
     * be written.
     *
     * @return string The resultant HTML code for output to the client.
    */ 
    function htmlEnd()
    {
        return "</body></html>";
    }
   
    /**
     * Render a thin border around the HTML present in $pane.
     * @param string $pane The HTML to be surrounds by a border.
     * @param string $class The name of the CSS style to use. Optional.
     *
     * @return string The resultant HTML code for output to the client.
    */
    function border($pane, $class="")
    {
        $cl  = $class=="" ? "" : "class=$class";
        $pn  = "<table $cl cellspacing=0 cellpadding=0 border=1 width=100%>";
        $pn .= "<tr><td>\n$pane\n</td></tr></table>";
        return $pn;
    }
   
    /*
     * Render two panes, with the content split horizontally so the two
     * pieces of HTML sit above each other.
     * @param string $p1 The HTML content to appear on top.
     * @param string $p2 The HTML content to appear underneath.
     *
     * @return string The resultant HTML code for output to the client.
    */ 
    function split2H($p1, $p2)
    {
        $page  = "<table cellspacing=0 cellpadding=0 border=0 width=100%>";
        $page .= "<tr><td>\n$p1\n</td></tr>";
        $page .= "<tr><td>\n$p2\n</td></tr>";
        $page .= "</table>";
        return $page;
    }
    
    /*
     * Render two panes, with the content split vertically so the two
     * pieces of HTML sit side by side.
     *
     * @param string $p1  The HTML content to appear on the left.
     * @param string $p2  The HTML content to appear on the right.
     * @param int    $pc1 If used, indicates a specific width must be used
     *                    for $p1. May be a number of pixels, or a percentage. 
     * @param int    $pc2 If used, indicates a specific width must be used
     *                    for $p2. May be a number of pixels, or a percentage. 
     *
     * @return string The resultant HTML code for output to the client.
    */
    function split2V($p1, $p2, $pc1="", $pc2="")
    {
        if ($pc1 != "") {
            $pc1 = "width=".$pc1;
        }
        if ($pc2 != "") {
            $pc2 = "width=".$pc2;
        }
        $page  = "<table cellspacing=0 cellpadding=0 border=0 width=100%>";
        $page .= "<tr><td valign=top $pc1>\n$p1\n</td>";
        $page .= "<td valign=top $pc2>\n$p2\n</td></tr>";
        $page .= "</table>";
        return $page;
    }
    
    /*
     * Render three panes in a triangle, with the first atop the second two.
     *
     * @param string $p1  The HTML content to appear on top.
     * @param string $p2  The HTML content to appear at bottom left.
     * @param string $p3  The HTML content to appear at bottom right.
     *
     * @return string The resultant HTML code for output to the client.
    */
    function split3T($p1, $p2, $p3)
    {
        $page  = "<table cellspacing=0 cellpadding=0 border=0 width=100%>";
        $page .= "<tr><td colspan=2>\n$p1\n</td></tr>";
        $page .= "<tr><td>$p2</td><td>\n$p3\n</td></tr>";
        $page .= "</table>";
        return $page;
    }
    
    /*
     * Render three panes, split vertically so the HTML sit side by side. 
     *
     * @param string $p1  The HTML content to appear on the left.
     * @param string $p2  The HTML content to appear in the center.
     * @param string $p3  The HTML content to appear on the right.
     * @param int    $pc1 If used, indicates a specific width must be used
     *                    for $p1. May be a number of pixels, or a percentage. 
     * @param int    $pc2 If used, indicates a specific width must be used
     *                    for $p2. May be a number of pixels, or a percentage. 
     * @param int    $pc3 If used, indicates a specific width must be used
     *                    for $p3. May be a number of pixels, or a percentage. 
     *
     * @return string The resultant HTML code for output to the client.
    */
    function split3V($p1, $p2, $p3, $pc1="", $pc2="", $pc3="")
    {
        if ($pc1 != "") {
            $pc1 = "width=".$pc1;
        }
        if ($pc2 != "") {
            $pc2 = "width=".$pc2;
        }
        if ($pc3 != "") {
            $pc3 = "width=".$pc3;
        }
        $page  = "<table cellspacing=0 cellpadding=0 border=0 width=100%>";
        $page .= "<tr><td $pc1>\n$p1\n</td><td $pc2>\n$p2\n</td><td $pc3>\n$p3\n</td></tr>";
        $page .= "</table>";
        return $page;
    }
    
    /*
     * Render three panes as an inverse triangle, with the two on top, and
     * the third underneath them both.
     *
     * @param string $p1  The HTML content to appear on the top left.
     * @param string $p2  The HTML content to appear on the top right.
     * @param string $p3  The HTML content to appear in the base.
     * @param int    $pc1 If used, indicates a specific width must be used
     *                    for $p1. May be a number of pixels, or a percentage. 
     * @param int    $pc2 If used, indicates a specific width must be used
     *                    for $p2. May be a number of pixels, or a percentage. 
     *
     * @return string The resultant HTML code for output to the client.
    */
    function split3L($p1, $p2, $p3, $pc1="", $pc2="")
    {
        if ($pc1 != "") {
            $pc1 = "width=".$pc1;
        }
        if ($pc2 != "") {
            $pc2 = "width=".$pc2;
        }
        $page  = "<table cellspacing=0 cellpadding=0 border=1 width=100%>";
        $page .= "<tr><td $pc1>\n$p1\n</td><td $pc2>\n$p2\n</td></tr>";
        $page .= "<tr><td colspan=2>\n$p3\n</td></tr>";
        $page .= "</table>";
        return $page;
    }
    
    /*
     * Render four panes in a 2x2 square.
     *
     * @param string $p1  The HTML content to appear on the top left.
     * @param string $p2  The HTML content to appear on the top right.
     * @param string $p3  The HTML content to appear on the bottom left.
     * @param string $p4  The HTML content to appear on the bottom right.
     * @param int    $pc1 If used, indicates a specific width must be used
     *                    for $p1. May be a number of pixels, or a percentage. 
     * @param int    $pc2 If used, indicates a specific width must be used
     *                    for $p2. May be a number of pixels, or a percentage. 
     * @param int    $pc3 If used, indicates a specific width must be used
     *                    for $p3. May be a number of pixels, or a percentage. 
     * @param int    $pc4 If used, indicates a specific width must be used
     *                    for $p4. May be a number of pixels, or a percentage. 
     *
     * @return string The resultant HTML code for output to the client.
    */
    function split4($p1, $p2, $p3, $p4, $pc1="", $pc2="", $pc3="", $pc4="")
    {
        if ($pc1 != "") {
            $pc1 = "width=".$pc1;
        }
        if ($pc2 != "") {
            $pc2 = "width=".$pc2;
        }
        if ($pc3 != "") {
            $pc3 = "width=".$pc3;
        }
        if ($pc4 != "") {
            $pc4 = "width=".$pc4;
        }
        
        $page  = "<table cellspacing=0 cellpadding=0 border=0 width=100%>";
        $page .= "<tr><td $pc1>\n$p1\n</td><td $pc2>\n$p2\n</td></tr>";
        $page .= "<tr><td $pc3>\n$p3\n</td><td $pc4>\n$p4\n</td></tr>";
        $page .= "</table>";

        return $page;
    }
    
}

?>

