<?php
$releaseType="Debug";
// $releaseType="Release";

if ($releaseType == "Release") {
	$dbhost = 'localhost';
	$dbusername = 'setusername';
	$dbuserpassword = 'setuserpassword';
	$default_dbname = 'greproject';
	error_reporting(0);
}
else {
	$dbhost = 'localhost';
	$dbusername = 'root';
	$dbuserpassword = 'contest';
	$default_dbname = 'GREdict';
	// Enable error reporting
	error_reporting(E_ALL);
	ini_set('display_errors', '1');
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
