#!/bin/bash

#get script directory
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

#change to script directory
cd $DIR

#remove keycodes.txt
rm keycodes.txt
#remove cpudevmon.yml
rm cpudevmon.yml
