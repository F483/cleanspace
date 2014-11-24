#!/bin/bash
# Copyright (c) 2014 Fabian Barkhau <fabian.barkhau@gmail.com>
# License: MIT (see LICENSE.TXT file)

# make sure user has root rights
if [ "$(whoami)" != "root" ]; then
    echo "You don't have sufficient privileges!"
    exit 1
fi

# install devkitPro for building gba games
CURRENT_DIRECTORY=${PWD}
cd /tmp
perl ${CURRENT_DIRECTORY}/bin/devkitARMupdate.pl
cd ${CURRENT_DIRECTORY}
chown ${USER}:${USER} -R $(eval echo ~${SUDO_USER})/devkitPro/

