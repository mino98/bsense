<?php

//=== Include configuration values and functions:
require("../includes/config.inc.php");
require("../includes/functions.inc.php");

//=== Include header:
require("header.inc.php");

//=== Database connection:
global $db;
$db = @pg_connect("dbname=".$_config['databaseName']." user=".$_config['databaseUser']." password=".$_config['databasePass']);
if(!$db)
	die("<div class=\"error\"><strong>Internal error:</strong> code #1001.<br />
		Feel free to provide us feedback by using the form on the right. Thank you.</div>");
		
//=== Load the module selected:
$a = $_REQUEST['a'];
if(!$a || $a=="")
	$a = "usersList";	// default module

switch($a) {	
	case 'usersList':
		require("modules/usersList.php");
		break;

	case 'userLocation':
		require("modules/userLocation.php");
		break;

	case 'userFailures':
		require("modules/userFailures.php");
		break;
		
	case 'userResults':
		require("modules/userResults.php");
		break;
		
	case 'userResultsDistribution':
		require("modules/userResultsDistribution.php");
		break;
		
	case 'userResultDetails':
		require("modules/userResultDetails.php");
		break;
			
	case 'userResultGraph':
		require("modules/userResultGraph.php");
		break;
	
	case 'correlationGraph':
		require("modules/correlationGraph.php");
		break;

	default:
		echo "ERROR: module not found";
}

//=== Close connection with the database:
pg_close($db);

//=== Include header:
require("footer.inc.php");

?>
