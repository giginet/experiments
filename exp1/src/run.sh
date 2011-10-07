#!/bin/sh

i=0
while [ $i -le 10 ]
do
  echo "$i"
  echo "$i" | ./A-society
  mv log log.$i
  i=`expr $i + 1`
done
