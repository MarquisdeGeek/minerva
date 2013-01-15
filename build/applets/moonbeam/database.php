<?php

function get_db_conn() {
global $g_Connection;
global $db_ip, $db_user, $db_pass, $db_name;
	if ($g_Connection != NULL) {
		return $g_Connection;
	}
	
	$conn = mysql_connect($db_ip, $db_user, $db_pass);
	mysql_select_db($db_name, $conn);
	$g_Connection = $conn;
	
	return $conn;
}

function parseMetaList($metaString) {
	if ($metaString == "") {
		return array();
	}
	$metaList = split(",", $metaString);
	return $metaList;
}

function getMetaTag($metaTag) {
    try {
		$conn = get_db_conn();

		$result = mysql_query("SELECT id FROM metatags WHERE metatag='$metaTag'", $conn);
		if (mysql_affected_rows() != 0) {
			$row = mysql_fetch_assoc($result);
			return $row['id'];
		}
    } catch (Exception $e) {
        error_log($e->getMessage()." after calling ".mysql_error());
    }
	return NULL;
}

function addMetaTag($metaTag) {
	$id = 0;

    try {
		$conn = get_db_conn();

		// First, check we don't have it already
		$id = getMetaTag($metaTag);
		if ($id == NULL) {
			mysql_query("INSERT INTO metatags (metatag) VALUES ('$metaTag');", $conn);
			$id = mysql_insert_id();
		}
    } catch (Exception $e) {
        error_log($e->getMessage()." after calling ".mysql_error());
    }
	return $id;
}

/*
   This converts the array of meta names, into an array of meta integer
   IDs for use elsewhere. If the names don't exist, they are automatically
   added to the list.
*/
function processMetaList($metaList) {
	$meta_result = array();
    try {
		$conn = get_db_conn();
		foreach($metaList as $m) {
			$tagId = addMetaTag($m);
			if ($tagId != 0) {
				$meta_result[$tagId] = $m;
			}
        }
    } catch (Exception $e) {
        error_log($e->getMessage()." after calling ".mysql_error());
    }
	return $meta_result;
}


function addBookmark($userID, $bm) {
	return addUrl($userID, $bm->url, $bm->description, $bm->notes, $bm->metaList);
}

function addUrl($userID, $url, $description, $notes, $metaList) {
	try {
		$conn = get_db_conn();

		$url = "'".mysql_real_escape_string($url)."'";
		$description = "'".mysql_real_escape_string($description)."'";
		$notes = "'".mysql_real_escape_string($notes)."'";

		mysql_query("INSERT INTO links (uid, url, description, notes) VALUES ($userID, $url, $description, $notes);", $conn);

		$linkID = mysql_insert_id();
		
		if (is_array($metaList)) {
			$metaArray = $metaList;
		} else {
			$metaArray = processMetaList(parseMetaList($metaList));
		}
		foreach($metaArray as $mid=>$mname) {
			$result = mysql_query("INSERT INTO metamap (meta_id,user_id,link_id) VALUES ($mid, $userID, $linkID);");
		}
	} catch (Exception $e) {
		error_log($e->getMessage()." after calling ".mysql_error());
	}
}

function getMetaList() {
	$ml = array();
	try {
		$conn = get_db_conn();
		// we coule SELECT DISTINCT, but we ensure unique tags are added
		// so it's not necessary.
		$result = mysql_query("SELECT id,metatag FROM metatags", $conn);
		while ($row = mysql_fetch_assoc($result)) {
			$ml[$row['id']] = $row['metatag'];
		}
	} catch (Exception $e) {
		error_log($e->getMessage()." after calling ".mysql_error());
	}
	return $ml;
}

function getBookmark($userID = 0, $withTagList = NULL, $excludeTagList = NULL) {
	$bookmarks = array();

    try {
		$conn = get_db_conn();
		$where = "";
		$notin = "";

		if ($withTagList != NULL) {
			$where.= " && links.id=metamap.link_id && (";
 			$add = "";
			foreach($withTagList as $tid) {
				$where .= $add."metamap.meta_id=$tid";
				$add = " && ";
			}
			$where .= ")";
		}
		if ($excludeTagList != NULL && count($excludeTagList) != 0) {
			$notin = " &&  metamap.link_id NOT IN (SELECT link_id FROM metamap WHERE ";
			$add = "";

			foreach($excludeTagList as $tid=>$ton) {
				if ($ton == 0) {
					$notin .= $add." metamap.meta_id=$tid";
					$add = " || ";
				}
			}
			$notin .= ")";
			if ($add == "") {
				$notin = "";
			}

		}
		$sql = "SELECT DISTINCT links.* FROM links,metamap WHERE uid=$userID $where $notin";
		$result = mysql_query($sql, $conn);
		if ($result != NULL) {
		while ($row = mysql_fetch_assoc($result)) {
				$bkm = new BookmarkLink($row['url'], $row['description'], $row['notes'], "");
       			$bkm->metaList = array();

				$linkID = $row['id'];
				$metaResult = mysql_query("SELECT * FROM metamap WHERE user_id=$userID AND link_id=$linkID;", $conn);

				while ($meta_row = mysql_fetch_assoc($metaResult)) {
					$bkm->metaList[] = $meta_row['meta_id'];
				}
				$bookmarks[] = $bkm;
			}
		}
	} catch (Exception $e) {
		error_log($e->getMessage()." after calling ".mysql_error());
	}
    return $bookmarks;
}

?>

