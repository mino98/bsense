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
	<p>Graph of the recorded measurement:</p>";

// Extract results from the DB:
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

/**
 * parseResults: parses the results in an array.
 * 
 * PARAMETERS:
 *   $results: textual table with the results
 *   $samples: number of samples to average with a simple moving average (SMA). If null, no averaging done. 
 */
function parseResults2($results, $samples = null) {
	$rows = explode("\n", $results);
	$results = array(); 
	$i = 0;
	
	foreach($rows as $row) {
		
		// Skip empty lines:
		if($row == "")
			continue; 
			
		// Parse columns into an array:
		$columns = explode(" ", $row);

		// Check format:
		if(count($columns) != 5) {
			echo "WARNING: result row \"$row\" not parse-able!\n";
			continue;
		}

		$results[$i]['time'] = floatval($columns[0]);
		$results[$i]['bitrate'] = floatval($columns[1]);
		$results[$i]['delay'] = floatval($columns[2]);
		$results[$i]['jitter'] = floatval($columns[3]);
		$results[$i]['loss'] = floatval($columns[4]);
	
		// Average results if requested:
		if($samples) {
			$sum = array(); $count = 0;
			for($j = $samples - 1; $j >= 0; $j--) {
				if(($i - $j) >= 0) {
					$sum['bitrate'] += $results[$i - $j]['bitrate'];
					$sum['delay'] += $results[$i - $j]['delay'];
					$sum['jitter'] += $results[$i - $j]['jitter'];
					$sum['loss'] += $results[$i - $j]['loss'];
					$count++;
				}
			}
			$results[$i]['bitrate'] = $sum['bitrate'] / $count;
			$results[$i]['delay'] = $sum['delay'] / $count;
			$results[$i]['jitter'] = $sum['jitter'] / $count;		
			$results[$i]['loss'] = $sum['loss'] / $count;
		}
	
		$i++;
	}
	
	return $results;
}

// Parse the results:
$RawResultsUp = parseResults2($detail['RawResultsUp'], 2);
$RawResultsDown = parseResults2($detail['RawResultsDown'], 2);

?>

<div class='hline'></div><h3>Ping test:</h3>
<div id="delay" style="width:95%; height:150px; margin-top: 15px; margin-bottom: 15px"></div>
<div id="jitter" style="width:95%; height:150px; margin-top: 15px; margin-bottom: 15px"></div>
<div id="loss" style="width:95%; height:150px; margin-top: 15px; margin-bottom: 15px"></div>

<div class='hline'></div><h3><strong>Upstream test:</strong></h3>
<div id="bitrateUp" style="width:95%; height:300px; margin-top: 15px; margin-bottom: 15px"></div>

<div class='hline'></div><h3><strong>Downstream test:</strong></h3>
<div id="bitrateDown" style="width:95%; height:300px; margin-top: 15px; margin-bottom: 15px"></div>


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
	echo "\tbitrateDown.push([".$row['time'].", ".($row['bitrate'] / 1000)."]);\n";
}

?>

	// === Latency graph:
	var options = {
	    lines: { 
			show: true, 
			lineWidth: 1
		},
		label: 'RTTM delay',
	    points: { 
			show: false 
		},
		grid: {
	       	backgroundColor: '#fff'
	   	},
		colors: [ '#f00' ],
		yaxis: { 
			min: 0,
			tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + " ms"; },
			labelWidth: 90
		},
		xaxis: {
			min: 0,
	    	tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + "\""; }
		}
	};

	$.plot($("#delay"), [ delay ], options);

	// === Jitter graph:
	
	var options = {
	    lines: { 
			show: true, 
			lineWidth: 1
		},
		label: 'Jitter',
	    points: { 
			show: false 
		},
		grid: {
	       	backgroundColor: '#fff'
	   	},
		colors: [ '#f00' ],
		yaxis: { 
			min: 0,
			tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + " ms"; },
			labelWidth: 90
		},
		xaxis: {
			min: 0,
	    	tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + "\""; }
		}
	};
	
	$.plot($("#jitter"), [ jitter ], options);

	// === Packetloss graph:
	
	var options = {
	    lines: { 
			show: true, 
			lineWidth: 1
		},
		label: 'Packet loss',
	    points: { 
			show: false 
		},
		grid: {
	       	backgroundColor: '#fff'
	   	},
		colors: [ '#f00' ],
		yaxis: { 
			min: 0,
			tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals); },
			labelWidth: 90
		},
		xaxis: {
			min: 0,
	    	tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + "\""; }
		}
	};
	
	$.plot($("#loss"), [ loss ], options);

	// === Upstream graph:
	
	var options = {
	    lines: { 
			show: true, 
			lineWidth: 1,
			fill: true
		},
		label: 'Bitrate',
	    points: { 
			show: false 
		},
		grid: {
	       	backgroundColor: '#fff'
	   	},
		colors: [ '#f00' ],
		yaxis: { 
			min: 0,
			tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + " Mbps"; },
			labelWidth: 90
		},
		xaxis: {
			min: 0,
	    	tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + "\""; }
		}
	};
	
	$.plot($("#bitrateUp"), [ bitrateUp ], options);

	// === Downstream graph:

	var options = {
	    lines: { 
			show: true, 
			lineWidth: 1,
			fill: true
		},
		label: 'Bitrate',
	    points: { 
			show: false 
		},
		grid: {
	       	backgroundColor: '#fff'
	   	},
		colors: [ '#f00' ],
		yaxis: { 
			min: 0,
			tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + " Mbps"; },
			labelWidth: 90
		},
		xaxis: {
			min: 0,
	    	tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + "\""; }
		}
	};
	
	$.plot($("#bitrateDown"), [ bitrateDown ], options);

});
</script>