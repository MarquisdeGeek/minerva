<?php

/*
Notes:
If you plan on using this interface, you will need to add the following
configuration element to your apache2 set-up.


<Directory /var/www/sites/homecontrol/minerva/control>
Options +FollowSymLinks
IndexIgnore * / *
# Turn on the RewriteEngine
RewriteEngine On
#  Rules
RewriteCond %{REQUEST_FILENAME} !-f
RewriteCond %{REQUEST_FILENAME} !-d
RewriteRule . index.php

 </Directory>
*/


$requestURI = explode("/", $_SERVER['REQUEST_URI']);
print_r($requestURI);
print "user: ".$_SERVER['REMOTE_USER'];
$usr = $_SERVER['PHP_AUTH_USER']; 
echo "Hello " . $usr; 
?>

