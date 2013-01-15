<?php
include "recipehelper.conf";

$cmd = $_GET["cmd"];
$book = $_GET["book"];
$recipe = $_GET["recipe"];

$recipeBook = "/usr/local/minerva/conf/recipes/$book.xml";

$xpath = Warp_RecipeHelper::init($recipeBook);

if ($recipe != "") {

   $idx = 0;
   foreach ($xpath->query('//recipe') as $node) {
      $html = Warp_RecipeHelper::recipeGetData($node);
      if ($idx == $recipe) {
        break;
      }
      ++$idx;
   }

} else if ($book != "") {

   $idx = 0;
   foreach ($xpath->query('//recipe') as $node) {
      $recipe = Warp_RecipeHelper::recipeGetName($node);
      $html .= "<a href=javascript:readrecipe($idx)>$recipe</a><br/>";
      ++$idx;
   }
}

print $html;
?>


