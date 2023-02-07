#!/bin/bash

#get script directory
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

#change to script directory
cd $DIR

#create a file to store the keycodes
touch keycodes.txt

#copy udevmon.yml
cp udevmon.yml cpudevmon.yml

#replace keyNum with $1 and KBDName with $2 in cpudevmon.yml
sed -i "s/keyNum/$1/g" cpudevmon.yml
sed -i "s/KBDName/$2/g" cpudevmon.yml

#stop current udevmon service run new with cpudevmon.yml then restart udevmon service
pkexec -e systemctl stop udevmon && udevmon -c cpudevmon.yml && systemctl start udevmon

#remove cpudevmon.yml
rm cpudevmon.yml
