#!/bin/bash

#check if the number of arguments is 2
if [ $# -ne 2 ]; then
    echo "Usage: keycodegrab.sh <number of keycodes> <keyboard name>"
    exit 1
fi

#get script directory
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

#change to script directory
cd $DIR

#create a file to store the keycodes
touch keycodes.txt

#copy udevmon.yml
cp udevmon.yml cpudevmon.yml

#check if there is a config for udevmon
if [ - /etc/interception/udevmon.d ]; then
    #copy to backup
    cp -r /etc/interception/udevmon.d/ /etc/interception/udevmon.d.bak
fi

#replace keyNum with $1 and KBDName with $2 in cpudevmon.yml
sed -i "s/keyNum/$1/g" cpudevmon.yml
sed -i "s/KBDName/$2/g" cpudevmon.yml
#replace file with $DIR/keycodes.txt in cpudevmon.yml
sed -i "s|file|$DIR/keycodes.txt|g" cpudevmon.yml

#stop current udevmon service run new with cpudevmon.yml then restart udevmon service
pkexec bash -c "$DIR/udevmon.sh $1"
