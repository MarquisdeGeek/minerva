<?php

class Warp_MainAdmin_Applet extends Warp_Applet
{

    function Warp_MainAdmin_Applet() {
       parent::__construct("Administration Console");
    }

    function renderWindow(&$appMan)
    {
return <<<EOS
<p>
<i>Minerva</i> relies on a number of underlying technologies. They are all
lightweight enough to be installed on all systems, even if they're not used.
However, for completeness, WebFace and WARP are only necessary if you are using
the web browser to control the Minerva applets. Cosmic is only needed if you
have an X10 RF transmitter.
</p><p>
Bearskin is the abstraction layer for the controller software, and is always
necessary.
</p><p>
Minerva Web contains a number of WARP-compliant applets all stored in
<tt>/var/www/minerva/warp</tt>. These web applets use the <i>Zinc</i>
technology to make the Bearskin components callable from PHP. This is stored
in <tt>/var/www/minerva/zinc</tt>.
</p><p>
Minerva installs itself into </p><pre>/usr/local/minerva</pre> and adopts the
familar <pre>etc</pre> and <pre>bin</pre> directory structure from Unix.
<p>Currently, all Bearskin configuration must be done by editing the files in <tt>/usr/local/minerva/etc</tt>, with the web interfaces and settings control 
through <tt>/var/www/minerva/warp/conf</tt> and <tt>/var/www/minerva/zinc/conf</tt>.
</p><p>
The authority modules currently installed are:
<ul>
<li>cdplayer</li>
<li>bluetooth</li>
<li>system (df/smb/etc)</li>
<li>mixer</li>
<li>mp3jukebox</li>
<li>photos</li>
<li>videostream</li>
<li>vlc</li>
<li>x10</li>
</ul>


EOS;
}
}

?>


