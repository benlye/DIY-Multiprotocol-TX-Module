#!/bin/bash

SED=$(which gsed || which sed)

SRCPATH=../Multiprotocol

optenable() {
    for opt in "$@" ; do
        eval "${SED} -i 's/\/\{2,\}[[:blank:]]*\(#define[[:blank:]]*\b${opt}\b\)/\1/g' $SRCPATH/_Config.h"
    done
}

optdisable() {
    for opt in "$@" ; do
        eval "${SED} -i 's/^\([[:blank:]]*\)\(#define[[:blank:]]*\b${opt}\b\)/\1\/\/\2/g' $SRCPATH/_Config.h"
    done
}

optset() {
    eval "${SED} -i 's/\(#define \b${1}\b\).*$/\1 ${2}/g' $SRCPATH/_Config.h"
}

optadd() {
    eval "echo \"#define ${1} ${2}\" >>$SRCPATH/_Config.h"
}

buildMulti() { exitcode=0; BUILDCMD="arduino --verify --board $BOARD $SRCPATH/Multiprotocol.ino --pref build.path=/tmp/build/"; echo $BUILDCMD; $BUILDCMD; if [ $? -ne 0 ]; then exitcode=1; fi; echo; return $exitcode; }

A7105_PROTOCOLS=$(sed -n 's/[\/\/]*[[:blank:]]*#define[[:blank:]]*\([[:alnum:]_]*_A7105_INO\)\(.*\)/\1/p' $SRCPATH/_Config.h)
CC2500_PROTOCOLS=$(sed -n 's/[\/\/]*[[:blank:]]*#define[[:blank:]]*\([[:alnum:]_]*_CC2500_INO\)\(.*\)/\1/p' $SRCPATH/_Config.h)
CYRF6936_PROTOCOLS=$(sed -n 's/[\/\/]*[[:blank:]]*#define[[:blank:]]*\([[:alnum:]_]*_CYRF6936_INO\)\(.*\)/\1/p' $SRCPATH/_Config.h)
NRF24L01_PROTOCOLS=$(sed -n 's/[\/\/]*[[:blank:]]*#define[[:blank:]]*\([[:alnum:]_]*_NRF24L01_INO\)\(.*\)/\1/p' $SRCPATH/_Config.h)
ALL_PROTOCOLS=$(echo $A7105_PROTOCOLS $CC2500_PROTOCOLS $CYRF6936_PROTOCOLS $NRF24L01_PROTOCOLS);

MAJOR_VERSION=$(grep "VERSION_MAJOR" "$SRCPATH/Multiprotocol.h" | awk -v N=3 'gsub(/\r/,""){print $N}')
MINOR_VERSION=$(grep "VERSION_MINOR" "$SRCPATH/Multiprotocol.h" | awk -v N=3 'gsub(/\r/,""){print $N}')
REVISION_VERSION=$(grep "VERSION_REVISION" "$SRCPATH//Multiprotocol.h" | awk -v N=3 'gsub(/\r/,""){print $N}')
PATCH_VERSION=$(grep "VERSION_PATCH" "$SRCPATH//Multiprotocol.h" | awk -v N=3 'gsub(/\r/,""){print $N}')
MULTI_VERSION=$MAJOR_VERSION.$MINOR_VERSION.$REVISION_VERSION.$PATCH_VERSION

# Make a copy of _Config.h
cp $SRCPATH/_Config.h $SRCPATH/_Config.h.bak

# Set the board for the STM32 builds
BOARD="multi4in1:STM32F1:multistm32f103c:debug_option=none"

### Build for the JP4IN1 for OpenTX ###

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

### Build for the Banggood STM32 module in the 9XR Pro ###

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

# // Enable the protocols
optenable FLYSKY_A7105_INO
optenable FRSKYD_CC2500_INO
optenable FRSKYX_CC2500_INO
optenable MJXQ_NRF24L01_INO
optenable H8_3D_NRF24L01_INO

buildMulti

# Copy the binary
cp /tmp/build/Multiprotocol.ino.hex ./firmware/multi-avr-bg9xr-${MULTI_VERSION}.hex

# Restore _Config.h
cp $SRCPATH/_Config.h.bak $SRCPATH/_Config.h

# Clean up
rm $SRCPATH/_Config.h.bak 