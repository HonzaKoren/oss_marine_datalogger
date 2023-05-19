#!/bin/bash

diff -qb /dev/sda1 /dev/sdb1 2>&1>/dev/null
SYNC_NEEDED=$?
CURRENT_BOOT=$(mount | grep /boot | cut -f1 -d" ")

if [ $SYNC_NEEDED -eq 1 ]
then
        # An updated version of /boot is present
        if [ "$CURRENT_BOOT" != "/dev/sda1" -a "$CURRENT_BOOT" != "/dev/sdb1" ]
        then
                echo "Unable to detect mounted boot. Cannot decide, what is the source and the target."
                exit 1
        else
                if [ "$CURRENT_BOOT" = "/dev/sda1" ]
                then
                        echo Syncing sda1 to sdb1
                        TARGET=/dev/sdb1
                else
                        echo Syncing sdb1 to sda1
                        TARGET=/dev/sda1
                fi

        fi
fi

# Sync
umount /boot
fsck.vfat $CURRENT_BOOT
cat $CURRENT_BOOT > $TARGET
mount -a
