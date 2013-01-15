<?php

class Top_Process
{
    var $pid, $user, $pri, $ni, $size, $rss, $share, $stat;
    var $cpu, $mem, $time, $cmd;
    var $_mul; // _multiplier, is -1 to reverse the sort algorithm
    
    function Top_Process()
    {
        static $rev=-1;
        
        $this->_mul =& $rev;
    }
    
    // Generic sort variable
    function setReverseSort($r)
    {
        $this->_mul = $r ? 1 : -1;
    }
    
    // Generic comparison functions
    function compareNum($a, $b)
    {
        if ($a == $b) {
            return 0;
        }
        return($a < $b) ? -1 : 1;
    }
    
    function compareText($a, $b)
    {
        $al = strtolower($a);
        $bl = strtolower($b);
        if ($al == $bl) {
            return 0;
        }
        return($al < $bl) ? -1 : 1;
    }
    
    
    // The individual comparison functions
    function comparePID($a, $b)
    {
        return Top_Process::compareNum($a->pid, $b->pid) * $a->_mul;
    }
    
    function compareUser($a, $b)
    {
        return Top_Process::compareText($a->user, $b->user) * $a->_mul;
    }
    
    function comparePri($a, $b)
    {
        return Top_Process::compareNum($a->pri, $b->pri) * $a->_mul;
    }
    
    function compareNice($a, $b)
    {
        return Top_Process::compareNum($a->ni, $b->ni) * $a->_mul;
    }
    
    function compareSize($a, $b)
    {
        return Top_Process::compareNum($a->size, $b->size) * $a->_mul;
    }
    
    function compareShare($a, $b)
    {
        return Top_Process::compareNum($a->share, $b->share) * $a->_mul;
    }
    
    function compareMem($a, $b)
    {
        return Top_Process::compareNum($a->mem, $b->mem) * $a->_mul;
    }
    
    function compareTime($a, $b)
    {
        return Top_Process::compareNum($a->time, $b->time) * $a->_mul;
    }
    
    function compareCPU($a, $b)
    {
        return Top_Process::compareNum($a->cpu, $b->cpu) * $a->_mul;
    }
    
    function compareStat($a, $b)
    {
        return Top_Process::compareText($a->stat, $b->stat) * $a->_mul;
    }
    
    function compareCmd($a, $b)
    {
        return Top_Process::compareText($a->cmd, $b->cmd) * $a->_mul;
    }
    
    
}


class Top
{
    var $load = array();
    var $tasks = array();
    var $cpu = array();
    var $mem = array();
    var $swap = array();
    var $processes = array();
    var $valid_users = array();
    var $forbidden_users = array();
    var $sort;
    
    // If no valid users are given, we assume all users' tasks
    // may be shown
    function clearValidUsers()
    {
        $this->valid_users = array();
    }
    
    function clearFobiddenUsers()
    {
        $this->forbidden_users = array();
    }
    
    function addValidUser($user)
    {
        $this->valid_users[] = $user;
    }
    
    function addForbiddenUser($user)
    {
        $this->forbidden_users[] = $user;
    }
    
    function poll()
    {
        // We add these arguments since we can't
        // parse the output without.
        $cmd = "top -n 1 -b";
        $res = shell_exec($cmd);
        
        // Create regexps for the main summary portion
        $regexp_load = '/load average:\s+(.*?), (.*?), (.*)/i';
        $regexp_tasks = '/(\d+)\s+processes:\s+(\d+)\s+sleeping,\s+(\d+)\s+running,\s+(\d+)\s+zombie,\s+(\d+)\s+stopped/i';
        $regexp_cpu = '/Cpu(\d?).*:\s+(.*?)\% user,\s+(.*?)\% system,\s+(.*?)\% nice,\s+(.*?)\% idle/i';
        $regexp_mem = '/Mem:\s+(.*?)k total,\s+(.*?)k used,\s+(.*?)k free,\s+(.*?)k buffers/i';
        $regexp_swap = '/Swap:\s+(.*?)k total,\s+(.*?)k used,\s+(.*?)k free,\s+(.*?)k cached/i';
        
        // Now parse the commands output
        $parse_processes = false;
        $arr = explode("\n", $res);
        foreach($arr as $line)
        {
            if ($parse_processes && $line!="") {
                // remove initial spaces on line
                $line = trim($line);
                $process = preg_split("/[\s<]+/", $line);
                
                $p = new Top_Process;
                $p->pid   = intval($process[0]);
                $p->user  = $process[1];
                $p->pri   = intval($process[2]);
                $p->ni    = intval($process[3]);
                $p->size  = intval($process[4]);
                $p->rss   = intval($process[5]);
                $p->share = intval($process[6]);
                $p->stat  = $process[7];
                $p->cpu   = (float)($process[8]);
                $p->mem   = (float)($process[9]);
                $p->time  = $process[10];
                
                $p->cmd   = "";
                for ($num=11; $num<count($process); $num++) {
                    $p->cmd.= $process[$num]." ";
                }
                
                if (in_array($p->user, $this->forbidden_users)) {
                    // null
                } else if (count($this->valid_users) == 0 ||
                in_array($p->user, $this->valid_users)) {
                    $this->processes[] = $p;
                }
                
            } else // parse stats
            {
                if (strstr($line, "PID")) {
                    $parse_processes = true;
                } else if (preg_match($regexp_load, $line, $m)) {
                    $this->load = $m;
                } else if (preg_match($regexp_tasks, $line, $m)) {
                    $this->tasks = $m;
                } else if (preg_match($regexp_cpu, $line, $m)) {
                    $this->cpu = $m;
                } else if (preg_match($regexp_mem, $line, $m)) {
                    $this->mem = $m;
                } else if (preg_match($regexp_swap, $line, $m)) {
                    $this->swap = $m;
                }
                
            }
        }
        
    }
    
    function sortBy($sort)
    {
        switch ((int)($sort / 10)) {
        case	0:
            $sp = "comparePID";
            break;
        case	1:
            $sp = "compareUser";
            break;
        case	2:
            $sp = "comparePri";
            break;
        case	3:
            $sp = "compareNice";
            break;
        case	4:
            $sp = "compareSize";
            break;
        case	5:
            $sp = "compareRss";
            break;
        case	6:
            $sp = "compareShare";
            break;
        case	7:
            $sp = "compareStat";
            break;
        case	9:
            $sp = "compareMem";
            break;
        case	10:
            $sp = "compareTime";
            break;
        case	11:
            $sp = "compareCmd";
            break;
            default:
        case	8:
            $sp = "compareCPU";
            break;
        }
        
        $this->processes[0]->setReverseSort(($sort & 1)?true:false);
        uasort($this->processes, array("Top_Process", $sp));
    }
    
}

?>

