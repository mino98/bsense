<?php include("../header.inc.php"); ?>

<div id="inside" class="clearfloat">
	<div id="page-title" class="clearfloat">
	<h1>Your broadband statistics</h1>
	</div>
	<div id="page-content">
	<!-- Content start -->
	
<?php

	//=== Include configuration values and functions:
	require("../includes/config.inc.php");
	require("../includes/functions.inc.php");

	//=== Database connection:
	global $db;
	$db = @pg_connect("dbname=".$_config['databaseName']." user=".$_config['databaseUser']." password=".$_config['databasePass']);
	if(!$db)
		die("<div class=\"error\"><strong>Internal error:</strong> code #1001.<br />
			Feel free to provide us feedback by using the form on the right. Thank you.</div>");

	// Verify if the UUID has a valid format:
	$UUID = trim($_REQUEST['UUID']);	
	if(!preg_match('/^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$/', $UUID))
		die("ERROR: the supplied UUID is not valid");

	$sql = "SELECT 
	 			\"Timestamp\",
				\"MeasurementID\"
			FROM \"RawMeasurementData\"
			WHERE \"UUID\" = '".addslashes($UUID)."' 
			ORDER BY \"Timestamp\" DESC;";
	$measurements = pg_query($sql);
	
	if(pg_num_rows($measurements) == 0) {
		echo "<p>There are <strong>no recorded measurements</strong> for your line. Measurements are normally taken every 15 minutes, while 
			you are connected to the Internet.<p>";

		echo "<p>If the problem persist, please make sure you are running the <strong>latest version of the software</strong> (which you can download from 
			<a href='/broadbandtest.php'>this page</a>) or <strong>get in touch with us</strong> using the form on the right column.</p>";
			
		echo "<p>Thanks for your help.</p>";

	} else {

		echo "
			<p>The following measurements have been recorded on your line:</p>

			<div id='placeholder' style='width:100%; height:300px; display: none'></div>
		
			<div style='height:500px; width:100%; overflow:auto; border: 0px; padding: 8px;'>
				<table class='data-table'>
					<tr>
						<th class='data-th' width='8%'>#</th>
						<th class='data-th'>Date and time:</th>
						<th class='data-th'>Plot:</th>
					</tr>";

		// for each measurement:
		$i = 0;
		while($measurement = pg_fetch_array($measurements)) {
			$i++;

			echo "<tr class=\"".(($i % 2) ? 'data-even-tr' : 'data-odd-tr')."\">";

			echo "<td class='data-td'>".$i."</td>";
			echo "<td class='data-td'>".date("d/m/Y H:i", strtotime($measurement['Timestamp']))."</td>";
			echo "<td class='data-td'>
				<input type='button' class='plotUpstream' value='Upload' id='".$measurement['MeasurementID']."'>
				<input type='button' class='plotDownstream' value='Download' id='".$measurement['MeasurementID']."'>
				<input type='button' class='plotLatency' value='Latency' id='".$measurement['MeasurementID']."'>
				<!-- <input type='button' class='plotLoss' value='Packet Loss' id='".$measurement['MeasurementID']."'> -->
				</td>";

			echo "</tr>";
		}
		
		echo "</table></div>";
	}

	// Close connection with the database:
	pg_close($db);

?>
	<script id="source" language="javascript" type="text/javascript">
	$(function () {
		

		//== Plot latency graph:
	    $("input.plotLatency").click(function () {
			var button = this;
			$("#placeholder").slideDown('fast', function() {
				var MeasurementID = button.id;
			
				var options = {
					legend: { noColumns: 2 },
			        lines: { show: true, lineWidth: 2 },
			        points: { show: false },
					colors: [ '#090', '#900' ],
					yaxis: { 
						min: 0,
						tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + " ms"; },
						labelWidth: 60
					},
					xaxis: {
						min: 0,
				    	tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + "\""; }
					}
			    };
		
		        function onDataReceived(series) {
		            var data = [];
					data.push(series[0]);
					data.push(series[1]);
		            $.plot($("#placeholder"), data, options);
		         }

		        $.ajax({
		            url: 'measurementData.php?type=latency&MeasurementID='+MeasurementID,
		            method: 'GET',
		            dataType: 'json',
		            success: onDataReceived
		        });
			});
	    });


		//== Plot packetloss graph:
	    $("input.plotLoss").click(function () {
			var button = this;
			$("#placeholder").slideDown('fast', function() {
				var MeasurementID = button.id;
			
				var options = {
			        lines: { show: true, lineWidth: 2 },
			        points: { show: false },
					colors: [ '#090' ],
					yaxis: { 
						min: 0,
						tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + " %"; },
						labelWidth: 60
					},
					xaxis: {
						min: 0,
				    	tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + "\""; }
					}
			    };
		
		        function onDataReceived(series) {
		            var data = [];
					data.push(series);
		            $.plot($("#placeholder"), data, options);
		         }

		        $.ajax({
		            url: 'measurementData.php?type=loss&MeasurementID='+MeasurementID,
		            method: 'GET',
		            dataType: 'json',
		            success: onDataReceived
		        });
		    });
		});
		

		//== Plot Upstream graph:
	    $("input.plotUpstream").click(function () {
			var button = this;
			$("#placeholder").slideDown('fast', function() {
				var MeasurementID = button.id;
			
				var options = {
			        lines: { show: true, lineWidth: 1, fill: true },
			        points: { show: false },
					colors: [ '#090' ],
					yaxis: { 
						min: 0,
						tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + " Mbps"; },
						labelWidth: 60
					},
					xaxis: {
						min: 0,
				    	tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + "\""; }
					}
			    };
		
		        function onDataReceived(series) {
		            var data = [];
					data.push(series);
		            $.plot($("#placeholder"), data, options);
		         }

		        $.ajax({
		            url: 'measurementData.php?type=upstream&MeasurementID='+MeasurementID,
		            method: 'GET',
		            dataType: 'json',
		            success: onDataReceived
		        });
		    });
		});
	
		//== Plot Downstream graph:
	    $("input.plotDownstream").click(function () {
			var button = this;
			$("#placeholder").slideDown('fast', function() {
				var MeasurementID = button.id;
			
				var options = {
			        lines: { show: true, lineWidth: 1, fill: true },
			        points: { show: false },
					colors: [ '#090' ],
					yaxis: { 
						min: 0,
						tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + " Mbps"; },
						labelWidth: 60
					},
					xaxis: {
						min: 0,
				    	tickFormatter: function suffixFormatter(val, axis) { return val.toFixed(axis.tickDecimals) + "\""; }
					}
			    };
		
		        function onDataReceived(series) {
		            var data = [];
					data.push(series);
		            $.plot($("#placeholder"), data, options);
		         }

		        $.ajax({
		            url: 'measurementData.php?type=downstream&MeasurementID='+MeasurementID,
		            method: 'GET',
		            dataType: 'json',
		            success: onDataReceived
		        });
		    });
		});
		
		
	});
	</script>
	
	
	</div>
	<?php include("../sidebar.inc.php"); ?>
</div>

<!-- Content end -->
<?php include("../footer.inc.php"); ?>
