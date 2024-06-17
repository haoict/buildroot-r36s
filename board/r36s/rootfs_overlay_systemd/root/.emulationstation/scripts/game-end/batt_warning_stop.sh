#!/bin/bash

if [ ! -e "/home/ark/.config/.NOVERBALBATTERYWARNING" ]; then
  sudo killall python3
fi