<?php
require_once 'system/setup.conf';
require_once 'warp/warplib/applet.inc';
require_once 'warp/warplib/appletutils.inc';

require_once 'domhelper.inc';
require_once 'getdata.inc';

class Warp_Contacts_Applet extends Warp_Applet
{
var $snList;
var $show;
var $xpath;
var $valid;
var $personList;
var $errorMessage;

    function Warp_Contacts_Applet($caption = "Contact Information")
    {
        parent::__construct($caption);
        $this->errorMessage = "";
    }

    function prepare(&$appMan)
    {
        $user = $appMan->getAuth()->getUser();
        if ($user == "") {
           $user = "public";
        }

        $contactsFileName = getMinervaRoot()."/etc/users/$user/contacts.xml";

        $dom = new DOMDocument('1.0');
        $this->valid = @$dom->load($contactsFileName);
        if ($this->valid) {
           $this->xpath = new DOMXPath($dom);

           $this->snList = PersonalData::getSortedNameIndex($this->xpath);
           $this->personList = PersonalData::getPersonNodeList($this->xpath, $this->getAppletFileName("contacts/images/"));
           $this->show = $appMan->QueryParameter($this, "show");
        } else {
           $this->errorMessage = "There is an error in the XML '$contactsFileName'. Please fix.";
		}
    }

    function renderPanel(&$appMan) 
    {
        return "";
    }

    function renderWindow(&$appMan)
    {
        $this->prepare($appMan);

        if (!$this->valid) {
           return $this->errorMessage."\nNo contacts data...";
        }
        $main = "";
        $nameList = PersonalData::getNameList($this->xpath);

        //
        // The first bar is the alphabet, followed by 'all'
        //
        for ($i="A"; $i != "AA"; $i++) {
            $alpha.= $appMan->getAppletLink($this, "show", $i, "&nbsp;$i&nbsp;");
        }
        $alpha .= $appMan->getAppletLink($this, "show", "all", "All");

        $main.= "<div align=center>$alpha</div><hr/>";

        //
        // We then show all the individual names under this letter
        //
        $nameBar = $this->show;
        foreach($this->snList as $k=>$v) {
           if (strcasecmp($this->show,"all")==0 || strcasecmp($this->show,substr($v,0,1))==0) {
              $nameBar .= " - ";
              $nameBar .= "<a href=#n$k>";
              $nameBar .= $nameList[$k];
              $nameBar .= "</a>";
           }
        }
        $main.= "<div align=center>$nameBar</div><hr/>";

        if ($this->show == "") {
           $main .= "Select a letter to view contacts...";
        }

        //
        // And finally, the contact details, themselves
        //
        $contacts = "";
        foreach($this->snList as $k=>$v) {
           if (strcasecmp($this->show,"all")==0 || strcasecmp($this->show,substr($v,0,1))==0) {
              $contacts .= "<table border=0 cellspacing=0 width=90%>";
              $contacts .= "<tr><td colspan=3 bgcolor=#cccccc width=100%><font size=4>";
              $contacts .= "<a name='n$k'/>";
              $contacts .= $nameList[$k];
              $contacts .= "</font></td><td><a href=#top>^</a></td></tr>";
              $contacts .= $this->personList[$k];
              $contacts .= "</table>";
              $contacts .= "<br/>\n";
           }
        }

        $main .= $contacts;
        
        return $main;     
    }
}

?>

