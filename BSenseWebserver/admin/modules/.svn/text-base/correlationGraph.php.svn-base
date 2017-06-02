<?php
//=== Graph of results for a specific user:

echo "
	<p><a href=\"?\">[&lt;&lt; Back to the users list]</a></p>
	<h3>Correlation graph of estimated/measurement speeds for ADSL users:</h3>
	Note: horizontal axis is measured speed, vertical is estimated speed.";

?>

<div id="correlationGraph" style="width:790px; height:700px; margin-top: 15px; margin-bottom: 15px"></div>

<script id="source" language="javascript" type="text/javascript">
$(function () {
	var points = [];
	
<?php

// x = measured
// y = expected

$sql = "SELECT u.\"DownloadSpeed\", e.\"EstimatedSpeed\"
		FROM \"BroadbandUsers\" AS u
		INNER JOIN \"BtAdslCheck\" AS e ON e.\"Postcode\"=u.\"Postcode\"
		WHERE u.\"TechnologyName\" = 'ADSL'
		AND \"DownloadSpeed\" > 0";
$users = pg_query($sql);

while($user = pg_fetch_array($users)) {
	echo "\tpoints.push([".$user['DownloadSpeed'].", ".$user['EstimatedSpeed']."]);\n";
}

?>
	//=== DownloadSpeed graph:
	var plot = $.plot($("#correlationGraph"), 
		[{
			data: points,
			color: '#f00',
		}],
		{
			yaxis: { 
				min: 0,
				max: 16,
				tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + " Mbps"; },
				labelWidth: 90
			},
			xaxis: {
				min: 0,
				max: 16,
				tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + " Mbps"; }
			},
			lines: {
				show: false
			},
		    points: { 
				show: true,
				radius: 3
			}
		});
		
	var tl = plot.pointOffset({ x: 0, y: 16});	// top left corner of the graph
	var tr = plot.pointOffset({ x: 16, y: 16});	// top right corner of the graph
	var bl = plot.pointOffset({ x: 0, y: 0});	// bottom left corner of the graph
	var br = plot.pointOffset({ x: 16, y: 0});	// bottom right corner of the graph

	var ctx = plot.getCanvas().getContext("2d");
	ctx.beginPath();
	ctx.moveTo(bl.left, bl.top);
	ctx.lineTo(tr.left, tr.top);
	ctx.lineTo(br.left, br.top);
	ctx.lineTo(bl.left, bl.top);
	ctx.fillStyle = "rgba(0, 0, 200, 0.3)";
	ctx.fill();
	ctx.beginPath();
	ctx.moveTo(bl.left, bl.top);
	ctx.lineTo(tr.left, tr.top);
	ctx.lineTo(tl.left, tl.top);
	ctx.lineTo(bl.left, bl.top);
	ctx.fillStyle = "rgba(200, 0, 0, 0.3)";
	ctx.fill();
	
	var o = plot.pointOffset({ x: 1, y: 15});
	$("#correlationGraph").append('<div style="position:absolute;top:'+ (o.top) +'px;left:'+ (o.left) +'px;color:#666;font-size:normal">Overestimated</div>');
	var o = plot.pointOffset({ x: 12, y: 2});
	$("#correlationGraph").append('<div style="position:absolute;top:'+ (o.top) +'px;left:'+ (o.left) +'px;color:#666;font-size:normal">Underestimated</div>');
	
});
</script>
