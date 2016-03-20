#!/bin/bash

DIRS="../expl ../expl_new"

if [ -n "$1" ] ; then
  DIRS="$1 $2 $3 $4 $5 $6"
fi

for dir in $DIRS ; do
  for f in $dir/* ; do
    if [ `basename $f` = "titan_hard.vcg" ] ; then
      continue;
    fi
    echo "./avcg $f"
    ./avcg $f;
  done
done


