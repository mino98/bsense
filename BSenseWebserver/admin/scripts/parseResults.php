<?php

// This script writes the values in the ParsedMeasurementData table for the experiments that have
// not been parsed yet.

// Include the functions file:
include("/var/www/broadbandforall.net/includes/functions.inc.php");

// Function for the RIPE lookup:
require("/var/www/broadbandforall.net/includes/ripeWhois.php");

// Database connection:
global $db;
$db = @pg_connect("dbname=broadbandforall user=broadbandforall password=bro72gd92rffg");
if(!$db)
	die("Internal error: cannot connect to the database.");

			
//==== STEP 1:
//==== RESULTS PARSING
$sql = "SELECT 
			u.\"OperatorName\",
			r.\"ClientIp\",
			r.\"RawResultsUp\",
			r.\"RawResultsDown\",
			r.\"MeasurementID\"
		FROM \"RawMeasurementData\" AS r
		INNER JOIN \"BroadbandUsers\" AS u ON r.\"UUID\"=u.\"UUID\"
		WHERE \"MeasurementID\" NOT IN (
			SELECT \"MeasurementID\" 
			FROM \"ParsedMeasurementData\"
		)";
$measurements = pg_query($sql);

echo "Step 1:\n";
echo "/=========================================================================================================================\\\n";
echo "MeasurementID:\tUploadSpeed:\tDownloadSpeed:\tLatency:\tJitter:\t\tPacketLoss:\n";

$i = 0;
$whoisCache = array();
while($measurement = pg_fetch_array($measurements)) {
	$i++;
	$valid = 'false'; 
	$whois = null;
	echo $measurement['MeasurementID']."\t\t";

	//=== Whois of the Client IP
	
	// If not in cache:
	if(isset($whoisCache[$measurement['ClientIp']])) {
		$whois = $whoisCache[$measurement['ClientIp']];
	} else {
		$retries = 3;
		while(!$whois && $retries > 0) {
			$whois = doRISLookup($measurement['ClientIp']);
			$retries--;
		}
		if(!$whois) {
			echo "Cannot connect to whois server, skipping...\n";
			continue; // skip if we couldn't connect with the RIPE server after $retries attempts
		}

		// Cache the value for now:
		$whoisCache[$measurement['ClientIp']] = $whois;
	}

	//=== Check whether the AS of the Client IP matches those of the ISP that the user had declared:
	switch($measurement['OperatorName']) {
		case 'BT':
			if($whois['AS'] == 2856) $valid = 'true'; break;
			
		case 'Scotnet':
			if($whois['AS'] == 28757) $valid = 'true'; break;

		case 'Avanti':
			if($whois['AS'] == 39356) $valid = 'true'; break;

		case 'Scotnet':
			if($whois['AS'] == 2856) $valid = 'true'; break;

		case 'Tegola':
			if($whois['AS'] == 786) $valid = 'true'; break;

		case 'AOL':
			if($whois['AS'] == 43234) $valid = 'true'; break;

		case 'Demon':
			if($whois['AS'] == 2529) $valid = 'true'; break;

		case 'Virgin Media':
			if($whois['AS'] == 5089) $valid = 'true'; break;

		case 'Sky':
			if($whois['AS'] == 5607) $valid = 'true'; break;

		case 'Orange':
			if($whois['AS'] == 12576) $valid = 'true'; break;

		case 'Other':
			$valid = 'true'; break;

		default:
			$valid = 'false';
	}
	
	//=== DOWN:
	$resultsTable = parseResults($measurement['RawResultsDown']);
	
	$DownloadSpeed = parseDownloadSpeed($resultsTable);
	echo number_format($DownloadSpeed, 3)."\t\t";

	//=== UP:
	$resultsTable = parseResults($measurement['RawResultsUp']);

	$UploadSpeed = parseUploadSpeed($resultsTable);
	echo number_format($UploadSpeed, 3)."\t\t";

	$Latency = parseLatency($resultsTable);
	echo number_format($Latency, 3)."\t\t";

	$Jitter = parseJitter($resultsTable);
	echo number_format($Jitter, 3)."\t\t";

	$PacketLoss = parsePacketLoss($resultsTable);
	echo number_format($PacketLoss, 3)."\t\t";
	
	//=== If speed is above the offered throughput, mark the results as not valid:
	if(	$UploadSpeed <= 0 ||
		$UploadSpeed > 30 || 
		$DownloadSpeed <= 0 ||
		$DownloadSpeed > 30
	  )
		$valid = 'false';

	//=== Write to SQL:
	$sql = "INSERT INTO \"ParsedMeasurementData\" ( 
				\"UploadSpeed\", 
				\"DownloadSpeed\",
				\"Latency\",
				\"Jitter\",
				\"PacketLoss\",
				\"MeasurementID\",
				\"AS\",
				\"Description\",
				\"Valid\"
			) VALUES (
				'".addslashes($UploadSpeed)."',
				'".addslashes($DownloadSpeed)."',
				'".addslashes($Latency)."',
				'".addslashes($Jitter)."',
				'".addslashes($PacketLoss)."',
				'".$measurement['MeasurementID']."',
				'".addslashes($whois['AS'])."',
				'".addslashes($whois['Description'])."',
				'".addslashes($valid)."'
			);";
	pg_query($sql);
	
	echo "\n";
}
echo "\\=========================================================================================================================/\n";
echo "Done: $i experiments analysed.\n\n";


//==== STEP 2:
//==== UPDATING USER STATISTICS
echo "Step 2:\n";
echo "/=========================================================================================================================\\\n";
echo "UUID:\t\t\t\t\tUploadSpeed:\tDownloadSpeed:\tLatency:\tJitter:\t\tPacketLoss:\n";

$sql = "SELECT *
		FROM \"BroadbandUsers\"
		ORDER BY \"TechnologyName\" ASC, \"OperatorName\" ASC;";
$users = pg_query($sql);

$i = 0;
while($user = pg_fetch_array($users)) {
	$i++;
	echo $user['UUID']."\t";
	
	//=== How many successful tests:
	$sql = "SELECT count(*)
			FROM \"RawMeasurementData\"
			WHERE \"UUID\"='".addslashes($user['UUID'])."'";
	$rows = pg_query($sql);
	$row = pg_fetch_array($rows);
	$SuccessfulTestsNumber = $row[0];
	
	//=== Last successful test:
	$sql = "SELECT \"Timestamp\"
			FROM \"RawMeasurementData\"
			WHERE \"UUID\"='".addslashes($user['UUID'])."'
			ORDER BY \"Timestamp\" DESC
			LIMIT 1;";
	$rows = pg_query($sql);
	$row = pg_fetch_array($rows);
	$SuccessfulTestsLast = ($row[0] == "") ? 'null' : "'".addslashes($row[0])."'";
		
	//=== How many failures:
	$sql = "SELECT count(*)
			FROM \"FailureLogs\"
			WHERE \"UUID\"='".addslashes($user['UUID'])."'";
	$rows = pg_query($sql);
	$row = pg_fetch_array($rows);
	$FailuresNumber = $row[0];
	
	//=== Last failure:
	$sql = "SELECT \"Timestamp\"
			FROM \"FailureLogs\"
			WHERE \"UUID\"='".addslashes($user['UUID'])."'
			ORDER BY \"Timestamp\" DESC
			LIMIT 1;";
	$rows = pg_query($sql);
	$row = pg_fetch_array($rows);
	$FailuresLast = ($row[0] == "") ? 'null' : "'".addslashes($row[0])."'";
		
	//=== Aggregate experiments results:
	$UploadSpeeds = array();
	$DownloadSpeeds = array();
	$Latencies = array();
	$Jitters = array();
	$PacketLosses = array();

	$sql = "SELECT p.*, r.\"DownstreamExperimentID\", r.\"UpstreamExperimentID\"
			FROM \"ParsedMeasurementData\" AS p
			INNER JOIN \"RawMeasurementData\" AS r ON p.\"MeasurementID\"=r.\"MeasurementID\"
			WHERE r.\"UUID\"='".addslashes($user['UUID'])."'
			AND p.\"Valid\" = 'true'";
	$rows = pg_query($sql);
	while($row = pg_fetch_array($rows)) {
		$UploadSpeeds[] = $row['UploadSpeed'];
		$DownloadSpeeds[] = $row['DownloadSpeed'];
		$Latencies[] = $row['Latency'];
		$Jitters[] = $row['Jitter'];
		$PacketLosses[] = $row['PacketLoss'];
	}

	$UploadSpeed = percentile($UploadSpeeds, 0.5);
	$DownloadSpeed = percentile($DownloadSpeeds, 0.5);
	$Latency = percentile($Latencies, 0.5);
	$Jitter = percentile($Jitters, 0.5);
	$PacketLoss = percentile($PacketLosses, 0.5);

	echo number_format($UploadSpeed, 3)."\t\t".
		number_format($DownloadSpeed, 3)."\t\t".
		number_format($Latency, 3)."\t\t".
		number_format($Jitter, 3)."\t\t".
		number_format($PacketLoss, 3);


	//=== Update database:
	$sql = "UPDATE \"BroadbandUsers\"
			SET 
				\"SuccessfulTestsNumber\"=".$SuccessfulTestsNumber.",
				\"SuccessfulTestsLast\"=".$SuccessfulTestsLast.",
				\"FailuresNumber\"=".$FailuresNumber.",
				\"FailuresLast\"=".$FailuresLast.",
				\"UploadSpeed\"=".$UploadSpeed.",
				\"DownloadSpeed\"=".$DownloadSpeed.",
				\"Latency\"=".$Latency.",
				\"Jitter\"=".$Jitter.",
				\"PacketLoss\"=".$PacketLoss."
			WHERE
				\"UUID\"='".addslashes($user['UUID'])."'";
	pg_query($sql);
	echo "\n";
}
echo "\\=========================================================================================================================/\n";
echo "Done: $i users udpated.\n";

pg_close($db);

?>
