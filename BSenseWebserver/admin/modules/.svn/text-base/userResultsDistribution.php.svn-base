<?php
//=== Graph of results for a specific user:

// Verify if the UUID has a valid format:
$UUID = trim($_REQUEST['UUID']);	
if(!preg_match('/^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$/', $UUID))
	die("ERROR: the supplied UUID is not valid");

echo "
	<p><a href=\"?\">[&lt;&lt; Back to the users list]</a></p>
	<p>Distribution of results for this user:</p>";

?>

<div class='hline'></div><h3><strong>Download speed:</strong></h3>
<div id="DownloadSpeed" style="width:95%; height:300px; margin-top: 15px; margin-bottom: 15px"></div>

<div class='hline'></div><h3><strong>Upload speed:</strong></h3>
<div id="UploadSpeed" style="width:95%; height:300px; margin-top: 15px; margin-bottom: 15px"></div>

<div class='hline'></div><h3>Latency:</h3>
<div id="Latency" style="width:95%; height:300px; margin-top: 15px; margin-bottom: 15px"></div>

<div class='hline'></div><h3>Jitter:</h3>
<div id="Jitter" style="width:95%; height:300px; margin-top: 15px; margin-bottom: 15px"></div>

<div class='hline'></div><h3>PacketLoss (please ignore for now...):</h3>
<div id="PacketLoss" style="width:95%; height:300px; margin-top: 15px; margin-bottom: 15px"></div>


<script id="source" language="javascript" type="text/javascript">
$(function () {
	var DownloadSpeedUdp = []; var DownloadSpeedTcp = [];
	var UploadSpeedUdp = []; var UploadSpeedTcp = [];
	var Latency = [];
	var Jitter = [];
	var PacketLoss = [];
	
<?php

$DownloadSpeedsUdp = array(); $DownloadSpeedsTcp = array();
$UploadSpeedsUdp = array(); $UploadSpeedsTcp = array();
$Latencies = array();
$Jitters = array();
$PacketLosses = array();

$sql = "SELECT p.*, r.\"DownstreamExperimentID\", r.\"UpstreamExperimentID\"
		FROM \"RawMeasurementData\" AS r
		INNER JOIN \"ParsedMeasurementData\" AS p ON r.\"MeasurementID\"=p.\"MeasurementID\"
		WHERE r.\"UUID\" = '".addslashes($UUID)."'
		AND p.\"Valid\" = 'true';";
$results = pg_query($sql);
while($result = pg_fetch_array($results)) {
	if($result['DownstreamExperimentID'] == 1)
		$DownloadSpeedsUdp[] = $result['DownloadSpeed'];
	else
		$DownloadSpeedsTcp[] = $result['DownloadSpeed'];
		
	if($result['UpstreamExperimentID'] == 2)
		$UploadSpeedsUdp[] = $result['UploadSpeed'];
	else
		$UploadSpeedsTcp[] = $result['UploadSpeed'];
		
	$Latencies[] = $result['Latency'];
	$Jitters[] = $result['Jitter'];
	$PacketLosses[] = $result['PacketLoss'];
}

// Calculate the probability density functions:

$min = min($DownloadSpeedsUdp) * 0.5; // add extra space on the left
$max = max($DownloadSpeedsUdp) * 1.2; // add extra space on the right
for($i = $min; $i <= $max; $i += 0.100) {	// 100Kbps
	$count = 0;
	foreach($DownloadSpeedsUdp as $DownloadSpeed) {
		if( $DownloadSpeed>$i && $DownloadSpeed<=($i + 0.100) )
			$count++;
	}
	echo "\tDownloadSpeedUdp.push([".$i.", ".$count."]);\n";
}
$min = min($DownloadSpeedsTcp) * 0.5; // add extra space on the left
$max = max($DownloadSpeedsTcp) * 1.2; // add extra space on the right
for($i = $min; $i <= $max; $i += 0.100) {	// 100Kbps
	$count = 0;
	foreach($DownloadSpeedsTcp as $DownloadSpeed) {
		if( $DownloadSpeed>$i && $DownloadSpeed<=($i + 0.100) )
			$count++;
	}
	echo "\tDownloadSpeedTcp.push([".$i.", ".$count."]);\n";
}

$min = min($UploadSpeedsUdp) * 0.5; // add extra space on the left
$max = max($UploadSpeedsUdp) * 1.2; // add extra space on the right
for($i = $min; $i <= $max; $i += 0.010) {	// 10Kbps
	$count = 0;
	foreach($UploadSpeedsUdp as $UploadSpeed) {
		if( $UploadSpeed>$i && $UploadSpeed<=($i + 0.010) )
			$count++;
	}
	echo "\tUploadSpeedUdp.push([".$i.", ".$count."]);\n";
}
$min = min($DownloadSpeedsTcp) * 0.5; // add extra space on the left
$max = max($DownloadSpeedsTcp) * 1.2; // add extra space on the right
for($i = $min; $i <= $max; $i += 0.010) {	// 10Kbps
	$count = 0;
	foreach($DownloadSpeedsTcp as $UploadSpeed) {
		if( $UploadSpeed>$i && $UploadSpeed<=($i + 0.010) )
			$count++;
	}
	echo "\tUploadSpeedTcp.push([".$i.", ".$count."]);\n";
}

$min = min($Latencies) * 0.5; // add extra space on the left
$max = max($Latencies) * 1.2; // add extra space on the right
for($i = $min; $i <= $max; $i += 0.005) {	// 5 ms
	$count = 0;
	foreach($Latencies as $Latency) {
		if( $Latency>$i && $Latency<=($i + 0.005) )
			$count++;
	}
	echo "\tLatency.push([".($i*1000).", ".$count."]);\n";
}

$min = min($Jitters) * 0.5; // add extra space on the left
$max = max($Jitters) * 1.2; // add extra space on the right
for($i = $min; $i <= $max; $i += 0.001) {	// 1 ms
	$count = 0;
	foreach($Jitters as $Jitter) {
		if( $Jitter>$i && $Jitter<=($i + 0.001) )
			$count++;
	}
	echo "\tJitter.push([".($i*1000).", ".$count."]);\n";
}

$min = min($PacketLosses) * 0.5; // add extra space on the left
$max = max($PacketLosses) * 1.2; // add extra space on the right
for($i = $min; $i <= $max; $i += 0.001) {	// ???
	$count = 0;
	foreach($PacketLosses as $PacketLoss) {
		if( $PacketLoss>$i && $PacketLoss<=($i + 0.001) )
			$count++;
	}
	echo "\tPacketLoss.push([".$i.", ".$count."]);\n";
}


// Find the median values:
$sql = "SELECT \"DownloadSpeed\", \"UploadSpeed\", \"Latency\", \"Jitter\", \"PacketLoss\"
		FROM \"BroadbandUsers\" 
		WHERE \"UUID\" = '".addslashes($UUID)."'";
$medians = pg_query($sql);
$median = pg_fetch_array($medians)

?>
	//=== DownloadSpeed graph:
	var plot = $.plot($("#DownloadSpeed"), 
		[{
			data: DownloadSpeedUdp,
			color: '#f00',
			label: 'UDP'
		},{
			data: DownloadSpeedTcp,
			color: '#0f0',
			label: 'TCP'
		}],
		{
			yaxis: { 
				tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals); },
				labelWidth: 90
			},
			xaxis: {
		    	tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + " Mbps"; }
			},
			lines: {
				fill: true,
				lineWidth: 1
			},
		    points: { 
				show: false 
			},
			grid: {
		       	backgroundColor: '#fff',
				markings: [{ 
					color: '#666', lineWidth: 2, xaxis: { from: <?php echo $median['DownloadSpeed']; ?>, to: <?php echo $median['DownloadSpeed']; ?>} 
				}]
		   	}
		});
    var o = plot.pointOffset({ x: <?php echo $median['DownloadSpeed']; ?>, y: 5});
    $("#DownloadSpeed").append('<div style="position:absolute;left:' + (o.left + 5) + 'px;top:10px;color:#666;font-size:smaller">Median: <?php echo number_format($median['DownloadSpeed'], 3); ?></div>');
		
	//=== UploadSpeed graph:
	var plot = $.plot($("#UploadSpeed"), 
		[{
			data: UploadSpeedUdp,
			color: '#f00',
			label: 'UDP'
		},{
			data: UploadSpeedTcp,
			color: '#0f0',
			label: 'TCP'
		}],
		{
			yaxis: { 
				tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals); },
				labelWidth: 90
			},
			xaxis: {
		    	tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + " Mbps"; }
			},
			lines: {
				fill: true,
				lineWidth: 1
			},
		    points: { 
				show: false 
			},
			grid: {
		       	backgroundColor: '#fff',
				markings: [{ 
					color: '#666', lineWidth: 2, xaxis: { from: <?php echo $median['UploadSpeed']; ?>, to: <?php echo $median['UploadSpeed']; ?>} 
				}]
		   	}
		});
	var o = plot.pointOffset({ x: <?php echo $median['UploadSpeed']; ?>, y: 5});
	$("#UploadSpeed").append('<div style="position:absolute;left:' + (o.left + 5) + 'px;top:10px;color:#666;font-size:smaller">Median: <?php echo number_format($median['UploadSpeed'], 3); ?></div>');
		
	//=== Latency graph:
	var plot = $.plot($("#Latency"), 
		[{
			data: Latency, 
			color: '#f00'
		}],
		{
			yaxis: { 
				tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals); },
				labelWidth: 90
			},
			xaxis: {
		    	tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + " ms"; }
			},
			lines: {
				fill: true,
				lineWidth: 1
			},
		    points: { 
				show: false 
			},
			grid: {
		       	backgroundColor: '#fff',
				markings: [{ 
					color: '#666', lineWidth: 2, xaxis: { from: <?php echo $median['Latency']*1000; ?>, to: <?php echo $median['Latency']*1000; ?>} 
				}]
		   	}
		});
    var o = plot.pointOffset({ x: <?php echo ($median['Latency']*1000); ?>, y: 5});
    $("#Latency").append('<div style="position:absolute;left:' + (o.left + 5) + 'px;top:10px;color:#666;font-size:smaller">Median: <?php echo number_format($median['Latency']*1000, 3); ?></div>');
			
	//=== Jitter graph:
	var plot = $.plot($("#Jitter"), 
		[{
			data: Jitter, 
			color: '#f00'
		}],
		{
			yaxis: { 
				tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals); },
				labelWidth: 90
			},
			xaxis: {
		    	tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + " ms"; }
			},
			lines: {
				fill: true,
				lineWidth: 1
			},
		    points: { 
				show: false 
			},
			grid: {
		       	backgroundColor: '#fff',
				markings: [{ 
					color: '#666', lineWidth: 2, xaxis: { from: <?php echo $median['Jitter']*1000; ?>, to: <?php echo $median['Jitter']*1000; ?>} 
				}]
		   	}
		});
    var o = plot.pointOffset({ x: <?php echo $median['Jitter']*1000; ?>, y: 5});
    $("#Jitter").append('<div style="position:absolute;left:' + (o.left + 5) + 'px;top:10px;color:#666;font-size:smaller">Median: <?php echo number_format($median['Jitter']*1000, 3); ?></div>');

	//=== PacketLoss graph:
	var plot = $.plot($("#PacketLoss"), 
		[{
			data: PacketLoss, 
			color: '#f00'
		}],
		{
			yaxis: { 
				tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals); },
				labelWidth: 90
			},
			xaxis: {
		    	tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals); }
			},
			lines: {
				fill: true,
				lineWidth: 1
			},
		    points: { 
				show: false 
			},
			grid: {
		       	backgroundColor: '#fff',
				markings: [{ 
					color: '#666', lineWidth: 2, xaxis: { from: <?php echo $median['PacketLoss']; ?>, to: <?php echo $median['PacketLoss']; ?>} 
				}]
		   	}
		});
    var o = plot.pointOffset({ x: <?php echo $median['PacketLoss']; ?>, y: 5});
    $("#PacketLoss").append('<div style="position:absolute;left:' + (o.left + 5) + 'px;top:10px;color:#666;font-size:smaller">Median: <?php echo number_format($median['PacketLoss'], 3); ?></div>');
		
});
</script>
