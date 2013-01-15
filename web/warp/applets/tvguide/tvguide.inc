<?php

require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';

require_once 'warp/conf/tvguide/tvguide.conf';

ini_set('include_path', '/usr/local/minerva:'.ini_get('include_path'));

require_once 'bin/inc/tvguide/progs.inc';
require_once 'bin/inc/tvguide/users.conf';
require_once 'bin/inc/tvguide/channels.conf';

class Warp_TVGuide_Applet extends Warp_Applet
{
var $_user;
var $_day;

    function Warp_TVGuide_Applet($caption = "TV Guide")
    {
        parent::__construct($caption);
    }

    function init(&$appMan)
    {
        Warp_Applet::Init($appMan);

        $this->_user = $appMan->QueryParameter($this, "user");
        $this->_user = $appMan->QueryParameter($this, "setuser", $this->_user);

        if ($this->_user != $appMan->getAuth()->getUser()) {
           $this->_user = "public";
        }

        $this->_day = appletUtils::getWrappedIndex($appMan, $this, "day", 0, 3);
        $cmd = $appMan->QueryParameter($this, "setday");
        if ($cmd != "") {
            $this->_day = appletUtils::getWrappedIndex($appMan, $this, "setday", 0, 3);
        }
    }

    function getName()
    {
        return $this->_name." - ".date("l dS \of F Y");
    }

    function getWindowCaption()
    {
        return $this->_name." - ".date("l dS \of F Y");
    }

    function getPanelCaption()
    {
        return $this->_name." - ".date("d/m/Y");
    }

    function getIconCaption()
    {
        return $this->_name;
    }

    function getDirectory()
    {
        return "tvguide";
    }

    function renderPanel(&$appMan)
    {
        return $this->getCurrentShows();
    }

    function getUserPane(&$appMan)
    {
        $currentUser = $appMan->getAuth()->getUser();
        if ($this->_user == "public") {
           $html .= "public ";
           $html.= $appMan->getAppletLink($this, "setuser", $currentUser, $currentUser);
        } else {
           $html.= $appMan->getAppletLink($this, "setuser", "public", "public");
           $html.= " $currentUser";
        }

        return $html;
    }

    function getDayPane(&$appMan)
    {
        $anchor = $appMan->getStyleText("prev");
        $html.= $appMan->getAppletLink($this, "setday", $this->_day==0?2:$this->_day-1, $anchor);

        $anchor = $appMan->getStyleText("next");
        $html.= $appMan->getAppletLink($this, "setday", $this->_day==2?0:$this->_day+1, $anchor);

        $u = TVGuide_Users::getUserGuide($this->_user);
        return $u->getDay($this->_day)->_date . $html;
    }
   
    function getCurrentShows() 
    {
        // We could also use  tvonnow default ... etc for this
        $u = TVGuide_Users::getUserGuide("public");
        $day = 0;	// currently, day[0] is always 'today' 
        $html = "Currently showing...<br/>";

        $stations = TVGuide_Channels::getChannelList();
        $attime = date("Gi");
        foreach($stations as $id => $name) {
           $prog = $u->getDay($day)->getProgrammeAt($id, $attime);
           if ($prog->_name != "") {
              $html .= "$name : <i>".$prog->_name."</i>";
              $html .= "<BR/>";
           }
        }
        return $html;
    }

    function renderWindow(&$appMan)
    {
        $html = "";

        $html .= "<div align=right>".$this->getUserPane($appMan)."</div>";
        $html .= $appMan->getMasterBarStart();

        $table = "";

        $stations = TVGuide_Channels::getChannelList();
        $u = TVGuide_Users::getUserGuide($this->_user);

        foreach($stations as $id => $name) {
           $shows = $u->getDay($this->_day)->getTableFor($id);
           $link = "<img width=32 src=".$this->getConfFileName("images/$id.png")." border=0><br>$name";
           if ($shows == "") {
              if (Warp_TVGuide_Conf::showAllChannels()) {
                 $html .= $appMan->getMasterBarEntry($link, "");
              }
           } else {
              $html .= $appMan->getMasterBarEntry($link, "#".$id);
              $table .= "<a name=$id>".$link;
              $table .= $shows;
           }
        }

        $html .= $appMan->getMasterBarEnd();

        $html .= "<div align=right>".$this->getDayPane($appMan)."</div>";

        $html .= $table;

        return $html;
    }

    function getRefreshParams(&$appMan)
    {
        return $appMan->getArgument($this, "user", $this->_user) . "&".
               $appMan->getArgument($this, "day", $this->_day);
    }

    function renderInfo(&$app_man, $fast=false)
    {
        if ($fast) {
            return "-";
        }

        return <<<EOD

<p>This uses crontab to download a selection of TV channels every night
and search them for specific keywords given by the various users. It then
builds a results table of all (potentially) interesting programmes for
the next three days. These are then accessible to the user through
the TV Guide web page.</p>
<p>You can change the list of search terms for the user 'steev', for example,
by editing the file <tt>/usr/local/minerva/etc/users/steev/tvsearch/list</tt>.
This should be a comma-separated list of terms. Leave this blank to get a list
of all the programmes on TV. If you include a file in this
directory called <tt>email</tt> then this list will also be emailed the user
specified in the file.
<p>Only Minerva users can receive TV guides, so ensure the user exists in
<tt>/usr/local/minerva/etc/users</tt>. New users can be added with the
<tt>addminervauser</tt> command. After adding a new user you must re-build the
web components for the Minerva applet, and manually start a search. This is
done by running the commands,</p>
<pre>
~minerva/conf/tv/make_web.sh
~minerva/update/tv/process_searches.sh
</pre>
<p>The configuration files are ultimated stored in
<tt>/var/www/minerva/warp/conf/tvguide</tt> and are all generated automatically
by the scripts above. You should never need to alter these.</p>
<p><i>Dependencies: </i>xsltproc</p>
<p><i>Future Plans: </i>Integration with the Minerva TV server to automatically record the selected shows.<br/></p>

EOD;
    }

}

?>

