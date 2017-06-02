#!/bin/sh

# Parameters:
server=10.0.0.1
downlimit=10000
downdelay=50
uplimit=512
updelay=50
packetsize=1024
packetrates="10 20 30 40 50 60 70 80 90 100 200 400 800 1600 3200"
duration=60000

function cleanup() {
	echo "\n--------------------------------------------------------------------"
	echo "Cleaning up... \c"
	killall -9 ITGSend 2> /dev/null
	killall -9 ITGDec 2> /dev/null
	ipfw delete $uprulenumber
	ipfw delete $downrulenumber
	echo "done!"
	exit
}
trap cleanup INT

echo "Configuring dummynet: \c"

uprulenumber=`ipfw add pipe 3 out dst-ip $server | cut -d" " -f1`
ipfw pipe 3 config bw ${uplimit}Kbit/s delay ${updelay}ms

downrulenumber=`ipfw add pipe 4 in src-ip $server | cut -d" " -f1`
ipfw pipe 4 config bw ${downlimit}Kbit/s delay ${downdelay}ms

echo "done!"
echo "--------------------------------------------------------------------"

for packetrate in $packetrates
do
	for i in {1..5}
	do
		./ITGSend -a $server -C $packetrate -c $packetsize -t $duration -T UDP -m rttm -Sdp 3001 -rp 3005 -l congestion_udp/${packetrate}_${i}.log > /dev/null 2> /dev/null

		achieved=`./ITGDec congestion_udp/${packetrate}_${i}.log 2>/dev/null | grep "Average packet rate" | tail -1 | sed -e "s|.*= *\([^ ]*\).*|\1|"`
		rate=`./ITGDec congestion_udp/${packetrate}_${i}.log 2>/dev/null | grep "Average bitrate" | tail -1 | sed -e "s|.*= *\([^ ]*\).*|\1|"`

		echo "TEST: ${i}/5 - UP [${uplimit} Kbit/s - ${updelay} ms] - DOWN [${downlimit} Kbit/s - ${downdelay} ms] - PACKETSIZE: ${packetsize} bytes - PACKETRATE [set: ${packetrate} pkt/s - \c"
		echo "got: $achieved pkt/s] - RATE: $rate Kbit/s"
	done
	echo ""
done

cleanup
