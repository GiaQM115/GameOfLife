#!/bin/bash

SPEED=0
DIRECTION="N"
PROB=35
SIZE=5
TREE=85
INITBURN=50
COUNTER=1
COUNTER2=1
LOW=0
MID=0
HIGH=0

rm averages.txt

while [ $SPEED -le 2 ]
do
	while [ $COUNTER -le 10 ]
	do
		./wildfire $SIZE $PROB $TREE $INITBURN $SPEED $DIRECTION
		COUNTER=$(( $COUNTER + 1 ))
		SIZE=$(( $SIZE + 2 ))
	done
	SPEED=$(( $SPEED + 1 ))
	SIZE=5
	COUNTER=1
done

while read LINE
do
	if [ $COUNTER2 -le 10 ]
	then
		LOW=$(( $LOW + $LINE ))
	elif [ $COUNTER2 -le 20 ]
	then
		MID=$(( $MID + $LINE ))
	else
		HIGH=$(( $HIGH + $LINE ))
	fi
	COUNTER2=$(( $COUNTER2 + 1 ))
done < averages.txt

echo "The average percent burned in low wind is $(( $LOW / 10 ))"
echo "The average percent burned in mid wind is $(( $MID / 10 ))"
echo "The average percent burned in high wind is $(( $HIGH / 10 ))"
