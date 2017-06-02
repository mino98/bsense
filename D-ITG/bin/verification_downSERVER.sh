#!/bin/sh

# This script runs the various verification scenarios for the "down" tests, one after another.
#
# Instructions:
# 1) on the server, run the script:
#    ./verification_downCLIENT.sh
#
# 2) on the client, run this script.
#    ./verification_downSERVER.sh

# General parameters:
duration=15000

# Let's create the files containing the flows definition:
mkdir tmp 2> /dev/null
echo "-H -T UDP -C 400 -c 1024 -d 0 -t $duration -Ssp 3000 -rp 3100 -sp 3100" > tmp/scriptfile_udp
echo "-H -T UDP -C 400 -c 1024 -d 0 -t $duration -Ssp 3000 -rp 3101 -sp 3101" >> tmp/scriptfile_udp
echo "-H -T UDP -C 400 -c 1024 -d 0 -t $duration -Ssp 3000 -rp 3102 -sp 3102" >> tmp/scriptfile_udp
echo "-H -T UDP -C 400 -c 1024 -d 0 -t $duration -Ssp 3000 -rp 3103 -sp 3103" >> tmp/scriptfile_udp
echo "-H -T UDP -C 400 -c 1024 -d 0 -t $duration -Ssp 3000 -rp 3104 -sp 3104" >> tmp/scriptfile_udp
echo "-H -T UDP -C 400 -c 1024 -d 0 -t $duration -Ssp 3000 -rp 3105 -sp 3105" >> tmp/scriptfile_udp
echo "-H -T UDP -C 400 -c 1024 -d 0 -t $duration -Ssp 3000 -rp 3106 -sp 3106" >> tmp/scriptfile_udp
echo "-H -T UDP -C 400 -c 1024 -d 0 -t $duration -Ssp 3000 -rp 3107 -sp 3107" >> tmp/scriptfile_udp

echo "-H -T TCP -C 400 -c 1024 -d 0 -t $duration -Ssp 4000 -rp 4100 -sp 4100" > tmp/scriptfile_tcp
echo "-H -T TCP -C 400 -c 1024 -d 0 -t $duration -Ssp 4000 -rp 4101 -sp 4101" >> tmp/scriptfile_tcp
echo "-H -T TCP -C 400 -c 1024 -d 0 -t $duration -Ssp 4000 -rp 4102 -sp 4102" >> tmp/scriptfile_tcp
echo "-H -T TCP -C 400 -c 1024 -d 0 -t $duration -Ssp 4000 -rp 4103 -sp 4103" >> tmp/scriptfile_tcp
echo "-H -T TCP -C 400 -c 1024 -d 0 -t $duration -Ssp 4000 -rp 4104 -sp 4104" >> tmp/scriptfile_tcp
echo "-H -T TCP -C 400 -c 1024 -d 0 -t $duration -Ssp 4000 -rp 4105 -sp 4105" >> tmp/scriptfile_tcp
echo "-H -T TCP -C 400 -c 1024 -d 0 -t $duration -Ssp 4000 -rp 4106 -sp 4106" >> tmp/scriptfile_tcp
echo "-H -T TCP -C 400 -c 1024 -d 0 -t $duration -Ssp 4000 -rp 4107 -sp 4107" >> tmp/scriptfile_tcp

# Starts the sender process and keeps it running:
while [ 1 ]
do
	sleep 1
	echo "Starting ITGSend UDP..."
	./ITGSend tmp/scriptfile_udp -l /dev/null
	echo "-----------------------------------"

	sleep 1
	echo "Starting ITGSend TCP..."
	./ITGSend tmp/scriptfile_tcp -l /dev/null
	echo "-----------------------------------"
done


