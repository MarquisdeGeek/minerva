<?php
/* vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4: */

/**
* This is a WebFace class that contains a number of utility methods to render
* web controls, using only features of the underlying classes. Therefore, a
* developer may use their own Web_View class, and these implementations will
* still be valid.
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
*/

class WebFace_Utils
{
    public static $WARP_SLIDER_ARROWS_NONE = 0;
    public static $WARP_SLIDER_ARROWS_LEFT = 1;
    public static $WARP_SLIDER_ARROWS_RIGHT = 2;
    public static $WARP_SLIDER_ARROWS_STRADDLE = 3;

    public static $WARP_SLIDER_NUMBER_NONE = 0;
    public static $WARP_SLIDER_NUMBER_LEFT = 1;
    public static $WARP_SLIDER_NUMBER_RIGHT = 2;

    /**
     * Generates a anchor, or optionally, a piece of static text.
     * This is generally used as a placeholder abstraction for more complex
     * WebFace implementations, e.g. WARP.
     *
     * @&$webface   object  Reference to the current webface.
     * $param       string  The complete parameter name for passing back to
     *                      this page upon a click.
     * $value       string  The value to be attached to $param
     * $text        string  The HTML code to appear within the anchor.
     * $static      boolean When true, indicates that this anchor is read-only
     *                      and no anchors should be created. $text is returned
     *                      in this case.
    */
    public static function createAnchor(&$webface, $param, $value, $text, $static=false)
    {
        if ($static) {
            return $text;
        }
        return $webface->ctrl->createAnchor($param, $value, $text);
    }
 
    /**
     * This generates the HTML code to create a slider control suitable for
     * state-based web apps.
     *
     * <code>
     * $wf = new Web_Face();
     * print WebFace_Utils::renderSliderBar($wf, 0, 0, 100, 4, "curr", NULL, 
     *           WebFace_Utils::$WARP_SLIDER_ARROWS_STRADDLE, 
     *           WebFace_Utils::$WARP_SLIDER_NUMBER_RIGHT);
     * </code> 
     *
     * @&$webface   object  Reference to the current webface.
     * $default     int     The value position of the slider, if no parameter
     *                      (as taken from the WebFace controller) is supplied.
     * $minValue    int     The lower number on the scale.
     * $maxValue    int     The upper number on the scale.
     * $visible     int     The number of graphics/visual elements that will
     *                      be composited to form the slider. It is best to make
     *                      this a whole fraction of the number of elements in
     *                      your range.
     * $anchor      string  The anchor parameter used to pass the selection
     *                      back to the app.
     * $image       array   A list of images to use for left, right, on, and off
     *                      (in that order) in the slider bar. May be null, 
     *                      where it will use the graphics in /icons
     * $showArrows  int     Indicates the position where the increment and 
     *                      decrement arrows should go.
     * $showNumbers int     Indicates the position where the current slider
     *                      value should be written.
     * $static      boolean If set to true indicates that the slider is read
     *                      only (i.e. its value is static) and no anchor is
     *                      created.
     *
     * @return      string  The HTML necessary to handle a slider bar.
    */
    public static function renderSliderBar(&$webface, $default, $minValue, $maxValue, $visible, $anchor="", $images=array(), $showArrows=WARP_SLIDER_ARROWS_RIGHT, $showNumbers=WARP_SLIDER_NUMBER_RIGHT, $static=false)
    {
        // Prepare defaults for graphics, if none given
        if ($images == NULL || count($images) == 0) {
            $images[] = "/icons/left.gif";
            $images[] = "/icons/right.gif";
            $images[] = "/icons/ball.red.gif";
            $images[] = "/icons/ball.gray.gif";
        }

        // Compute basic img/alt text
        $imgLeft  = '<img border=0 src='.$images[0].' alt="<">';
        $imgRight = '<img border=0 src='.$images[1].' alt=">">';
        $imgOn    = '<img border=0 src='.$images[2].' alt="X">';
        $imgOff   = '<img border=0 src='.$images[3].' alt="-">';
    
        // Have we got a value for this slider already?
        $curr = $webface->ctrl->getParameterValue($anchor);
        if ($curr == "") {
            $curr = $default;
        }
    
        // Validate input
        if ($curr < $minValue) {
            $curr = $minValue;
        } else if ($curr > $maxValue) {
            $curr = $maxValue;
        }
        $total = ($maxValue - $minValue);
 
        // Generate the bar itself
        for($i=0; $i < $visible; $i++) {
            $lower = $minValue + floor($total / $visible) * ($i + 1);

            if ($curr >= $lower && $curr != 0) {
                $text = $imgOn;
            } else {
                $text = $imgOff;
            }
    
            $slider .= self::createAnchor($webface, $anchor, $lower, $text, $static);
        }
        $previous = $curr - $total/($visible);
        $next = $curr + $total/($visible);
    
        if ($previous < $minValue) {
            $previous = $minValue;
        }
    
        if ($next > $maxValue) {
            $next = $maxValue;
        }
    
        // Handle information to the left of the bar
        if ($showNumbers == self::$WARP_SLIDER_NUMBER_LEFT) {
            $bar .= "$curr ";
        }
    
        if ($showArrows == self::$WARP_SLIDER_ARROWS_LEFT || $showArrows == self::$WARP_SLIDER_ARROWS_STRADDLE) {
            $bar .= self::createAnchor($webface, $anchor, $previous, $imgLeft, $static);
            if ($showArrows == self::$WARP_SLIDER_ARROWS_LEFT) {
                $bar .= self::createAnchor($webface, $anchor, $next, $imgRight, $static);
            }
    
            $bar .= "&nbsp;";
        }
    
        // Now add the slider
        $bar .= $slider;
    
        // Handle information to the right of the bar
        if ($showArrows == self::$WARP_SLIDER_ARROWS_RIGHT || $showArrows == self::$WARP_SLIDER_ARROWS_STRADDLE) {
            $bar .= "&nbsp;";
    
            if ($showArrows == self::$WARP_SLIDER_ARROWS_RIGHT) {
                $bar .= self::createAnchor($webface, $anchor, $previous, $imgLeft, $static);
            }
            $bar .= self::createAnchor($webface, $anchor, $next, $imgRight, $static);
        }
    
        if ($showNumbers == self::$WARP_SLIDER_NUMBER_RIGHT) {
            $bar .= " $curr";
        }
    
        return $bar;
    }
}

?>

