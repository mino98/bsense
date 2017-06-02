<?php

//----------------------------------------------------------------------------------------------------------------------------
//=== CONFIGURATION PARAMETERS:

// Database parameters:
$_config['databaseName'] = "broadbandforall";
$_config['databaseUser'] = "broadbandforall";
$_config['databasePass'] = "pass";

// Our contact email:
$_config['contactEmail'] = 'info@broadbandforall.net';

// Fullpath to the ITGSend binary:
$_config['ITGSend'] = "/opt/D-ITG/bin/ITGSend";

// Parameters for the remote ITGSend in the upstream test (one per line, do NOT use "-rp" here as it's automatically added):
// Experiment 1: "ping + UDP"
$_config['upstreamSendParameters'][1][] = "-a edinburgh.t.broadbandforall.net -T UDP -C 10 -c 56 -d 0 -t 60000 -m rttm -Sdp 3000";		// ping test for a minute
$_config['upstreamSendParameters'][1][] = "-a edinburgh.t.broadbandforall.net -T UDP -C 3200 -c 1024 -d 60000 -t 15000 -m rttm -Sdp 3000";	// 25600 Mbps offered throughput for a minute
// Experiment 2: "ping + TCP"
$_config['upstreamSendParameters'][2][] = "-a edinburgh.t.broadbandforall.net -T UDP -C 10 -c 56 -d 0 -t 60000 -m rttm -Sdp 3000";		// ping test for a minute
$_config['upstreamSendParameters'][2][] = "-a edinburgh.t.broadbandforall.net -T TCP -C 3200 -c 1024 -d 60000 -t 15000 -m rttm -Sdp 3000";	// 25600 Mbps offered throughput for a minute

// Parameters for the local ITGSend in the downstream test (one per line, do NOT use "-sp" nor "-Ssp" here as they are automatically added):
// Experiment 1: "UDP saturation"
$_config['downstreamSendParameters'][1][] = "-H -T UDP -C 3200 -c 1024 -d 0 -t 15000 -rp 4000";
// Experiment 2: "TCP saturation"
$_config['downstreamSendParameters'][2][] = "-H -T TCP -C 400 -c 1024 -d 0 -t 15000 -rp 4000";
$_config['downstreamSendParameters'][2][] = "-H -T TCP -C 400 -c 1024 -d 0 -t 15000 -rp 4001";
$_config['downstreamSendParameters'][2][] = "-H -T TCP -C 400 -c 1024 -d 0 -t 15000 -rp 4002";
$_config['downstreamSendParameters'][2][] = "-H -T TCP -C 400 -c 1024 -d 0 -t 15000 -rp 4003";
$_config['downstreamSendParameters'][2][] = "-H -T TCP -C 400 -c 1024 -d 0 -t 15000 -rp 4004";
$_config['downstreamSendParameters'][2][] = "-H -T TCP -C 400 -c 1024 -d 0 -t 15000 -rp 4005";
$_config['downstreamSendParameters'][2][] = "-H -T TCP -C 400 -c 1024 -d 0 -t 15000 -rp 4006";
$_config['downstreamSendParameters'][2][] = "-H -T TCP -C 400 -c 1024 -d 0 -t 15000 -rp 4007";

// Parameters for the remote ITGRecv in the downstream test (do NOT use "-Sp" here as it's automatically added):
// Experiment 1:
$_config['downstreamRecvParameters'][1] = "-H edinburgh.t.broadbandforall.net";
// Experiment 2:
$_config['downstreamRecvParameters'][2] = "-H edinburgh.t.broadbandforall.net";
//----------------------------------------------------------------------------------------------------------------------------


?>
