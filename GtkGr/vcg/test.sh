#!/bin/bash

FT=
DIRS="../../expl ../../expl_new"

if [ -n "$1" ] ; then
  DIRS="$1 $2 $3 $4 $5 $6"
fi

for dir in $DIRS ; do
  for f in $dir/* ; do
    if [ `basename $f` = "titan_hard.vcg" ] ; then
      continue;
    fi
    ./a.out $f;
    if [ $? != 0 ] ; then
      FT="$FT $f";
    fi
  done
done

if [ -n "$FT" ] ; then echo "FAILED: $FT" ; fi


