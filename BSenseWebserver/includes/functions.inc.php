<?php

/**
 * generateUuid: generates a new RFC 4122-compliant uuid
 */
function generateUuid() {
    return sprintf('%04x%04x-%04x-%03x4-%04x-%04x%04x%04x',
        mt_rand(0, 65535), mt_rand(0, 65535), // 32 bits for "time_low"
        mt_rand(0, 65535), // 16 bits for "time_mid"
        mt_rand(0, 4095),  // 12 bits before the 0100 of (version) 4 for "time_hi_and_version"
        bindec(substr_replace(sprintf('%016b', mt_rand(0, 65535)), '01', 6, 2)),
            // 8 bits, the last two of which (positions 6 and 7) are 01, for "clk_seq_hi_res"
            // (hence, the 2nd hex digit after the 3rd hyphen can only be 1, 5, 9 or d)
            // 8 bits for "clk_seq_low"
        mt_rand(0, 65535), mt_rand(0, 65535), mt_rand(0, 65535) // 48 bits for "node"
    );  
}

/**
 * verifyUuid: verifies whether the given UUID is in database
 */
function verifyUuid($UUID) {
	global $_db, $_config;
	
	// Verify if the UUID has a valid format:
	if(!preg_match('/^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$/', $UUID))
		return false;

	// Verify if the UUID is in database:
	$sql = "SELECT \"UUID\"
			FROM \"BroadbandUsers\"
			WHERE \"UUID\"='".addslashes($UUID)."';";
	$results = pg_query($_db, $sql);
	if(pg_num_rows($results) != 1)
		return false;
		
	return true;	
}

/**
 * generatePortNumber: returns a port number for the experiments
 */
function generatePortNumber() {
	global $_db, $_config;

	$sql = "SELECT \"generatePortNumber\"(1)";
	$rows = pg_query($_db, $sql);
	$row = pg_fetch_array($rows);

	return $row[0];
}

/**
 * validateResults: performs sanity checks on the uploaded results 
 * to make sure they are valid before saving them in database
 */
function validateResults($results) {
	$lastTime = null;
	
	$rows = explode("\n", $results);
	foreach($rows as $row) {
		$columns = explode(" ", $row);
		
		// Each line must be 5 columns:
		if(count($columns) != 5) {
			return false;
		}
		
		// We check that time is monotonically increasing:
		
		if(!$lastTime)	// first row:
			$lastTime = $columns[0];
		else {
			if($columns[0] > $lastTime) {
				// time is increasing, which is fine:
				$lastTime = $columns[0];
			} else {
				// time is going back, stop here:
				return false;
			}
		}
	}

	return true;
}

/**
 * sanitizeResults: "sanitize" the logs for the bug of the early versions (<=0.5.4).
 */
function sanitizeResults($results) {
	$lastTime = null;
	$sanitizedResults = null;
	
	$rows = explode("\n", $results);
	foreach($rows as $row) {
		$columns = explode(" ", $row);
				
		if(!$lastTime)	// first row:
			$lastTime = $columns[0];
		else {
			if($columns[0] > $lastTime) {
				// time is increasing, keep these results:
				$sanitizedResults .= $row."\n";
				$lastTime = $columns[0];
			} else {
				// time is going back, reset the results:
				$sanitizedResults = $row."\n";
				$lastTime = $columns[0];
			}
		}
	}

	return $sanitizedResults;
}

/**
 * parseLatency: returns the average value for this experiment 
 */
function parseLatency($resultsTable) {	
	$sum = 0; $slots = 0;
	foreach($resultsTable as $row) {
		$sum += $row['delay'];
		$slots++;
		if($row['time'] >= 60) 
			break;
	}
	
	// calculate the average:
	return $sum / $slots;
}

/**
 * parseJitter: returns the average value for this experiment 
 */
function parseJitter($resultsTable) {
	$sum = 0; $slots = 0;
	foreach($resultsTable as $row) {
		$sum += $row['jitter'];
		$slots++;
		if($row['time'] >= 60) 
			break;
	}
	
	// calculate the average:
	return $sum / $slots;
}

/**
 * parsePacketLoss: returns the average value for this experiment 
 */
function parsePacketLoss($resultsTable) {
	$sum = 0; $slots = 0;
	foreach($resultsTable as $row) {
		$sum += $row['loss'];
		$slots++;
		if($row['time'] >= 60) 
			break;
	}
	
	// calculate the average:
	return $sum / $slots;
}

/**
 * parseUploadSpeed: returns the average value for this experiment 
 */
function parseUploadSpeed($resultsTable) {
	$sum = 0; $slots = 0;
	foreach($resultsTable as $row) {
		if($row['time'] < 60) 
			continue;
		$sum += $row['bitrate'];
		$slots++;
	}
	
	// calculate the average:
	if($slots == 0) 
		return 0;
	else
		$average = $sum / $slots; // in Kbps
		
	return $average / 1000; // in Mbps
}

/**
 * parseDownloadSpeed: returns the average value for this experiment 
 */
function parseDownloadSpeed($resultsTable) {
	$sum = 0; $slots = 0;
	foreach($resultsTable as $row) {
		$sum += $row['bitrate'];
		$slots++;
	}
	
	// calculate the average:
	if($slots == 0) 
		return 0;
	else
		$average = $sum / $slots; // in Kbps
		
	return $average / 1000; // in Mbps
}

/**
 * parseResults: return an array with the results 
 */
function parseResults($results) {
	$rows = explode("\n", $results);
	$ret = array();
	
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

		$parsed['time'] = floatval($columns[0]);
		$parsed['bitrate'] = floatval($columns[1]);
		$parsed['delay'] = floatval($columns[2]);
		$parsed['jitter'] = floatval($columns[3]);
		$parsed['loss'] = floatval($columns[4]);
		
		$ret[] = $parsed;
	}
	return $ret;
}

/**
 * percentile: calculates the $percentile percentile on the given array
 */
function percentile($data, $percentile){
	if(empty($data))
		return 0;

	$count = count($data);
	$allindex = ($count-1) * $percentile;
	$intvalindex = intval($allindex);
	$floatval = $allindex - $intvalindex;

	sort($data);
	
	$result = array();
	if(!is_float($floatval)){
	    $result = $data[$intvalindex];
	}else {
	    if($count > $intvalindex + 1)
	        $result = $floatval*($data[$intvalindex+1] - $data[$intvalindex]) + $data[$intvalindex];
	    else
	        $result = $data[$intvalindex];
	}
	
	return $result;
}

/**
 * Validate an email address.
 */
function validEmail($email) {
   $isValid = true;
   $atIndex = strrpos($email, "@");
   if (is_bool($atIndex) && !$atIndex) {
      $isValid = false;
   } else {
      $domain = substr($email, $atIndex+1);
      $local = substr($email, 0, $atIndex);
      $localLen = strlen($local);
      $domainLen = strlen($domain);
      if ($localLen < 1 || $localLen > 64)
      {
         // local part length exceeded
         $isValid = false;
      }
      else if ($domainLen < 1 || $domainLen > 255)
      {
         // domain part length exceeded
         $isValid = false;
      }
      else if ($local[0] == '.' || $local[$localLen-1] == '.')
      {
         // local part starts or ends with '.'
         $isValid = false;
      }
      else if (preg_match('/\\.\\./', $local))
      {
         // local part has two consecutive dots
         $isValid = false;
      }
      else if (!preg_match('/^[A-Za-z0-9\\-\\.]+$/', $domain))
      {
         // character not valid in domain part
         $isValid = false;
      }
      else if (preg_match('/\\.\\./', $domain))
      {
         // domain part has two consecutive dots
         $isValid = false;
      }
      else if (!preg_match('/^(\\\\.|[A-Za-z0-9!#%&`_=\\/$\'*+?^{}|~.-])+$/', str_replace("\\\\","",$local)))
      {
         // character not valid in local part unless 
         // local part is quoted
         if (!preg_match('/^"(\\\\"|[^"])+"$/',
             str_replace("\\\\","",$local)))
         {
            $isValid = false;
         }
      }
      if ($isValid && !(checkdnsrr($domain,"MX") || checkdnsrr($domain,"A")))
      {
         // domain not found in DNS
         $isValid = false;
      }
   }
   return $isValid;
}


?>