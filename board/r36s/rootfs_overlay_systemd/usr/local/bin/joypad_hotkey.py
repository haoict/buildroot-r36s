#!/usr/bin/env python3

import evdev
import asyncio
import time
from subprocess import check_output

arkos_joypad = evdev.InputDevice("/dev/input/event2")

class Joypad:
    l1 = 310
    r1 = 311
    l2 = 312
    r2 = 313

    up = 544
    down = 545
    left = 546
    right = 547

    x = 307
    y = 308
    a = 304
    b = 305

    fn = 708
    select = 704
    start = 705

def runcmd(cmd, *args, **kw):
    print(f">>> {cmd}")
    check_output(cmd, *args, **kw)

async def handle_event(device):
    # event.code is the button number
    # event.value is 1 for press, 0 for release
    # event.type is 1 for button, 3 for axis
    async for event in device.async_read_loop():
        if device.name == "GO-Super Gamepad":
            keys = arkos_joypad.active_keys()
            if Joypad.fn in keys:
                if event.code == Joypad.select and event.value == 1:
                    runcmd("/usr/local/bin/writevt.elf /dev/tty1 'root\n'", shell=True)
                    exit
                if event.code == Joypad.start and event.value == 1:
                    runcmd("/usr/local/bin/writevt.elf /dev/tty1 'ls -la\n'", shell=True)
                    exit

def run():
    asyncio.ensure_future(handle_event(arkos_joypad))

    loop = asyncio.get_event_loop()
    loop.run_forever()

if __name__ == "__main__": # admire
    run()
