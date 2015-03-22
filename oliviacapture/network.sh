#!sh

tcpdump -i lo0 udp > a.log &
sleep 5
killall tcpdump

wc -l a.log
