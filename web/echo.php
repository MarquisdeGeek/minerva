<?php

$rt.= "Get:\n";
$rt.= print_r($_GET, TRUE);

$rt.= "Post:\n";
$rt.= print_r($_POST, TRUE);
 //print "phone:".$_POST['from'];

file_put_contents( 'log.txt', file_get_contents( 'log.txt' ) . $rt );

if ($_POST['from'] == "012345678") {
  if ($_POST['text'] == "bedroom on") {
     system("/usr/local/bin/heyu turn studio on");
  } else if ($_POST['text'] == "bedroom off") {
     system("/usr/local/bin/heyu turn studio off");
  }
}

$command = "/usr/local/minerva/bin/msgrcv sms ";
$command.= $_POST['from'];
$command.= " ";
$command.= $_POST['text'];

echo $command;

system($command);
?>

