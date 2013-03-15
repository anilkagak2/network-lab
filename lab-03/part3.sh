#! /bin/bash
# Script for running traceroute 5 times each for 
# packet sizes of 56, 2000 & 3200

pkt_size=(56 2000 3200)

for j in {0..2}
do
	printf 'traceroute jampui.iitg.ernet.in %s\n' "${pkt_size[j]}"
	for i in {1..5}
	do
		traceroute jampui.iitg.ernet.in ${pkt_size[j]}
	done
done
