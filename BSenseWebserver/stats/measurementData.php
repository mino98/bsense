<?php

//=== Include configuration values and functions:
require("../includes/config.inc.php");
require("../includes/functions.inc.php");

//=== Database connection:
global $db;
$db = @pg_connect("dbname=".$_config['databaseName']." user=".$_config['databaseUser']." password=".$_config['databasePass']);
if(!$db)
	die("<div class=\"error\"><strong>Internal error:</strong> code #1001.<br />
		Feel free to provide us feedback by using the form on the right. Thank you.</div>");

//=== Return json data of a specific measurement:

$MeasurementID = intval($_REQUEST['MeasurementID']);	
if($MeasurementID <= 0)
	die("ERROR: the supplied MeasurementID is not valid");

$sql = "SELECT 
			\"RawResultsUp\",
 			\"RawResultsDown\"
		FROM \"RawMeasurementData\"
		WHERE \"MeasurementID\" = '".addslashes($MeasurementID)."' 
		LIMIT 1;";
		
$details = pg_query($sql);
if(pg_num_rows($details) != 1)
	die("ERROR: measurement not found.");
	
$detail = pg_fetch_array($details);

//=== Parse the results:
switch($_REQUEST['type']) {
	
	//=== Latency graph: 
	case 'latency':
		$RawResultsUp = parseResults($detail['RawResultsUp']);
		
		$delay = array();
		$jitter = array();

		foreach($RawResultsUp as $row) {
			if($row['time'] > 59)
				break;
			
			$delay[] = array($row['time'], ($row['delay'] * 1000));
			$jitter[] = array($row['time'], ($row['jitter'] * 1000));
		}

		echo json_encode(
			array(
				array( label => 'Latency&nbsp;&nbsp;', data => $delay),
				array( label => 'Jitter', data => $jitter)
			)
		);
		break;

	//=== Latency graph: 
	case 'loss':
		$RawResultsUp = parseResults($detail['RawResultsUp']);

		$loss = array();

		foreach($RawResultsUp as $row) {
			if($row['time'] > 59)
				break;

			$loss[] = array($row['time'], $row['loss']);
		}

		echo json_encode(
			array(
				array( label => 'Packet loss', data => $loss )
			)
		);
		break;

	//=== Upstream graph: 
	case 'upstream':
		$RawResultsUp = parseResults($detail['RawResultsUp']);

		$bitrate = array();

		foreach($RawResultsUp as $row) {
			if($row['time'] < 60)
				continue;
				
			$bitrate[] = array(($row['time'] - 60), ($row['bitrate'] / 1000));
		}

		echo json_encode(
			array( label => 'Speed', data => $bitrate )
		);
		break;
		
	//=== Downstream graph: 
	case 'downstream':
		$RawResultsDown = parseResults($detail['RawResultsDown']);

		$bitrate = array();

		foreach($RawResultsDown as $row) {
			$bitrate[] = array($row['time'], ($row['bitrate'] / 1000));
		}

		echo json_encode(
			array( label => 'Speed', data => $bitrate )
		);
		break;
		
	default:	
		die("ERROR: data type not valid");
}

?>