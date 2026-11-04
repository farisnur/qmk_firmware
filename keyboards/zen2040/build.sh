#!/bin/sh
# Builds zen2040_left.uf2 and zen2040_right.uf2 in the repository root.
cd "$(dirname "$0")/../.." || exit 1
for side in left right; do
  SIDE=$(echo "$side" | tr a-z A-Z)
  rm -rf ".build/obj_zen2040_$side"
  qmk compile -kb zen2040 -km default -e TARGET=zen2040_$side -e EXTRAFLAGS=-DINIT_EE_HANDS_$SIDE || exit 1
done
