<?php

//=== Table of broadband users:
echo "Graphs and temporary experiments:
<ul>
<li><a href='?a=correlationGraph'>Correlation graph for ADSL users</a></li>
</ul>
<div class='hline'></div>
<h3>Users that downloaded and installed the software:</h3>

<table style='width: 100%'>
		<tr>
			<th rowspan='2'>#</th>
			<th rowspan='2'>Postcode:</th>
			<th rowspan='2'>BT est.<br />speed (Mbps):</th>
			<th rowspan='2'>Email address:</th>
			<th rowspan='2'>Operator name:</th>
			<th rowspan='2'>Technology:</th>
			<th rowspan='2'>Advertised up/down:</th>
			<th rowspan='2'>OS Version:</th>
			<th rowspan='2'>Registration date:</th>
			<th colspan='2'>Successful tests:</th>
			<th colspan='2'>Failures:</th>
			<th colspan='5'>Median values:</th>
		</tr>
		<tr>
			<th>Number</th>
			<th>Last</th>
			<th>Number</th>
			<th>Last</th>
			<th>Down<br/>(Mbps)</th>
			<th>Up<br/>(Mbps)</th>
			<th>Latency<br/>(ms)</th>
			<th>Jitter<br/>(ms)</th>
			<th>Loss<br/>(?)</th>
		</tr>
";
	
$sql = "SELECT *
		FROM \"BroadbandUsers\"
		ORDER BY \"TechnologyName\" ASC, \"OperatorName\"  ASC, \"RegistrationTimestamp\" DESC;";
$users = pg_query($sql);

$i = 0;
while($user = pg_fetch_array($users)) {
	$i++;
	
	echo "<tr class=\"".(($i % 2) ? 'even' : 'odd')."\">";
	echo "<td>".$i."</td>";
	echo "<td>".$user['Postcode']."<br /><a href='?a=userLocation&UUID=".$user['UUID']."'' title='Show on map...'><img src='/images/map-icon.png' alt='Show on map...'></a></td>";
	
	//=== Get BT estimated speed for this postcode:
	$sql = "SELECT \"EstimatedSpeed\"
			FROM \"BtAdslCheck\"
			WHERE \"Postcode\"='".addslashes($user['Postcode'])."'";
	$estimations = pg_query($sql);
	$estimation = pg_fetch_array($estimations);
	echo "<td>".$estimation['EstimatedSpeed']."</td>";
	
	echo "<td>".$user['EmailAddress']."</td>";
	echo "<td>".$user['OperatorName']."</td>";
	echo "<td>".$user['TechnologyName']."</td>";
	echo "<td>".(($user['AdvertisedSpeedDown'] == -1) ? "unknown" : $user['AdvertisedSpeedDown']." Mbps")."<br />";
	echo (($user['AdvertisedSpeedUp'] == -1) ? "unknown" : $user['AdvertisedSpeedUp']." Mbps")."</td>";
	echo "<td>".$user['OS']."<br />".$user['OSVersion']."</td>";
	echo "<td>".date("H:i d/m", strtotime($user['RegistrationTimestamp']))."</td>";	
	
	// Successful tests:
	echo "<td>".$user['SuccessfulTestsNumber']."<br />";
	echo "<a href='?a=userResults&UUID=".$user['UUID']."' title='View...'><img src='/images/search-icon.gif' alt='View...'></a>";
	echo "<a href='?a=userResultsDistribution&UUID=".$user['UUID']."' title='View results distribution...'><img src='/images/graph-icon.png' alt='View results distribution...'></a></td>";
	if(!$user['SuccessfulTestsLast'])
		echo "<td>n/a</td>";
	else
		echo "<td>".date("H:i d/m", strtotime($user['SuccessfulTestsLast']))."</td>";	
	
	// Failures:
	echo "<td>".$user['FailuresNumber']."<br />";
	echo "<a href='?a=userFailures&UUID=".$user['UUID']."' title='View...'><img src='/images/search-icon.gif' alt='View...'></a></td>";
	if(!$user['FailuresLast'])
		echo "<td>n/a</td>";
	else
		echo "<td>".date("H:i d/m", strtotime($user['FailuresLast']))."</td>";
	
	// Medians:
	echo "<td>".number_format($user['DownloadSpeed'], 3)."</td>";
	echo "<td>".number_format($user['UploadSpeed'], 3)."</td>";
	echo "<td>".number_format($user['Latency'], 3)."</td>";
	echo "<td>".number_format($user['Jitter'], 3)."</td>";
	echo "<td>".number_format($user['PacketLoss'], 3)."</td>";
	
	echo "</tr>\n";
}
echo "</table>";


?>
