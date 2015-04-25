#!/bin/bash

FT=

for f in ../../expl/* ; do
  ./a.out $f;
  if [ $? != 0 ] ; then
    FT="$FT $f";
  fi
done

for f in ../../expl_new/* ; do
  if [ `basename $f` = "titan_hard.vcg" ] ; then
    continue;
  fi
  ./a.out $f
  if [ $? != 0 ] ; then
    FT="$FT $f"
  fi
done

if [ -n "$FT" ] ; then echo "FAILED: $FT" ; fi


