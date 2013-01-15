<?php

function getMovieID($id) {
	if (preg_match('/^\d+$/', $id)) {
		return $id;
	}

	require_once("imdbsearch.class.php");
	require_once("imdb.class.php");

	$search = new imdbsearch();
	$search->search_episodes(FALSE);
	
	$search->setsearchname($id);
	return processSearch($search);
}

function getPersonID($id) {
	if (preg_match('/^\d+$/', $id)) {
		return $id;
	}

	require_once("imdbsearch.class.php");
	require_once("imdb_person.class.php");

	$search = new imdbpsearch();
	$search->setsearchname($id);

	return processSearch($search);
}

function processSearch(&$search) {
	$results = $search->results();
	
	if (is_array($results)) {
		$id = $results[0]->imdbID;
	} else {
		$id = $results->imdbID;
	}

	return $id;
}

?>
