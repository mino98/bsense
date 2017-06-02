<?php
//=== Graph of results for a specific user:

// Verify if the UUID has a valid format:
$UUID = trim($_REQUEST['UUID']);	
if(!preg_match('/^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$/', $UUID))
	die("ERROR: the supplied UUID is not valid");

$MeasurementID = intval($_REQUEST['MeasurementID']);	
if($MeasurementID <= 0)
	die("ERROR: the supplied MeasurementID is not valid");

echo "
	<p><a href=\"?a=userResults&UUID=".$UUID."\">[&lt;&lt; Back to the measurement list]</a></p>
	<p>Graphs of the recorded measurement:</p>";

// Extract results from the DB:
$sql = "SELECT 
			r.\"RawResultsUp\",
 			r.\"RawResultsDown\",
			p.*
		FROM \"RawMeasurementData\" AS r
		LEFT JOIN \"ParsedMeasurementData\" AS p ON r.\"MeasurementID\"=p.\"MeasurementID\"
		WHERE r.\"MeasurementID\" = '".addslashes($MeasurementID)."' 
		LIMIT 1;";
		
$details = pg_query($sql);
if(pg_num_rows($details) != 1)
	die("ERROR: measurement not found.");
	
$detail = pg_fetch_array($details);

// Parse the results:
$RawResultsUp = parseResults($detail['RawResultsUp']);
$RawResultsDown = parseResults($detail['RawResultsDown']);

?>

<div class='hline'></div><h3><strong>Downstream test:</strong></h3>
<div id="bitrateDown" style="width:95%; height:300px; margin-top: 15px; margin-bottom: 15px"></div>

<div class='hline'></div><h3><strong>Upstream test:</strong></h3>
<div id="bitrateUp" style="width:95%; height:300px; margin-top: 15px; margin-bottom: 15px"></div>

<div class='hline'></div><h3>Ping test:</h3>
<div id="delay" style="width:95%; height:150px; margin-top: 15px; margin-bottom: 15px"></div>
<div id="jitter" style="width:95%; height:150px; margin-top: 15px; margin-bottom: 15px"></div>
<div id="loss" style="width:95%; height:150px; margin-top: 15px; margin-bottom: 15px"></div>


<script id="source" language="javascript" type="text/javascript">
$(function () {
	var delay = [];
	var jitter = [];
	var loss = [];
	var bitrateUp = [];
	var bitrateDown = [];
	
<?php

// Print out data:
foreach($RawResultsUp as $row) {
	if($row['time'] < 60) {
		echo "\tdelay.push([".$row['time'].", ".($row['delay'] * 1000)."]);";
		echo "\tjitter.push([".$row['time'].", ".($row['jitter'] * 1000)."]);";
		echo "\tloss.push([".$row['time'].", ".$row['loss']."]);\n";
	} else {
		echo "\tbitrateUp.push([".($row['time'] - 60).", ".($row['bitrate'] / 1000)."]);\n";
	}
}
foreach($RawResultsDown as $row) {
	echo "\tbitrateDown.push([".($row['time']).", ".($row['bitrate'] / 1000)."]);\n";
}

?>

	// === Downstream graph:
	var plot = $.plot($("#bitrateDown"), 
		[{ 
			data: bitrateDown,
			color: '#f00'
		}],
		{
			yaxis: { 
				min: 0,
				tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + " Mbps"; },
				labelWidth: 90
			},
			xaxis: {
				min: 0,
		    	tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + "\""; }
			},
			lines: {
				lineWidth: 1,
				fill: true
			},
		    points: { 
				show: false 
			},
			grid: {
		       	backgroundColor: '#fff',
				markings: [{ 
					color: '#666', lineWidth: 2, yaxis: { from: <?php echo $detail['DownloadSpeed']; ?>, to: <?php echo $detail['DownloadSpeed']; ?> } 
				}]
			}
		});
    var o = plot.pointOffset({ x: 0, y: <?php echo $detail['DownloadSpeed']; ?>});
    $("#bitrateDown").append('<div style="position:absolute;left:' + (o.left + 5) + 'px;top:' + (o.top - 15) + 'px;background-color:rgba(255,255,255,0.7);color:#666;font-size:smaller">Average: <?php echo number_format($detail['DownloadSpeed'], 3); ?></div>');

	// === Upstream graph:
	var plot = $.plot($("#bitrateUp"), 
		[{ 
			data: bitrateUp, 
			color: '#f00'
		}],
		{
			yaxis: { 
				min: 0,
				tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + " Mbps"; },
				labelWidth: 90
			},
			xaxis: {
				min: 0,
		    	tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + "\""; }
			},
			lines: {
				lineWidth: 1,
				fill: true
			},
		    points: { 
				show: false 
			},
			grid: {
		       	backgroundColor: '#fff',
				markings: [{ 
					color: '#666', lineWidth: 2, yaxis: { from: <?php echo $detail['UploadSpeed']; ?>, to: <?php echo $detail['UploadSpeed']; ?> } 
				}]
		   	}
		});
	var o = plot.pointOffset({ x: 0, y: <?php echo $detail['UploadSpeed']; ?>});
    $("#bitrateUp").append('<div style="position:absolute;left:' + (o.left + 5) + 'px;top:' + (o.top - 15) + 'px;background-color:rgba(255,255,255,0.7);color:#666;font-size:smaller">Average: <?php echo number_format($detail['UploadSpeed'], 3); ?></div>');

	// === Latency graph:
	var plot = $.plot($("#delay"), 
		[{ 
			data: delay, 
			color: '#f00'
		}],
		{
			yaxis: { 
				min: 0,
				tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + " ms"; },
				labelWidth: 90
			},
			xaxis: {
				min: 0,
		    	tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + "\""; }
			},
			lines: {
				lineWidth: 1,
				fill: true
			},
		    points: { 
				show: false 
			},
			grid: {
		       	backgroundColor: '#fff',
				markings: [{ 
					color: '#666', lineWidth: 2, yaxis: { from: <?php echo $detail['Latency']*1000; ?>, to: <?php echo $detail['Latency']*1000; ?> } 
				}]
		   	}
		});
	var o = plot.pointOffset({ x: 0, y: <?php echo $detail['Latency']*1000; ?>});
	$("#delay").append('<div style="position:absolute;left:' + (o.left + 5) + 'px;top:' + (o.top - 15) + 'px;background-color:rgba(255,255,255,0.7);color:#666;font-size:smaller">Average: <?php echo number_format($detail['Latency']*1000, 3); ?></div>');

	// === Jitter graph:
	var plot = $.plot($("#jitter"), 
		[{ 
			data: jitter, 
			color: '#f00'
		}],
		{
			yaxis: { 
				min: 0,
				tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + " ms"; },
				labelWidth: 90
			},
			xaxis: {
				min: 0,
		    	tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + "\""; }
			},
			lines: {
				lineWidth: 1,
				fill: true
			},
		    points: { 
				show: false 
			},
			grid: {
		       	backgroundColor: '#fff',
				markings: [{ 
					color: '#666', lineWidth: 2, yaxis: { from: <?php echo $detail['Jitter']*1000; ?>, to: <?php echo $detail['Jitter']*1000; ?> } 
				}]
		   	}
		});
	var o = plot.pointOffset({ x: 0, y: <?php echo $detail['Jitter']*1000; ?>});
	$("#jitter").append('<div style="position:absolute;left:' + (o.left + 5) + 'px;top:' + (o.top - 15) + 'px;background-color:rgba(255,255,255,0.7);color:#666;font-size:smaller">Average: <?php echo number_format($detail['Jitter']*1000, 3); ?></div>');

	// === Packetloss graph:
	var plot = $.plot($("#loss"), 
		[{ 
			data: loss, 
			color: '#f00'
		}],
		{
			yaxis: { 
				min: 0,
				tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals); },
				labelWidth: 90
			},
			xaxis: {
				min: 0,
		    	tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + "\""; }
			},
			lines: {
				lineWidth: 1,
				fill: true 
			},
		    points: { 
				show: false 
			},
			grid: {
		       	backgroundColor: '#fff',
				markings: [{ 
					color: '#666', lineWidth: 2, yaxis: { from: <?php echo $detail['PacketLoss']; ?>, to: <?php echo $detail['PacketLoss']; ?> } 
				}]
		   	}
		});
	var o = plot.pointOffset({ x: 0, y: <?php echo $detail['PacketLoss']; ?>});
	$("#loss").append('<div style="position:absolute;left:' + (o.left + 5) + 'px;top:' + (o.top - 15) + 'px;background-color:rgba(255,255,255,0.7);color:#666;font-size:smaller">Average: <?php echo number_format($detail['PacketLoss'], 3); ?></div>');

});
</script>
