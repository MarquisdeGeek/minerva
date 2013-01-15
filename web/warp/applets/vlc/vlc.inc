<?php

require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';
require_once 'warp/conf/vlc/vlc.conf';

class Warp_VLC_Applet extends Warp_Applet
{
    function getName()
    {
       return "Video Stream";
    }

    function init(&$appMan)
    {
        Warp_Applet::Init($appMan);
    }

    function getDirectory() 
    {
       return "vlc";
    }

    function getPlayingString(&$appMan)
    {
        if (!$appMan->getAuth()->isUserValidFor("vlc", "r")) {
           return "Unavailable...";
        } else if (!VideoStreamControl::isPlaying()) {
           return "Nothing is playing...";
        }
        return "Currently playing '".VideoStreamControl::getCurrentName()."'...";
    }

    function renderPanel(&$appMan)
    {
        return $this->getPlayingString($appMan);
    }

    function renderWindow(&$appMan)
    {
       if (!$appMan->getAuth()->isUserValidFor("vlc", "r")) {
          return "";
       }

       $server = Warp_VLC_Conf::getServerURL();

       $vlc  = $this->getPlayingString($appMan)."<br/><br/>";
       $vlc .= <<<END
<embed type="application/x-vlc-plugin"
         name="video1"
         autoplay="yes" loop="no" width="400" height="300"
         target="$server"/>
END;

       if ($appMan->getAuth()->isUserValidFor("vlc", "w")) {
          $vlc .= <<<END
<br />
  <a href="javascript:;" onclick='document.video1.play()'>Play video</a>
&nbsp;|&nbsp;
  <a href="javascript:;" onclick='document.video1.pause()'>Pause video</a>
&nbsp;|&nbsp;
  <a href="javascript:;" onclick='document.video1.stop()'>Stop video</a>
&nbsp;|&nbsp;
  <a href="javascript:;" onclick='document.video1.fullscreen()'>Fullscreen</a>

END;
        }

      $navigator = "";

      return $appMan->getView()->split2V($vlc, $navigator);
    }


    function renderInfo(&$app_man, $fast=false)
    {
        if ($fast) {
            return "-";
        }

        return <<<EOD
<p>A simple applet that hooks into a VLC server, whose address and port is dictated by <tt>warp/conf/vlc/vlc.conf</tt>. </p>
<p>You can control the VLC server using the <i>VideoStreamControl</i> applet
provided through Minerva. This provides an explorer-style window to let you
choose which file to start playing. You can also view the output in the
standalone VLC client.
<p>Alternatively, you may start VLC on the server mentioned in the vlc.conf file thusly,
<pre>
vlc --sout='#standard{access=http,mux=ogg,dst=192.168.1.1:8080}' /media/video/cc/cenobitic.mpg
</pre>
Replacing the <tt>dst=</tt> part with the real address and port to which your clients will be listening, and the filename with your actual file.
EOD;
    }

}


