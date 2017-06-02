<?php

/**
 *
 *  RIPE NCC RIS WHOIS Bulk Query Interface
 *
 */

function doRISLookup($ip) {
	$risserver = 'riswhois.ripe.net'; // RIPE NCC RISwhois Server (public)
	$risport = 43;                    // Port to which RIPE NCC RISwhois listens
	$socket_timeout = 5;             // Timeout for socket connection operations
	$socket_delay = 5;                // Timeout for socket read/write operations
	$buffer = '';

	// Create a new socket:
	$sock = @stream_socket_client("tcp://".$risserver.":".$risport, $errno, $errstr, $socket_timeout);
	if (!$sock) {
	   return false;
	}

	stream_set_blocking($sock, 0); // Set stream to non-blocking
	stream_set_timeout($sock, $socket_delay); // Set stream delay timeout

	// Build, then submit query:
	$request = "-k -1 -M\n";
	$request .= $ip."\n";
	$request .= " -k\n";
	fwrite($sock, $request);

	// Keep looking for more responses until EOF or timeout
	$before_query = date('U');
	while(!feof($sock)){
	   if($buf=fgets($sock,128)){
	     $buffer .= $buf;
	     if (date('U') > ($before_query + $socket_timeout)) break;
	  }
	}

	fclose($sock);

	$resp = explode("\n",$buffer);
	$response = array();
	foreach ($resp as $r) {

		if (preg_match('/origin:\s+AS(\d+)/i', $r, $matcher)) {
			$response['AS'] = $matcher[1]; 
		} else if (stristr($r,'descr')) {
		    $matcher = explode(":", $r);
			$matcher = trim($matcher[1]);
			
			// remove the first word, which is usually the AS name:
		    $response['Description'] = substr($matcher, strpos($matcher, " "));
		}
	}

	if(!$response['AS'] || empty($response['AS'])) $response['AS'] = "unknown";
	if(!$response['Description'] || empty($response['Description'])) $response['Description'] = "unknown";
	return $response;
}


?>