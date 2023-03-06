#!/bin/bash

#check if the number of arguments is 2
if [ $# -ne 1 ]; then
    echo "Usage: udevmon.sh <number of keycodes> <keyboard name>"
    exit 1
fi

#get script directory
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
#change to script directory
cd $DIR

#stop udevmon service
systemctl stop udevmon
#grab keycodes
udevmon -c "$DIR/cpudevmon.yml" &
#wait till keycodes.txt has as many lines as the number of keycodes
while [ $(wc -l < keycodes.txt) -lt $1 ]; do sleep 0.5; done;
#restart udevmon service
systemctl restart udevmon
