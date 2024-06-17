#!/bin/bash

if [ ! -e "/home/ark/.config/.NOVERBALBATTERYWARNING" ]; then
  batt_life_verbal_warning.py &
fi