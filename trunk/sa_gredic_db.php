<?php
/*// Enable error reporting
error_reporting(E_ALL);
ini_set('display_errors', '1');*/

//error_reporting(0);
$releaseType="Debug";
// $releaseType="Release";

if ($releaseType == "Release") {
	$dbhost = 'localhost';
	$dbusername = 'atique_atique';
	$dbuserpassword = 'xtreme_2306';
	$default_dbname = 'atique_greproject';
}
else {
	$dbhost = 'localhost';
	$dbusername = 'root';
	$dbuserpassword = 'contest';
	$default_dbname = 'GREdict';
}

function db_connect() {
	global $dbhost, $dbusername, $dbuserpassword, $default_dbname;
	global $MYSQL_ERRNO, $MYSQL_ERROR;

	$link_id = mysql_connect($dbhost, $dbusername, $dbuserpassword);
	if ( !$link_id) {
		$MYSQL_ERRNO = 0;
		$MYSQL_ERROR = "Connect failed to the host $dbhost.";
		return 0;
	}
	else if (empty($dbname) && !mysql_select_db($default_dbname)) {
		$MYSQL_ERRNO = mysql_errno();
		$MYSQL_ERROR = mysql_error();
		return 0;
	}
	else
		return $link_id;
}

function sql_error() {
	global $MYSQL_ERRNO, $MYSQL_ERROR;

	if (empty ($MYSQL_ERROR)) {
		$MYSQL_ERRNO = mysql_errno();
		$MYSQL_ERROR = mysql_error();
	}
	return "$MYSQL_ERRNO: $MYSQL_ERROR";
}

?>
