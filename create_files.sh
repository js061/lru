#!/bin/zsh
for i in {1..100}
do
	head -c 1M </dev/urandom > files/1M_$i.txt
done
for i in {1..100}
do
	head -c 2M </dev/urandom > files/2M_$i.txt
done
head -c 120k </dev/urandom > files/small.txt
head -c 1M </dev/urandom > files/1M.txt
head -c 10M </dev/urandom > files/10M.txt
head -c 20M </dev/urandom > files/20M.txt
head -c 40M </dev/urandom > files/40M.txt
head -c 100M </dev/urandom > files/100M.txt
# head -c 2G </dev/urandom > files/2G.txt
# head -c 4G </dev/urandom > files/4G.txt
echo DONE
