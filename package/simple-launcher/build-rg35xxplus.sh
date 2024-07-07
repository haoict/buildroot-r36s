#!/bin/bash
set -v

PLATFORM=rg35xxplus
HOST_WORKSPACE=$(pwd)
GIT_SHA_FETCH=$(git rev-parse HEAD | cut -c 1-7)

if [ ! "$(docker images -q rg35xxplus-toolchain:latest 2> /dev/null)" ]; then
  echo 'toolchain image does not exist, building a new image'
  mkdir -p toolchains
  git clone https://github.com/haoict/union-$PLATFORM-toolchain/ toolchains/$PLATFORM-toolchain
  cd toolchains/$PLATFORM-toolchain && make .build
  cd $HOST_WORKSPACE
fi

docker run -it --rm -v $HOST_WORKSPACE:/root/workspace $PLATFORM-toolchain /bin/bash -c ". ~/.bashrc && cd /root/workspace && GIT_SHA_FETCH=$GIT_SHA_FETCH make && chmod 777 simple-launcher && mv simple-launcher simple-launcher-$GIT_SHA_FETCH && tar -czvf simple-launcher-$GIT_SHA_FETCH.tar.gz simple-launcher-$GIT_SHA_FETCH && mv simple-launcher-$GIT_SHA_FETCH.tar.gz /root/workspace/ && rm simple-launcher simple-launcher.elf"