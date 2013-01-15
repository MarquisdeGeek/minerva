<?php

function getSunRise() {
  return exec("/usr/local/minerva/bin/sunrise");
}

function getSunSet() {
  return exec("/usr/local/minerva/bin/sunset");
}

?>
