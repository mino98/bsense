<?php
header("Content-type: text/plain");

//=== Includes the configuration parameters and additional functions:
global $_config;
require("../includes/config.inc.php");
include("../includes/functions.inc.php");

//=== Database connection:
global $_db;
$_db = @pg_connect("dbname=".$_config['databaseName']." user=".$_config['databaseUser']." password=".$_config['databasePass']);
if(!$_db)
	error("connection to the DB failed.");
	
//=== Let's select the action to be followed:
$a = trim(strtolower($_REQUEST['a']));
switch($a) {
	case "registeruser":	
		registerUser(); 
		@pg_close($_db);
		break; 	

	case "updateemail":	
		updateEmail(); 
		@pg_close($_db);
		break;

	case "upstreamparameters":	
		upstreamParameters(); 
		@pg_close($_db);
		break;

	case "downstreamparameters":	
		downstreamParameters(); 
		@pg_close($_db);
		break;

	case "saveresults":	
		saveResults(); 
		@pg_close($_db);
		break;

	case "savefailures":	
		saveFailures(); 
		@pg_close($_db);
		break;
			
	default:
		error("action not valid.");
}
	

/**
 * registerUser: save a new user in database
 */
function registerUser() {
	global $_db, $_config;

	// Verify if a valid postcode was submitted:	
	$Postcode = strtoupper(trim(stripslashes($_REQUEST['Postcode'])));
	$Postcode = str_replace(' ', '', $Postcode);

	if(empty($Postcode) || !preg_match('/^[A-Z]{1,2}[0-9R][0-9A-Z]?[0-9][A-Z]{2}$/', $Postcode) )
		error("Please enter a valid postcode.");

	// Verify if the postcode is from Scotland:
	$sql = "SELECT \"Postcode\"
		FROM \"Postcodes\"
		WHERE \"Postcode\"='".addslashes($Postcode)."';";
	$results = pg_query($_db, $sql);
	if(pg_num_rows($results) == 0)
		error("The postcode you provided doesn't seem to be located in Scotland.");
	elseif(pg_num_rows($results) > 1)
		error("Query failed.");

	// Parse and verify input data:
	$OperatorName = trim(stripslashes($_REQUEST['OperatorName']));
	if(empty($OperatorName))
		error("Please select your Operator from the list.");
	
	$TechnologyName = trim(stripslashes($_REQUEST['TechnologyName']));
	if(empty($TechnologyName))
		error("Please select how you connect to the Internet from the list.");

	$AdvertisedSpeedDown = floatval($_REQUEST['AdvertisedSpeedDown']);
	if(empty($AdvertisedSpeedDown))
		error("AdvertisedSpeedDown not valid.");
		
	$AdvertisedSpeedUp = floatval($_REQUEST['AdvertisedSpeedUp']);
	if(empty($AdvertisedSpeedUp))
		error("AdvertisedSpeedUp not valid.");
		
	$OS = trim(stripslashes($_REQUEST['OS']));
	if(empty($OS))
		error("OS is not valid.");

	$OSVersion = trim(stripslashes($_REQUEST['OSVersion']));

	$EmailAddress = trim(stripslashes($_REQUEST['EmailAddress']));
	if(!validEmail($EmailAddress))
		error("Please enter a valid email address.");
	
	// Generate a new UUID
	$UUID = generateUuid();

	// Save to database:
	$sql = "INSERT INTO \"BroadbandUsers\" (
				\"UUID\",
				\"Postcode\",
				\"OperatorName\",
				\"TechnologyName\",
				\"AdvertisedSpeedDown\",
				\"AdvertisedSpeedUp\",
				\"OS\",
				\"OSVersion\",
				\"EmailAddress\",
				\"RegistrationTimestamp\"
			) VALUES (
				'".addslashes($UUID)."',
				'".addslashes($Postcode)."',
				'".addslashes($OperatorName)."',
				'".addslashes($TechnologyName)."',
				'".addslashes($AdvertisedSpeedDown)."',
				'".addslashes($AdvertisedSpeedUp)."',
				'".addslashes($OS)."',
				'".addslashes($OSVersion)."',
				'".addslashes($EmailAddress)."',
				NOW()
			);";
	$ret = @pg_query($_db, $sql);
	if(!$ret)
		error("Query failed.");

	// All done, let's return the uuid
	echo $UUID."\n";

	// Email us about the registration:
	$msg = "A new BSense client has registered:\n";
	$msg.= "----------------------------------------------------------------\n";
	$msg.= "Postcode:\t\t$Postcode\n";
	$msg.= "Email address:\t\t$EmailAddress\n";
	$msg.= "Operator name:\t\t$OperatorName\n";
	$msg.= "Technology:\t\t$TechnologyName\n";
	$msg.= "Advertised speed down:\t$AdvertisedSpeedDown\n";
	$msg.= "Advertised speed up:\t$AdvertisedSpeedUp\n";
	$msg.= "OS:\t\t\t$OS ($OSVersion)\n";
	$msg.= "Date:\t\t\t".date("r")."\n";
	$msg.= "IP:\t\t\t".$_SERVER['REMOTE_ADDR']."\n";
	$msg.= "UUID:\t\t\t$UUID\n";
	$msg.= "\n\n";
	$msg.= "--\n";
	$msg.= "Sent by BSense - University of Edinburgh\n";

	$headers = "From: \"info@broadbandforall.net\" <info@broadbandforall.net>\r\n";
	
	$mail = mail($_config['contactEmail'], "New broadbandforall.net user!", $msg, $headers);
}

/**
 * upstreamParameters: sends out the parameters for the upstream experiment
 */
function upstreamParameters() {
	global $_db, $_config;
	
	// Verifies the UUID:
	$UUID = $_REQUEST['UUID'];
	if(!verifyUuid($UUID)) 
		error("The UUID you provided is invalid.");
	
	// Pick experiment to run:

	// the current logic is: as we have two experiments, we find the one was last ran and pick the other
	$sql = "SELECT \"UpstreamExperimentID\"
		FROM \"RawMeasurementData\"
		WHERE \"UUID\" = '".addslashes($UUID)."'
		ORDER BY \"Timestamp\" DESC
		LIMIT 1";
	$rows = pg_query($_db, $sql);
	if(pg_num_rows($rows) == 0)
		$UpstreamExperimentID = 1;
	else {
		$row = pg_fetch_array($rows);
		if($row['UpstreamExperimentID'] == 1)
			$UpstreamExperimentID = 2;
		else	
			$UpstreamExperimentID = 1;
	}
	
	// the current logic is: pick randomly
	// $UpstreamExperimentID = mt_rand(1, 2);
	
	echo $UpstreamExperimentID."\n";

	// Return experiment parameters:
	foreach($_config['upstreamSendParameters'][$UpstreamExperimentID] as $line)
		echo $line." -rp ".generatePortNumber()."\n";
}

/**
 * downstreamParameters: sends out the parameters for the downstream experiment 
 * and starts the ITGSend process on the server
 */
function downstreamParameters() {
	global $_db, $_config;
	
	// Verifies the UUID:
	$UUID = $_REQUEST['UUID'];
	if(!verifyUuid($UUID)) 
		error("The UUID you provided is invalid.");

	// Determine a signaling port for this experiment:
	$signalingPort = generatePortNumber();

	// Pick experiment to run:
	// the current logic is: as we have two experiments, we find the one was last ran and pick the other
	$sql = "SELECT \"DownstreamExperimentID\"
		FROM \"RawMeasurementData\"
		WHERE \"UUID\" = '".addslashes($UUID)."'
		ORDER BY \"Timestamp\" DESC
		LIMIT 1";
	$rows = pg_query($_db, $sql);
	if(pg_num_rows($rows) == 0)
		$DownstreamExperimentID = 1;
	else {
		$row = pg_fetch_array($rows);
		if($row['DownstreamExperimentID'] == 1)
			$DownstreamExperimentID = 2;
		else	
			$DownstreamExperimentID = 1;
	}

	// the current logic is: pick randomly
	//$DownstreamExperimentID = mt_rand(1, 2);

	echo $DownstreamExperimentID."\n";

	// Write to a temporary file the parameters of the test:
	$tmpfname = tempnam(sys_get_temp_dir(), "bsense_");
	$fp = fopen($tmpfname, "w");
	foreach($_config['downstreamSendParameters'][$DownstreamExperimentID] as $line)
		fwrite($fp, $line." -sp ".generatePortNumber()." -Ssp ".$signalingPort."\n");
	fclose($fp);

	// Starts ITGSend
	$cmd = $_config['ITGSend']." ".$tmpfname." -l /dev/null > /dev/null 2> /dev/null & ";
	system($cmd);
	
	// Return experiment parameters:
	echo $_config['downstreamRecvParameters'][$DownstreamExperimentID]." -Sp ".$signalingPort."\n";
	
	// Remove temporary file:
	sleep(1); // to give ITGSend the time to read the file... 
	unlink($tmpfname);
}

/**
 * saveFailures: save logs of failures from the BSense application
 */
function saveFailures() {
	global $_db, $_config;
	
	// Verifies the UUID:
	$UUID = $_REQUEST['UUID'];
	if(!verifyUuid($UUID)) 
		error("The UUID you provided is invalid.");

	// Verify if the results are valid:
	$ErrorCode = intval($_REQUEST['ErrorCode']);
	$ErrorMessage = trim(stripslashes($_REQUEST['ErrorMessage']));

	// Get BSense Version:
	$BSenseVersion = trim($_REQUEST['BSenseVersion']);

	// Get Client IP:
	$ClientIp = $_SERVER['REMOTE_ADDR'];
	
	// Save in database:
	$sql = "INSERT INTO \"FailureLogs\" (
				\"UUID\",
				\"ErrorCode\",
				\"ErrorMessage\",
				\"ClientIp\",
				\"BSenseVersion\",
				\"Timestamp\"
			) VALUES (
				'".addslashes($UUID)."',
				'".addslashes($ErrorCode)."',
				'".addslashes($ErrorMessage)."',
				'".addslashes($ClientIp)."',
				'".addslashes($BSenseVersion)."',
				NOW()
			);";
	$ret = @pg_query($_db, $sql);
	if(!$ret)
		error("Query failed.");

	// All done:
	echo "ok\n";
}

/**
 * updateEmail: update user information by adding the email address
 */
function updateEmail() {
	global $_db, $_config;
	
	// Verifies the UUID:
	$UUID = $_REQUEST['UUID'];
	if(!verifyUuid($UUID)) 
		error("The UUID you provided is invalid.");
		
	// Parse and verify the input:
	$EmailAddress = trim(stripslashes($_REQUEST['EmailAddress']));
	if(!validEmail($EmailAddress))
		error("Please enter a valid email address.");
	
	// Save in database:
	$sql = "UPDATE \"BroadbandUsers\"
			SET \"EmailAddress\" = '".addslashes($EmailAddress)."'
			WHERE \"UUID\" = '".addslashes($UUID)."';";
	$ret = @pg_query($_db, $sql);
	if(!$ret)
		error("Query failed.");

	// All done:
	echo "ok\n";
}
	
/**
 * saveResults: save the measurement details in the database
 */
function saveResults() {
	global $_db, $_config;	
	
	// Verifies the UUID:
	$UUID = $_REQUEST['UUID'];
	if(!verifyUuid($UUID)) 
		error("The UUID you provided is invalid.");
	
	// Parse and verify the input:
	$BSenseVersion = trim(stripslashes($_REQUEST['BSenseVersion']));
	if(strlen($BSenseVersion) == 0)
		$BSenseVersion = "<= 0.5.3"; // versions prior to 0.5.4 didn't include client version in the POST fields
	
	$UpstreamExperimentID = intval($_REQUEST['UpstreamExperimentID']);
	if($UpstreamExperimentID == 0)
		error("UpstreamExperimentID is not valid.");
		
	$DownstreamExperimentID = intval($_REQUEST['DownstreamExperimentID']);
	if($DownstreamExperimentID == 0)
		error("DownstreamExperimentID is not valid.");
		
	$RawResultsUp = trim(stripslashes($_REQUEST['RawResultsUp']));
	if(strlen($RawResultsUp) == 0)
		error("RawResultsUp is empty.");
				
	$RawResultsDown = trim(stripslashes($_REQUEST['RawResultsDown']));
	if(empty($RawResultsDown))
		error("RawResultsDown is empty.");

	// Validate (and sanitize) the results:
	// This is needed for version "<= 0.5.4" because of a bug that was corrupting the results:
	if(!validateResults($RawResultsUp)) {
		$RawResultsUp = sanitizeResults($RawResultsUp);
	}

	if(!validateResults($RawResultsDown)) {
		$RawResultsDown = sanitizeResults($RawResultsDown);
	}		
		
	// Get Client IP:
	$ClientIp = $_SERVER['REMOTE_ADDR'];
	
	// Save in database:
	$sql = "INSERT INTO \"RawMeasurementData\" (
				\"UUID\",
				\"UpstreamExperimentID\",
				\"DownstreamExperimentID\",
				\"RawResultsUp\",
				\"RawResultsDown\",
				\"ClientIp\",
				\"BSenseVersion\",
				\"Timestamp\"
			) VALUES (
				'".addslashes($UUID)."',
				'".addslashes($UpstreamExperimentID)."',
				'".addslashes($DownstreamExperimentID)."',
				'".addslashes($RawResultsUp)."',
				'".addslashes($RawResultsDown)."',
				'".addslashes($ClientIp)."',
				'".addslashes($BSenseVersion)."',
				NOW()
			);";
	$ret = @pg_query($_db, $sql);
	if(!$ret)
		error("Query failed.");

	// All done:
	echo "ok\n";
}

/**
 * error: handles the errors
 */
function error($message) {
	global $_db, $_config;
	@pg_close($_db);
	
	echo "error\n";
	echo $message."\n";
	die();
}

?>
