<?php

/*
Run this file with:

php init.php

to add a number of useful/fun/arbitrary web sites to demonstrate a 
fully-working system
*/

require_once 'config.php';
require_once 'structs.php';
require_once 'database.php';

//new BookmarkLink("URL", "desc", "notes", "meta,tags")

$examples = array(
new BookmarkLink("http://www.bluedust.dontexist.com/", "Blue Dust", "The authors other main web site", "projects"),
new BookmarkLink("http://www.minervahome.net/", "Minerva", "Linux Home Automation for the masses", "ha,minerva"),
new BookmarkLink("http://www.warpdemo.homelinux.net/", "Warp Demo", "Running multiple web pages at once", "web2,minerva"),

new BookmarkLink("http://babelfish.yahoo.com/", "", "The de facto translation engine", "reference"),
new BookmarkLink("http://dictionary.com/", "", "Also includes thesarus", "reference"),
new BookmarkLink("http://londonnet.org", "", "", "life,london,reference"),
new BookmarkLink("http://london.randomness.org.uk/", "", "", "beer,reference,review,london"),


new BookmarkLink("http://www.instructables.com/id/The_Worlds_Most_Expensive_Chess_Set/", "A chess set made of money", "", "chess,make,money"),
new BookmarkLink("http://shop.lego.com/Product/?p=g678", "The official Lego chess set", "", "chess,lego"),
new BookmarkLink("http://www.brickshelf.com/cgi-bin/gallery.cgi?f=134313", "My Lego chess set", "", "chess,lego"),
new BookmarkLink("http//www.worle.com/chess/knight.htm#top", "Knights tour", "", "chess"),

new BookmarkLink("http://flickr.org", "", "Storing photos online.", "photo,web2"),
new BookmarkLink("http://www.flickr.com/photos/8564211@N04/", "", "The authors photostream, FWIW.", "photo,web2"),
new BookmarkLink("http://www.facebook.com", "", "", "social,web2"),

new BookmarkLink("http://blog.makezine.com", "Makezine blog", "My favourite daily waste of time", "fun,geek"),
new BookmarkLink("http://slashdot.org", "", "", "fun,geek"),
new BookmarkLink("http://www.dyndns.org", "", "Don't forget the 'www'", "tech"),

new BookmarkLink("http://xkcd.com", "", "If you don't get the jokes here, we can't be friends ;)", "humour"),
new BookmarkLink("http://thedailywtf.com/", "", "Curious perversions indeed.", "fun,geek"),
new BookmarkLink("http://userfriendly.org/", "", "", "fun,geek"),
new BookmarkLink("http://www.thedailymash.co.uk", "", "It's between this and the Onion", "humour"),
new BookmarkLink("http://graphjam.com", "", "", "humour"),
new BookmarkLink("http://zoomdoggle.com", "", "If you like Make, you'll like this", "fun,geek,make"),
new BookmarkLink("http://www.sciencecartoonsplus.com", "", "", "fun"),
new BookmarkLink("http://photoshopdisasters.blogspot.com/", "", "Artists can produce WTFs too...", "humour,fun"),

new BookmarkLink("http://bash.org", "", "", "geek,humour"),
new BookmarkLink("http://qdb.us", "", "", "geek,humour"),
new BookmarkLink("http://www.igetyourfail.blogspot.com/", "", "", "geek,humour,fun,fail"),
new BookmarkLink("http://www.failblog.org/", "", "", "geek,humour,fun,fail"),


new BookmarkLink("http://www.thebeercrate.com", "The Beer Crate", "The podcast for ale lovers the world over", "beer,reference,review,podcast"),
new BookmarkLink("http://www.beerintheevening.com", "", "", "beer,reference,review"),
new BookmarkLink("http://fancyapint.com", "", "", "beer,reference,review"),
new BookmarkLink("http://www.ratebeer.com/", "", "", "beer,reference"),
new BookmarkLink("http://beeradvocate.com/", "", "", "beer,reference"),
new BookmarkLink("http://www.craftbeerradio.com/", "", "", "beer,reference,podcast"),
new BookmarkLink("http://www.allaboutbeer.com", "", "", "beer,reference"),
new BookmarkLink("http://www.thepublican.com", "", "", "beer,reference"),
new BookmarkLink("http://www.camra.org.uk", "", "", "beer,reference"),

);

foreach($examples as $bm) {
	print "Adding ".$bm->url."\n";
	addBookmark(0, $bm);
}

?>

