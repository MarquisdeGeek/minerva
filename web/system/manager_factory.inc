<?php

class WAM_Factory
{
   public function create() {
      return new Warp_Applet_Manager();
   }
}

class WAM_FactoryTouch extends WAM_Factory {
   public function create() {
      return new Warp_Applet_ManagerTouch();
   }
}

?>

