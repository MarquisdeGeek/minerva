<?php

  class Programme {
    var $_name;
    var $_description;
    var $_timeOn;
    var $_timeOff;
    var $_channel;

    function Programme($n,$d,$o,$f,$c) {
       $this->_name = $n;
       $this->_description = $d;
       $this->_timeOn = $o;
       $this->_timeOff = $f;
       $this->_channel = $c;
    }

    function timeOn() {
       return substr($this->_timeOn,0, 2) . ":" . substr($this->_timeOn, 2, 2);
    }

    function timeOff() {
       return substr($this->_timeOff,0, 2) . ":" . substr($this->_timeOff, 2, 2);
    }

    function getTableRow() {
       return "<tr valign=top><td>".$this->timeOn()."</td><td>".$this->timeOff()."</td><td width=30%><i>$this->_name</i></td><td>$this->_description</td></tr>";
    }

  }
  class DaysProgramming {
    var $_day, $_month, $_year;
    var $_list;

    function DaysProgramming() {
       $this->_list = array();
    }
  
    function add($prog) {
       $this->_list[] = $prog;
    }

    function getProgrammeAt($chn, $attime) {
       for($i=0;$i<count($this->_list);++$i) {
          $prog = $this->_list[$i];
          if ($prog->_channel == $chn) {
             if ($attime >= $prog->_timeOn && $attime <= $prog->_timeOff) {
                return $prog;
             }
          }
       }
       return NULL;
    }

    function getShortList($chn, $attime="") {
       $code = "";

       for($i=0;$i<count($this->_list);++$i) {
          $prog = $this->_list[$i];
          if ($prog->_channel == $chn) {
             if ($attime == "" || ($attime >= $prog->_timeOn && $attime <= $prog->_timeOff)) {
                $code .= $prog->timeOn()."-".$prog->_name.".";
             }
          }
       }
       return $code;
    }

    function getTableFor($chn, $attime="") {
       $code = "";

       for($i=0;$i<count($this->_list);++$i) {
          $prog = $this->_list[$i];
          if ($prog->_channel == $chn) {
             if ($code == "") {
                $code = "<table class=tvguide>";
             }
             if ($attime == "" || ($attime >= $prog->_timeOn && $attime <= $prog->_timeOff)) {
                $code .= $prog->getTableRow();
             }
          }
       }
       if ($code != "") {
          $code .= "</table>";
       }
       return $code;
    }
  }

class UserProgrammes {

  var $_day = array();

  function getToday() { return $this->_day[0]; }

  function getDay($idx) {
     switch($id) {
        case 0:
        case 1:
        case 2: return $this->_day[$idx]; 
        default: return null;
     }
  }

}

?>
