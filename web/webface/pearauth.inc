<?php

class PearDB_Auth
{
    var $db_user;

    function setUser($user)
    {
        $this->db_user = $user;
        $this->CheckSession();
    }

    function isPurposeValid($purpose, $options)
    {
        $purpose = strtolower($purpose);
        $options = strtolower($options);

        if ($options == "all")  return 1;

        $allow = explode(",", $options);

        for($i=0;$i<count($allow);$i++)
        {
                $allow[$i] = strtolower($allow[$i]);
                if (trim($allow[$i]) == $purpose)
                        return 1;
        }

        return 0;
    }

    function isUserValidFor($purpose, $access="w")
    {
        $this->db_user = $_SERVER['PHP_AUTH_USER'];
        if ($user == "")
                return 0;


        # This is simplified logic.
        # Final version is:
        # 1. if purpose is forbidden (name preceeded with -) in user or group. FALSE
        # 2. if purpose is allows in either user of group. TRUE
        # 3. FALSE

        # NOTE: More work needed if additional modules are to be
        # supported. Security flaw if 'mp' module created, and
        # strpos finds mp3, and uses its rights.
        if ($access == "r") {
                if ($this->isPurposeValid($purpose, $_SESSION['usr_read'])) return 1;
                if ($this->isPurposeValid($purpose, $_SESSION['grp_read'])) return 1;
        }

        if ($access == "w") {
                if ($this->isPurposeValid($purpose, $_SESSION['usr_write'])) return 1;
                if ($this->isPurposeValid($purpose, $_SESSION['grp_write'])) return 1;
        }


        return true;
    }


function CheckSession() {
        # If we've filled the session return it. This caches
        # the data to save sql thrashing
        if ($_SESSION['group'] != "")
                return;

        $user = $_SERVER['PHP_AUTH_USER'];
        $link = mysql_connect();
        $query = 'SELECT users.usergroup, users.accessread, users.accesswrite FROM house.users WHERE users.name="'.$user.'"';
        $result = mysql_query($query);

        $rows = mysql_fetch_array($result, MYSQL_NUM);
        $_SESSION['group'] = $rows[0];
        $_SESSION['usr_read'] = $rows[1];
        $_SESSION['usr_write'] = $rows[2];
        mysql_free_result($result);

        $group = $_SESSION["group"];
        $query = 'SELECT groupnames.accessread, groupnames.accesswrite FROM house.groupnames WHERE groupnames.name="'.$group.'"';
        $result = mysql_query($query);
        $rows = mysql_fetch_array($result, MYSQL_NUM);
        $_SESSION['grp_read'] = $rows[0];
        $_SESSION['grp_write'] = $rows[1];
        mysql_free_result($result);

        mysql_close($link);
}

}

?>

