#!/bin/bash

# pull from git
git pull -C /etc/interception/layouts

# Get device names
DEVICES=$(pkexec libinput list-devices | grep Device)
