<?php
//=== Table of results for a specific user:

// Verify if the UUID has a valid format:
$UUID = trim($_REQUEST['UUID']);	
if(!preg_match('/^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$/', $UUID))
	die("ERROR: the supplied UUID is not valid");

$MeasurementID = intval($_REQUEST['MeasurementID']);	
if($MeasurementID <= 0)
	die("ERROR: the supplied MeasurementID is not valid");

echo "
	<p><a href=\"?a=userResults&UUID=".$UUID."\">[&lt;&lt; Back to the measurement list]</a></p>
	<h3>Details of the recorded measurement:</h3>";

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

// Parse the results:
$RawResultsUp = parseResults($detail['RawResultsUp']);
$RawResultsDown = parseResults($detail['RawResultsDown']);

// Show the results as tables:
echo "
	<table style='width: 100%'>
	<tr><td style='vertical-align: top; background-color: #ccc'>

		<p><strong>Upstream:</strong></p>
		<table style='width: 100%'>
			<tr>
				<th>Time</th>
				<th>Bitrate (Kbps):</th>
				<th>Delay (ms):</th>
				<th>Jitter (ms):</th>
				<th>Loss:</th>
			</tr>
";
	
$i = 0;
foreach($RawResultsUp as $row) {
	$i++;
	
	echo "<tr class=\"".(($i % 2) ? 'even' : 'odd')."\">";
	
	echo "<td>".number_format($row['time'], 1, ".", "")."</td>";
	echo "<td>".number_format($row['bitrate'], 3, ".", "")."</td>";
	echo "<td>".number_format(($row['delay'] * 1000), 3, ".", "")."</td>";
	echo "<td>".number_format(($row['jitter'] * 1000), 3, ".", "")."</td>";
	echo "<td>".$row['loss']."</td>";

	echo "</tr>";

}
echo "</table>

	</td><td style='vertical-align: top; background-color: #ccc'>

	<p><strong>Downstream:</strong></p>
	<table style='width: 100%'>
		<tr>
			<th>Time</th>
			<th>Bitrate (Kbps):</th>
			<th>Delay (ms):</th>
			<th>Jitter (ms):</th>
			<th>Loss:</th>
		</tr>
";
	
$i = 0;
foreach($RawResultsDown as $row) {
	$i++;
	
	echo "<tr class=\"".(($i % 2) ? 'even' : 'odd')."\">";
	
	echo "<td>".number_format($row['time'], 1, ".", "")."</td>";
	echo "<td>".number_format($row['bitrate'], 2, ".", "")."</td>";
	echo "<td>".number_format(($row['delay'] * 1000), 3, ".", "")."</td>";
	echo "<td>".number_format(($row['jitter'] * 1000), 3, ".", "")."</td>";
	echo "<td>".$row['loss']."</td>";
	
	echo "</tr>";

}
echo "</table>

</td></tr></table>
";


?>
