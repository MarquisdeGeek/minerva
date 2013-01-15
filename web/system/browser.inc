<?php

class WebBrowser
{
var $factory;
var $width, $height;

  function WebBrowser($factory, $w = -1, $h = -1) {
    $this->factory = $factory;
    $this->width = $w;
    $this->height = $h;
  }

  function createManager() {
    return $this->factory->create();
  }

  function knownSize() {
     return $this->width != -1;
  }

  function getWidth() {
     return $this->width;
  }

  function getHeight() {
     return $this->height;
  }
}

?>

