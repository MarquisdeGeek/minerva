<?php

$duration = (int)($_GET['d']);

shell_exec("/usr/local/minerva/bin/ctimer default start $duration > /dev/null &");


sleep($duration * 60);

print "The timer has expired. It lasted $duration minutes<br/>";

?>


