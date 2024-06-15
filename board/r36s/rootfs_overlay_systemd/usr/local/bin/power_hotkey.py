#!/usr/bin/env python3

import evdev
import asyncio
import time
from subprocess import check_output

intput_device = evdev.InputDevice("/dev/input/event0")

class Events:
    power = 116

def runcmd(cmd, *args, **kw):
    print(f">>> {cmd}")
    check_output(cmd, *args, **kw)

async def handle_event(device):
    async for event in device.async_read_loop():
        if event.code == Events.power:
            runcmd("poweroff -f", shell=True)
            exit

def run():
    asyncio.ensure_future(handle_event(intput_device))

    loop = asyncio.get_event_loop()
    loop.run_forever()

if __name__ == "__main__": # admire
    run()
