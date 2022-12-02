#!/bin/zsh
for i in {1..100}
do
  wc ../files/1M_$i.txt
done
echo DONE
