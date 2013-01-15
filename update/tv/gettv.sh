#!/bin/bash

. /usr/local/minerva/conf/tvconf.conf

pushd ~minerva/update/tv >/dev/null

mkdir -p cache/2
mkdir -p cache/1
mkdir -p cache/0


NUM=1
for channel in ${stations[@]}; do
  rm -f cache/2/ch$NUM.xml cache/1/ch$NUM.xml cache/0/ch$NUM.xml

  wget http://bleb.org/tv/data/listings/2/$channel.xml?Minerva -O cache/2/ch$NUM.xml
  sleep 2

  wget http://bleb.org/tv/data/listings/1/$channel.xml?Minerva -O cache/1/ch$NUM.xml
  sleep 2

  wget http://bleb.org/tv/data/listings/0/$channel.xml?Minerva -O cache/0/ch$NUM.xml
  sleep 2

  NUM=$(($NUM+1))

done

popd

