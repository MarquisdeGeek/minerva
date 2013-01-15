<?php

class MetaTag {
  public $id;
  public $tag;
}

class BookmarkLink {
  public $url;
  public $description;
  public $notes;
  public $metaList;

	public function BookmarkLink($u, $d, $n, $m) {
		$this->url = $u;
		$this->description = $d;
		$this->notes = $n;
		$this->metaList = processMetaList(parseMetaList($m));
	}

	public static function parsePost() {
		$bml = new BookmarkLink($_POST['url'], $_POST['desc'], $_POST['notes'], $_POST['meta']);
		if ($bml->meta == "") $bml->meta = "unassigned";
		return $bml;
	}

	public static function parseGet() {
		$bml = new BookmarkLink($_GET['url'], $_GET['desc'], $_GET['notes'], $_GET['meta']);
		if ($bml->meta == "") $bml->meta = "unassigned";
		return $bml;
	}
}

?>

