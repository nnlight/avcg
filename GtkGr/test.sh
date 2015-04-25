#!/bin/bash

for f in ../expl/* ; do
  echo "./avcg $f"
  ./avcg $f;
done

for f in ../expl_new/* ; do
  if [ `basename $f` = "titan_hard.vcg" ] ; then
    continue;
  fi
  echo "./avcg $f"
  ./avcg $f;
done

