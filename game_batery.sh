#!/bin/bash

set -e # to make sure nothing go wrong

#Check if it's the only one script running
if pidof -x $(basename $0) > /dev/null; then
  for p in $(pidof -x $(basename $0)); do
    if [ $p -ne $$ ]; then
      echo "Script $0 is already running: exiting"
      exit 1
    fi
  done
fi

usage()
{
  (echo "Usage: $0 number"
   echo "[number] must be greater than 0") 1>&2
}

if [ $# -le 0 ]
then
    usage
    echo "oi"
    exit 1
fi

if [ $1 -le 0 ]
then
  usage
  exit 1
fi

for i in `seq 1 $1`
do
    sleep 1
    echo "Starting match number $i"
    bash match.sh
    sleep 1
done

echo "Game batery finished! ($1 matches)"

