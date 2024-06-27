# Simple Launcher

## Buildroot: Quick build and copy to device
```bash
make simple-launcher-dirclean && make simple-launcher-rebuild && rsync -av output/build/simple-launcher-1.0/simple-launcher root@192.168.1.140:/usr/local/bin/.
```

## Compile for R36S from Arkos
```bash
# Fix build environment inside arkos
cd /home/ark
wget -t 3 -T 60 --no-check-certificate https://github.com/christianhaitian/arkos/raw/main/Headers/rg351-linux-headers-4.4.189_4.4.189-2_arm64.deb -O rg351-linux-headers-4.4.189_4.4.189-2_arm64.deb || rm -f rg351-linux-headers-4.4.189_4.4.189-2_arm64.deb
sudo dpkg -i --force-all rg351-linux-headers-4.4.189_4.4.189-2_arm64.deb
rm rg351-linux-headers-4.4.189_4.4.189-2_arm64.deb
sudo apt install --reinstall libsdl2-dev libsdl2-ttf-dev linux-libc-dev libc6-dev

# gcc
gcc -o simple-launcher.elf simple-launcher.c -lSDL2 -lSDL2_ttf
```

### Install
Copy all files to /home/ark

Run: 
```bash
chmod +x simple-launcher.elf
chmod +x simple-launcher.sh
chmod +x switch-to-simple-launcher.sh
```

Copy switch-to-simple-launcher.sh to /roms/tools/

### (Optional) update emulationstation.service to launch Simple Launcher as frontend (instead of ES) at boot
```bash
sudo sed -i 's|"$esdir/emulationstation" "$@"|/home/ark/simple-launcher.elf|g' /usr/bin/emulationstation/emulationstation.sh
sudo systemctl restart emulationstation
```

## Compile for RG35XXP
```bash
cd /mnt/mmc/Roms/APPS
gcc -o simple-launcher.elf simple-launcher.c -lSDL2 -lSDL2_ttf -DRG35XXP
```

### Install
Copy all files to /mnt/mmc/Roms/APPS
