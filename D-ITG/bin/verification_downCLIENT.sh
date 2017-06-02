#!/bin/sh

# This script runs the various verification scenarios for the "down" tests, one after another.
#
# Instructions:
# 1) on the server, run the script:
#    ./verification_downSERVER.sh
#
# 2) on the client, run this script.
#    ./verification_downCLIENT.sh

# General parameters:
server=10.0.0.1

# Function that does the actual tests:
function doTests() {
	for i in {1..10}
	do
		sleep 3
		rm tmp/logfile 2> /dev/null
		./ITGRecv -H $server -Sp 3000 -l tmp/logfile > /dev/null 2> /dev/null
		bitrateUdp=`./ITGDec tmp/logfile 2>/dev/null | grep "Average bitrate" | tail -1 | sed -e "s|.*= *\([^ ]*\).*|\1|"`

		sleep 3
		rm tmp/logfile 2> /dev/null
		./ITGRecv -H $server -Sp 4000 -l tmp/logfile > /dev/null 2> /dev/null
		bitrateTcp=`./ITGDec tmp/logfile 2>/dev/null | grep "Average bitrate" | tail -1 | sed -e "s|.*= *\([^ ]*\).*|\1|"`

		echo "${scenario}\t${bitrateUdp}\t${bitrateTcp}"

	done
}

# Function that deletes ipfw rules:
function cleanupDummynet() {
	ipfw delete $uprulenumber
	ipfw delete $downrulenumber
}


echo "# scenario\tbitrateUdp\tbitrateTcp"

# ====================== Scenario 1 ====================== 
# Unlimited
scenario=1
doTests


# ====================== Scenario 2 ====================== 
# ADSL 2+ (ITU G.992.5 Annex M)
# 24Mbps / 3Mbps / 70ms rtt
downrulenumber=`ipfw add pipe 4 in src-ip $server | cut -d" " -f1`
ipfw pipe 4 config bw 24Mbit/s delay 35ms
uprulenumber=`ipfw add pipe 3 out dst-ip $server | cut -d" " -f1`
ipfw pipe 3 config bw 3Mbit/s delay 35ms

scenario=2
doTests
cleanupDummynet
	
	
# ====================== Scenario 3 ====================== 
# ADSL 2 (ITU G.992.3, aka "ADSL Max")
# 8Mbps / 1.3Mbps / 70ms rtt
downrulenumber=`ipfw add pipe 4 in src-ip $server | cut -d" " -f1`
ipfw pipe 4 config bw 8Mbit/s delay 35ms
uprulenumber=`ipfw add pipe 3 out dst-ip $server | cut -d" " -f1`
ipfw pipe 3 config bw 1300Kbit/s delay 35ms

scenario=3
doTests
cleanupDummynet


# ====================== Scenario 4 ====================== 
# Avanti
# 512Kbps / 256Kbps / 800ms
downrulenumber=`ipfw add pipe 4 in src-ip $server | cut -d" " -f1`
ipfw pipe 4 config bw 512Kbit/s delay 400ms
uprulenumber=`ipfw add pipe 3 out dst-ip $server | cut -d" " -f1`
ipfw pipe 3 config bw 256Kbit/s delay 400ms

scenario=4
doTests
cleanupDummynet


# ====================== Scenario 5 ====================== 
# "Exchange Activate"
# 512Kbps / 256Kbps / 70ms
downrulenumber=`ipfw add pipe 4 in src-ip $server | cut -d" " -f1`
ipfw pipe 4 config bw 512Kbit/s delay 35ms
uprulenumber=`ipfw add pipe 3 out dst-ip $server | cut -d" " -f1`
ipfw pipe 3 config bw 256Kbit/s delay 35ms

scenario=5
doTests
cleanupDummynet

