# Simple Launcher

Quick build and copy to device
```bash
make simple-launcher-dirclean && make simple-launcher-rebuild && rsync -av output/build/simple-launcher-1.0/simple-launcher root@192.168.1.140:/usr/local/bin/.
```