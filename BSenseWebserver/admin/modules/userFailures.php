<?php

//=== Table of failures for a specific user:

// Verify if the UUID has a valid format:
$UUID = trim($_REQUEST['UUID']);	
if(!preg_match('/^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$/', $UUID))
	die("ERROR: the supplied UUID is not valid");

echo "
	<p><a href=\"?\">[&lt;&lt; Back to the users list]</a></p>
	<h3>Failure logs for this user:</h3>

	<table style='width: 100%'>
		<tr>
			<th>#</th>
			<th>Details:</th>
			<th>Error Log:</th>
		</tr>
";
	
$sql = "SELECT *
		FROM \"FailureLogs\"
		WHERE \"UUID\" = '".addslashes($UUID)."' 
		ORDER BY \"Timestamp\" DESC;";
$failures = pg_query($sql);

$i = 0;
while($failure = pg_fetch_array($failures)) {
	$i++;
	
	echo "<tr>";

	echo "<td class='odd' style='vertical-align: top'><strong>".$i."</strong></td>";
	
	echo "<td class='odd' style='text-align: left; vertical-align: top'>";
		echo "<strong>Time:</strong> ".date("d/m/Y H:i:s", strtotime($failure['Timestamp']))."<br />";	
		echo "<strong>Client IP:</strong> ".$failure['ClientIp']."<br />";
		echo "<strong>BSense version:</strong> ".$failure['BSenseVersion']."<br />";
		echo "<strong>Error code:</strong> ".$failure['ErrorCode']."<br />";
		echo "<br />";
	echo "</td>";
	
	echo "<td class='even' style='text-align: left; vertical-align: top'>".nl2br($failure['ErrorMessage'])."<br /><br /></td>";	

	echo "</tr>";

}
echo "</table>";


?>
