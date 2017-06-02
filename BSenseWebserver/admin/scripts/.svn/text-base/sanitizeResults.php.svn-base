<?php

// Include the functions file:
include("../../includes/functions.inc.php");


die("THIS SCRIPT IS OBSOLETE. mino");

// Database connection:
global $db;
$db = @pg_connect("dbname=broadbandforall user=broadbandforall password=bro72gd92rffg");
if(!$db)
	die("<div class=\"error\"><strong>Internal error:</strong> code #1001.<br />
		Feel free to provide us feedback by using the form on the right. Thank you.</div>");


echo "<pre>";
			
$sql = "SELECT 
			\"RawResultsUp\",
			\"RawResultsDown\",
			\"MeasurementID\"
		FROM \"RawMeasurementData\"";
$measurements = pg_query($sql);


while($measurement = pg_fetch_array($measurements)) {
	echo $measurement['MeasurementID'].": ";

	$ret = null;	
	$ret = sanitizeResults($measurement['RawResultsUp']);
	if(!$ret)
		echo "Up ERROR - ";
	else {
		$sql = "UPDATE \"RawMeasurementData\"
				SET \"RawResultsUp\" = '".addslashes($ret)."'
				WHERE \"MeasurementID\" = '".$measurement['MeasurementID']."';";
		pg_query($sql);
		echo "Up OK - ";
	}

	$ret = null;
	$ret = sanitizeResults($measurement['RawResultsDown']);
	if(!$ret)
		echo "Down ERROR\n";
	else {
		$sql = "UPDATE \"RawMeasurementData\"
				SET \"RawResultsDown\" = '".addslashes($ret)."'
				WHERE \"MeasurementID\" = '".$measurement['MeasurementID']."';";
		pg_query($sql);
		echo "Down OK\n";
	}
}

echo "</pre>";
pg_close($db);



?>