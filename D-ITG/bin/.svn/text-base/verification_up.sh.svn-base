#!/bin/sh

# This script runs the various verification scenarios for the "up" tests, one after another.
#
# Instructions:
# 1) on the server, start the command:
#    ./ITGRecv -l /dev/null -Sp 3000
#
# 2) on the client, run this script.
#

# General parameters:
server=10.0.0.1
duration=15000

# Let's create the files containing the flows definition:
mkdir tmp 2> /dev/null
echo "-a $server -C 10 -c 56 -t $duration -T UDP -m rttm -Sdp 3000 -rp 3100" > tmp/scriptfile_delay

echo "-a $server -C 400 -c 1024 -t $duration -T UDP -m rttm -Sdp 3000 -rp 3100" > tmp/scriptfile_udp
echo "-a $server -C 400 -c 1024 -t $duration -T UDP -m rttm -Sdp 3000 -rp 3101" >> tmp/scriptfile_udp
echo "-a $server -C 400 -c 1024 -t $duration -T UDP -m rttm -Sdp 3000 -rp 3102" >> tmp/scriptfile_udp
echo "-a $server -C 400 -c 1024 -t $duration -T UDP -m rttm -Sdp 3000 -rp 3103" >> tmp/scriptfile_udp
echo "-a $server -C 400 -c 1024 -t $duration -T UDP -m rttm -Sdp 3000 -rp 3104" >> tmp/scriptfile_udp
echo "-a $server -C 400 -c 1024 -t $duration -T UDP -m rttm -Sdp 3000 -rp 3105" >> tmp/scriptfile_udp
echo "-a $server -C 400 -c 1024 -t $duration -T UDP -m rttm -Sdp 3000 -rp 3106" >> tmp/scriptfile_udp
echo "-a $server -C 400 -c 1024 -t $duration -T UDP -m rttm -Sdp 3000 -rp 3107" >> tmp/scriptfile_udp

echo "-a $server -C 400 -c 1024 -t $duration -T TCP -m rttm -Sdp 3000 -rp 3100" > tmp/scriptfile_tcp
echo "-a $server -C 400 -c 1024 -t $duration -T TCP -m rttm -Sdp 3000 -rp 3101" >> tmp/scriptfile_tcp
echo "-a $server -C 400 -c 1024 -t $duration -T TCP -m rttm -Sdp 3000 -rp 3102" >> tmp/scriptfile_tcp
echo "-a $server -C 400 -c 1024 -t $duration -T TCP -m rttm -Sdp 3000 -rp 3103" >> tmp/scriptfile_tcp
echo "-a $server -C 400 -c 1024 -t $duration -T TCP -m rttm -Sdp 3000 -rp 3104" >> tmp/scriptfile_tcp
echo "-a $server -C 400 -c 1024 -t $duration -T TCP -m rttm -Sdp 3000 -rp 3105" >> tmp/scriptfile_tcp
echo "-a $server -C 400 -c 1024 -t $duration -T TCP -m rttm -Sdp 3000 -rp 3106" >> tmp/scriptfile_tcp
echo "-a $server -C 400 -c 1024 -t $duration -T TCP -m rttm -Sdp 3000 -rp 3107" >> tmp/scriptfile_tcp

# Function that does the actual tests:
function doTests() {
	for i in {1..10}
	do
		sleep 1
		rm tmp/logfile 2> /dev/null
		./ITGSend tmp/scriptfile_delay -l tmp/logfile > /dev/null 2> /dev/null
		delay=`./ITGDec tmp/logfile 2>/dev/null | grep "Average delay" | tail -1 | sed -e "s|.*= *\([^ ]*\).*|\1|"`
		jitter=`./ITGDec tmp/logfile 2>/dev/null | grep "Average jitter" | tail -1 | sed -e "s|.*= *\([^ ]*\).*|\1|"`
	
		sleep 1
		rm tmp/logfile 2> /dev/null
		./ITGSend tmp/scriptfile_udp -l tmp/logfile > /dev/null 2> /dev/null
		bitrateUdp=`./ITGDec tmp/logfile 2>/dev/null | grep "Average bitrate" | tail -1 | sed -e "s|.*= *\([^ ]*\).*|\1|"`

		sleep 1	
		rm tmp/logfile 2> /dev/null
		./ITGSend tmp/scriptfile_tcp -l tmp/logfile > /dev/null 2> /dev/null
		bitrateTcp=`./ITGDec tmp/logfile 2>/dev/null | grep "Average bitrate" | tail -1 | sed -e "s|.*= *\([^ ]*\).*|\1|"`

		echo "${scenario}\t${delay}\t${jitter}\t${bitrateUdp}\t${bitrateTcp}"
	done
}

# Function that deletes ipfw rules:
function cleanupDummynet() {
	ipfw delete $uprulenumber
	ipfw delete $downrulenumber
}


echo "# scenario\tdelay\tjitter\tbitrateUdp\tbitrateTcp"

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

