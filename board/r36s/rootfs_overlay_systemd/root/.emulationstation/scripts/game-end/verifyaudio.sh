#!/bin/bash

if [ ! -f "/boot/rk3566.dtb" ] && [ ! -f "/boot/rk3566-OC.dtb" ]; then
  DEVICE="rk3326"
else
  if [ "$(cat ~/.config/.DEVICE)" == "RG353V" ]; then
    DEVICE="rg353v"
  else
    DEVICE="rg503"
  fi
fi

# These next 2 gaurds are to address a potential issue in which .asoundrc and .asoundrcbak
# get potentially deleted or contentds deleted for some reason by the for loop.
if [ ! -e "/home/ark/.asoundrcbak" ] || [ $(stat -c %s "/home/ark/.asoundrcbak") = "0" ]; then
  cp -f /usr/local/bin/.asoundbackup/.asoundrcbak.${DEVICE} /home/ark/.asoundrcbak
  sudo chown ark:ark /home/ark/.asoundrcbak
fi

if [ ! -e "/home/ark/.asoundrc" ] || [ $(stat -c %s "/home/ark/.asoundrc") = "0" ]; then
  cp -f /home/ark/.asoundrcbak /home/ark/.asoundrc
  sudo chown ark:ark /home/ark/.asoundrc
  if [[ "$(tr -d '\0' < /proc/device-tree/compatible)" == *"rk3566"* ]]; then
    if [ "$(cat ~/.config/.DEVICE)" == "RGB30" ] || [ "$(cat ~/.config/.DEVICE)" == "RK2023" ] ; then
      amixer -q sset 'Playback Path' HP
    else
      amixer -q sset 'Playback Path' SPK
    fi
  fi
fi
