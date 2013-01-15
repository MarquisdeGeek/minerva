<?

# This code will give you the sunrise and sunset times for any
# latutude and longitude in the world. You just need to supply
# the latitude, longitude and difference from GMT.
#
# for results in areas not using Daylight Savings Time, delete all
# references to the is_daylight_time() function
#
# send the variables $latitude, $longitude and $timezone
# This script includes is_daylight_time() function from Steve Edberg
# and perl code translated from the perl module Astro-SunTime-0.01
# PHP code mattf@mail.com - please use this code in any way you wish
# and if you want to, let me know how you are using it.

if (!$latitude) {$latitude = 38.74274;}     #if not set use test variable
if (!$longitude) {$longitude = 0;} #if not set use test variable
if (!$timezone) {$timezone = 0;}           #if not set use test variable

   $yday = date(z);
   $mon = date(n);
   $mday = date(j);
   $year = date(Y);

function is_daylight_time($time)
{

 list($dom, $dow, $month, $hour, $min) = explode(":", date("d:w:m:H:i", $time));

 if   ($month > 4 && $month < 10) {
  $retval = 1;        # May thru September
 } elseif ($month == 4 && $dom > 7) {
  $retval = 1;        # After first week in April
 } elseif ($month == 4 && $dom <= 7 && $dow == 0 && $hour >= 2) {
  $retval = 1;        # After 2am on first Sunday ($dow=0) in April
 } elseif ($month == 4 && $dom <= 7 && $dow != 0 && ($dom-$dow > 0)) {
  $retval = 1;        # After Sunday of first week in April
 } elseif ($month == 10 && $dom < 25) {
  $retval = 1;        # Before last week of October
 } elseif ($month == 10 && $dom >= 25 && $dow == 0 && $hour < 2) {
  $retval = 1;        # Before 2am on last Sunday in October
 } elseif ($month == 10 && $dom >= 25 && $dow != 0 && ($dom-24-$dow < 1) ) {
  $retval = 1;        # Before Sunday of last week in October
 } else {
  $retval = 0;
 }

 return($retval);
}

$DST=is_daylight_time(date(U));
if ($DST) {$timezone = ($timezone + 1);}
if ($timezone == "13") {$timezone = "-11";}

   $A = 1.5708;
   $B = 3.14159;
   $C = 4.71239;
   $D = 6.28319;     
   $E = 0.0174533 * $latitude;
   $F = 0.0174533 * $longitude;
   $G = 0.261799  * $timezone;

   # For astronomical twilight, use R = -.309017
   # For     nautical twilight, use R = -.207912
   # For        civil twilight, use R = -.104528
   # For     sunrise or sunset, use R = -.0145439
   $R = -.0145439;






function get_suntime($i)
 {
global $A,$B,$C,$D,$E,$F,$G,$H,$R;
global $yday, $mon, $mday, $year;

if (!$i) {$J =  $A;  $type = "rise";}    # calculate sunrise
  else  {$J =  $C;  $type = "set"; }    # calculate sunset

   $K = $yday + (($J - $F) / $D);
   $L = ($K * .017202) - .0574039;              # Solar Mean Anomoly
   $M = $L + .0334405 * sin($L);                # Solar True Longitude
   $M += 4.93289 + (3.49066E-04) * sin(2 * $L);
  # Quadrant Determination
     if ($D == 0) {echo "Trying to normalize with zero offset..."; exit;}
     while ($M < 0)   {$M = ($M + $D);}
     while ($M >= $D) {$M = ($M - $D);}

   if (($M / $A) - intval($M / $A) == 0) {$M += 4.84814E-06;}
   $P = sin($M) / cos($M);                   # Solar Right Ascension
   $P = atan2(.91746 * $P, 1);

   # Quadrant Adjustment
   if ($M > $C) {$P += $D;}
   else {if ($M > $A) {$P += $B;} }

   $Q = .39782 * sin($M);            # Solar Declination 
   $Q = $Q / sqrt(-$Q * $Q + 1);     # This is how the original author wrote it!
   $Q = atan2($Q, 1);

   $S = $R - (sin($Q) * sin($E));
   $S = $S / (cos($Q) * cos($E));

   if (abs($S) > 1) {echo 'none';}     # Null phenomenon

   $S = $S / sqrt(-$S * $S + 1);
   $S = $A - atan2($S, 1);

   if ($type == 'rise') {$S = $D - $S ;}

   $T = $S + $P - 0.0172028 * $K - 1.73364; # Local apparent time
   $U = $T - $F;                            # Universal timer
   $V = $U + $G;                            # Wall clock time
  # Quadrant Determination
     if ($D == 0) {echo "Trying to normalize with zero offset..."; exit;}
     while ($V < 0)   {$V = ($V + $D);}
     while ($V >= $D) {$V = ($V - $D);}
   $V = $V * 3.81972;

   $hour = intval($V);
   $min  = intval((($V - $hour) * 60) + 0.5);

 //  echo "sun$type is at: ";

   //echo date( "g:i A", mktime($hour,$min,0,$mon,$mday,$year) );
   //echo "<br>";

return mktime($hour,$min,0,0,0,0);
//return mktime($hour,$min,0,$mon,$mday,$year);
 }

function GetSunRise() { return date( "H:i", get_suntime(0)); }
function GetSunSet()  { return date( "H:i", get_suntime(1)); }
function GetSunLight()  { 
	$sunrise = get_suntime(0);
	$sunset = get_suntime(1);

	$now = mktime(date("G"), date("i"),0,0,0,0);

	// Default
	$light = 100;
	$secMul = 60*60;	// secs in an hour

	// For the morning, it gets a little bit light 2 hrs before
	// and is fully bright 1 hr after.
	$earliestLight = $sunrise - $secMul*2.0;
	$fullMorningLight = $sunrise + $secMul * 1.0;
	
	if ($earliestLight < $now && $now < $fullMorningLight ) {
		$light = (($now - $earliestLight)*100) / ($fullMorningLight-$earliestLight);
	} 

	// In the evening...
	$lastFullDaylight = $sunset - $secMul * 1.0;
	$firstDarkess = $sunset +  $secMul * 1.0;
/*
print "set=$sunset  $lastFullDaylight   $now  $firstDarkess  \n ";	
print "now=".date("H i s  ", $now);
print date("H i s  ", $sunset);
print date("H i s  ", $lastFullDaylight);
print date("H i s  ", $firstDarkess);
*/
	if ($lastFullDaylight < $now && $now < $firstDarkess) {
//echo "fade";
		$light = (($now - $lastFullDaylight)*100) / ($firstDarkess-$lastFullDaylight);
		$light = 100 - $light;
	} else if ($now >= $firstDarkess) {
		$light = 0;
	}

	$light = (int)$light;

	return $light; 
}
?>
