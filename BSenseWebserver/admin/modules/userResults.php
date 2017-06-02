<?php

//=== Table of results for a specific user:

// Function for the RIPE lookup:
require("../includes/ripeWhois.php");

// Verify if the UUID has a valid format:
$UUID = trim($_REQUEST['UUID']);	
if(!preg_match('/^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$/', $UUID))
	die("ERROR: the supplied UUID is not valid");

echo "
	<p><a href=\"?\">[&lt;&lt; Back to the users list]</a></p>
	<h3>Measurements for this user:</h3>";
	
echo "
	<table style='width: 100%'>
		<tr>
			<th rowspan='2'>#</th>
			<th rowspan='2'>Date and Time:</th>
			<th rowspan='2'>Client IP:</th>
			<th rowspan='2'>BSense Version:</th>
			<th rowspan='2'>Upstream Experiment ID:</th>
			<th rowspan='2'>Downstream Experiment ID:</th>
			<th colspan='5'>Average Values:</th>
			<th rowspan='2'>View:</th>
		</tr>
		<tr>
			<th>DownloadSpeed (Mbps):</th>
			<th>UploadSpeed (Mbps):</th>
			<th>Latency (ms):</th>
			<th>Jitter (ms):</th>
			<th>PacketLoss (pkts):</th>
		</tr>
";
	
$sql = "SELECT 
 			r.\"Timestamp\",
			r.\"ClientIp\",
			r.\"BSenseVersion\",
			r.\"UpstreamExperimentID\",
			r.\"DownstreamExperimentID\",
			r.\"MeasurementID\",
			p.\"UploadSpeed\", 
			p.\"DownloadSpeed\",
			p.\"Latency\",
			p.\"Jitter\",
			p.\"PacketLoss\",
			p.\"Description\",
			p.\"Valid\"
		FROM \"RawMeasurementData\" AS r
		LEFT JOIN \"ParsedMeasurementData\" AS p ON r.\"MeasurementID\"=p.\"MeasurementID\"
		WHERE r.\"UUID\" = '".addslashes($UUID)."' 
		ORDER BY r.\"Timestamp\" DESC;";
$measurements = pg_query($sql);

$i = 0;
$whoisCacheIp = null;
$whoisCacheName = null;
while($measurement = pg_fetch_array($measurements)) {
	$i++;
	
	if($measurement['Valid'] == 't')	
		echo "<tr class=\"".(($i % 2) ? 'even' : 'odd')."\">";
	else
		echo "<tr class=\"notvalid\">";
		
	echo "<td>".$i."</td>";
	echo "<td>".date("d/m/Y H:i:s", strtotime($measurement['Timestamp']))."</td>";	
	echo "<td>".$measurement['ClientIp']."<br />".$measurement['Description']."</td>";
	echo "<td>".$measurement['BSenseVersion']."</td>";
	echo "<td>".(($measurement['UpstreamExperimentID'] == 1 ) ? '1 (UDP test)' : '2 (TCP test)')."</td>";
	echo "<td>".(($measurement['DownstreamExperimentID'] == 1 ) ? '1 (UDP test)' : '2 (TCP test)')."</td>";
	echo "<td>".number_format($measurement['DownloadSpeed'], 3)."</td>";
	echo "<td>".number_format($measurement['UploadSpeed'], 3)."</td>";
	echo "<td>".number_format($measurement['Latency'] * 1000, 3)."</td>";
	echo "<td>".number_format($measurement['Jitter'] * 1000, 3)."</td>";
	echo "<td>".number_format($measurement['PacketLoss'], 3)."</td>";
	echo "<td>
		<a href='?a=userResultGraph&UUID=".$UUID."&MeasurementID=".$measurement['MeasurementID']."' title='View as graph...'><img src='/images/graph-icon.png' alt='View as graph...'></a>
		<a href='?a=userResultDetails&UUID=".$UUID."&MeasurementID=".$measurement['MeasurementID']."' title='View as table...'><img src='/images/table-icon.gif' alt='View as table...'></a>
		</td>";

	echo "</tr>";

}
echo "</table>";


?>
