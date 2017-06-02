<?php

//=== Show the specified user on a map:

// Verify if the UUID has a valid format:
$UUID = trim($_REQUEST['UUID']);	
if(!preg_match('/^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$/', $UUID))
	die("ERROR: the supplied UUID is not valid");

// Extract the user's postcode from the database:
$sql = "SELECT \"Postcode\"
		FROM \"BroadbandUsers\"
		WHERE \"UUID\"='".addslashes($UUID)."';";
$users = pg_query($sql);
if(pg_num_rows($users) != 1)
	die("ERROR: user not found");

$user = pg_fetch_array($users);
$postcode = $user['Postcode'];

echo "
	<p><a href=\"?\">[&lt;&lt; Back to the users list]</a></p>
	<h3>User location:</h3>

	<script src=\"http://maps.google.com/maps?file=api&amp;v=2&amp;sensor=false&amp;key=ABQIAAAAZfdatJWENe7m30FS5er5NRQ5EYiUoniHHG0T5rncCcJPOM1YDRSSpN27o3ITYcMkzxxQGfq6NQyeMw\" type=\"text/javascript\"></script>
	<div id=\"map_canvas\" style=\"width: 900px; height: 600px\"></div>
	<script type=\"text/javascript\">
		if (GBrowserIsCompatible()) {
	    	var map = new GMap2(document.getElementById(\"map_canvas\"));
			map.setMapType(G_HYBRID_MAP);
			map.addControl(new GSmallMapControl());
			map.addControl(new GMapTypeControl());

			geocoder = new GClientGeocoder();
		 	geocoder.getLatLng(
				\"".$postcode."\",
				function(point) {
					if (!point) {
						alert(address + \" not found\");
					} else {
						map.setCenter(point, 15);
						var marker = new GMarker(point);
						map.addOverlay(marker);
						marker.openInfoWindowHtml(address);
	            	}
				}
	          );
		}
	</script>
";

?>
