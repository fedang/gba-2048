#!/bin/sh

set -ex

for f in graphics/block_*.bmp; do
	convert $f -colors 16 -type palette -define bmp:format=bmp3 $f
done
