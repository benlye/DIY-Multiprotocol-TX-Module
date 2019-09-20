#!/bin/bash

# Relative path to the firmware source
SRCPATH=../Multiprotocol

# Path to sed - needed for the option functions
SED=$(which gsed || which sed)

export PATH=$PATH:$HOME/arduino-ide

# Function to enable a firmware option
optenable() {
    for opt in "$@" ; do
        eval "${SED} -i 's/\/\{2,\}[[:blank:]]*\(#define[[:blank:]]*\b${opt}\b\)/\1/g' $SRCPATH/_Config.h"
    done
}

# Function to disable a firmware option
optdisable() {
    for opt in "$@" ; do
        eval "${SED} -i 's/^\([[:blank:]]*\)\(#define[[:blank:]]*\b${opt}\b\)/\1\/\/\2/g' $SRCPATH/_Config.h"
    done
}

# Function to set a firmware option
optset() {
    eval "${SED} -i 's/\(#define \b${1}\b\).*$/\1 ${2}/g' $SRCPATH/_Config.h"
}

# Function to add a firmware option
optadd() {
    eval "echo \"#define ${1} ${2}\" >>$SRCPATH/_Config.h"
}

# Function to build the firmware
buildMulti() { exitcode=0; BUILDCMD="arduino-cli compile -b $BOARD $SRCPATH/Multiprotocol.ino --build-path /tmp/build/"; echo $BUILDCMD; $BUILDCMD; if [ $? -ne 0 ]; then exitcode=1; fi; echo; return $exitcode; }

# Get all the protocol names
A7105_PROTOCOLS=$(sed -n 's/[\/\/]*[[:blank:]]*#define[[:blank:]]*\([[:alnum:]_]*_A7105_INO\)\(.*\)/\1/p' $SRCPATH/_Config.h)
CC2500_PROTOCOLS=$(sed -n 's/[\/\/]*[[:blank:]]*#define[[:blank:]]*\([[:alnum:]_]*_CC2500_INO\)\(.*\)/\1/p' $SRCPATH/_Config.h)
CYRF6936_PROTOCOLS=$(sed -n 's/[\/\/]*[[:blank:]]*#define[[:blank:]]*\([[:alnum:]_]*_CYRF6936_INO\)\(.*\)/\1/p' $SRCPATH/_Config.h)
NRF24L01_PROTOCOLS=$(sed -n 's/[\/\/]*[[:blank:]]*#define[[:blank:]]*\([[:alnum:]_]*_NRF24L01_INO\)\(.*\)/\1/p' $SRCPATH/_Config.h)
ALL_PROTOCOLS=$(echo $A7105_PROTOCOLS $CC2500_PROTOCOLS $CYRF6936_PROTOCOLS $NRF24L01_PROTOCOLS);

# Get the firmware version number
MAJOR_VERSION=$(grep "VERSION_MAJOR" "$SRCPATH/Multiprotocol.h" | awk -v N=3 '{gsub(/\r/,""); print $N}')
MINOR_VERSION=$(grep "VERSION_MINOR" "$SRCPATH/Multiprotocol.h" | awk -v N=3 '{gsub(/\r/,""); print $N}')
REVISION_VERSION=$(grep "VERSION_REVISION" "$SRCPATH//Multiprotocol.h" | awk -v N=3 '{gsub(/\r/,""); print $N}')
PATCH_VERSION=$(grep "VERSION_PATCH" "$SRCPATH//Multiprotocol.h" | awk -v N=3 '{gsub(/\r/,""); print $N}')
MULTI_VERSION=$MAJOR_VERSION.$MINOR_VERSION.$REVISION_VERSION.$PATCH_VERSION

# Make a copy of _Config.h
cp $SRCPATH/_Config.h $SRCPATH/_Config.h.bak

# Set the board for the STM32 builds
BOARD="multi4in1:STM32F1:multistm32f103c:debug_option=none"

### Build for the JP4IN1 for OpenTX ###
printf "\n\e[92mBuilding firmware v$MULTI_VERSION for the Jumper T16 JP4IN1 module\e[0m\n\n"

#// Forced CC2500 tuning for CC2500
optenable FORCE_FRSKYD_TUNING
optset FORCE_FRSKYD_TUNING -15
optenable FORCE_FRSKYX_TUNING
optset FORCE_FRSKYX_TUNING -15

# // Disable PPM
optdisable ENABLE_PPM

# // Force the same global ID as other modules
optenable FORCE_GLOBAL_ID
optset FORCE_GLOBAL_ID 0x65428639

buildMulti

# Copy the binary
cp /tmp/build/Multiprotocol.ino.bin ./firmware/multi-stm-jp4in1-${MULTI_VERSION}.bin

# Restore _Config.h
cp $SRCPATH/_Config.h.bak $SRCPATH/_Config.h

### Build for the T16 internal module ###
printf "\n\e[92mBuilding firmware v$MULTI_VERSION for the Jumper T16 internal module\e[0m\n\n"

#// Forced CC2500 tuning for CC2500
optenable FORCE_FRSKYD_TUNING
optset FORCE_FRSKYD_TUNING -8
optenable FORCE_FRSKYX_TUNING
optset FORCE_FRSKYX_TUNING -8

# // Disable PPM
optdisable ENABLE_PPM

# // Disable INVERT_TELEMETRY
optdisable INVERT_TELEMETRY

# // Force the same global ID as other modules
optenable FORCE_GLOBAL_ID
optset FORCE_GLOBAL_ID 0x65428639

buildMulti

# Copy the binary
cp /tmp/build/Multiprotocol.ino.bin ./firmware/multi-stm-t16int-${MULTI_VERSION}.bin

# Restore _Config.h
cp $SRCPATH/_Config.h.bak $SRCPATH/_Config.h

### Build for the Banggood STM32 module in the 9XR Pro ###
printf "\n\e[92mBuilding firmware v$MULTI_VERSION for the Banggood STM32 module for the 9XR Pro\e[0m\n\n"

#// Forced CC2500 tuning for CC2500
optenable FORCE_FRSKYD_TUNING
optset FORCE_FRSKYD_TUNING 23
optenable FORCE_FRSKYX_TUNING
optset FORCE_FRSKYX_TUNING 23

# // Disable PPM
optdisable ENABLE_PPM

# // Configure telemetry
optdisable INVERT_TELEMETRY
optdisable MULTI_TELEMETRY
optenable MULTI_STATUS

# // Force the same global ID as other modules
optenable FORCE_GLOBAL_ID
optset FORCE_GLOBAL_ID 0x65428639

buildMulti

# Copy the binary
cp /tmp/build/Multiprotocol.ino.bin ./firmware/multi-stm-bg9xr-${MULTI_VERSION}.bin

# Restore _Config.h
cp $SRCPATH/_Config.h.bak $SRCPATH/_Config.h

# Set the board for the Atmega AVR build
BOARD="multi4in1:avr:multiatmega328p:bootloader=optiboot"

### Build for the Banggood AVR module in the 9XR Pro ###
printf "\n\e[92mBuilding firmware v$MULTI_VERSION for the Banggood AVR module for the 9XR Pro\e[0m\n\n"

#// Forced CC2500 tuning for CC2500
optenable FORCE_FRSKYD_TUNING
optset FORCE_FRSKYD_TUNING -51
optenable FORCE_FRSKYX_TUNING
optset FORCE_FRSKYX_TUNING -51

# // Disable PPM
optdisable ENABLE_PPM

# // Configure telemetry
optdisable INVERT_TELEMETRY
optdisable MULTI_TELEMETRY
optenable MULTI_STATUS

# // Force the same global ID as other modules
optenable FORCE_GLOBAL_ID
optset FORCE_GLOBAL_ID 0x65428639

# // Disable all the protocols
optdisable $ALL_PROTOCOLS

# // Enable selected protocols
optenable FLYSKY_A7105_INO
optenable FRSKYD_CC2500_INO
optenable FRSKYX_CC2500_INO
optenable MJXQ_NRF24L01_INO
optenable H8_3D_NRF24L01_INO

buildMulti

# Copy the binary
cp /tmp/build/Multiprotocol.ino.bin ./firmware/multi-avr-bg9xr-${MULTI_VERSION}.bin

# Restore _Config.h
cp $SRCPATH/_Config.h.bak $SRCPATH/_Config.h

### Build for the Banggood AVR module in the T16 ###
printf "\n\e[92mBuilding firmware v$MULTI_VERSION for the Banggood AVR module for the T16\e[0m\n\n"

#// Forced CC2500 tuning for CC2500
optenable FORCE_FRSKYD_TUNING
optset FORCE_FRSKYD_TUNING -51
optenable FORCE_FRSKYX_TUNING
optset FORCE_FRSKYX_TUNING -51

# // Disable PPM
optdisable ENABLE_PPM

# // Force the same global ID as other modules
optenable FORCE_GLOBAL_ID
optset FORCE_GLOBAL_ID 0x65428639

# // Disable all the protocols
optdisable $ALL_PROTOCOLS

# // Enable selected protocols
optenable FLYSKY_A7105_INO
optenable FRSKYD_CC2500_INO
optenable FRSKYX_CC2500_INO
optenable MJXQ_NRF24L01_INO
optenable H8_3D_NRF24L01_INO
optenable SCANNER_CC2500_INO

buildMulti

# Copy the binary
cp /tmp/build/Multiprotocol.ino.bin ./firmware/multi-avr-bgt16-${MULTI_VERSION}.bin

# Restore _Config.h
cp $SRCPATH/_Config.h.bak $SRCPATH/_Config.h

# Clean up
rm $SRCPATH/_Config.h.bak
rm $SRCPATH/Multiprotocol.ino.multi4in1*.elf
rm $SRCPATH/Multiprotocol.ino.multi4in1*.bin
