#!/bin/sh

docker run --rm -it -v $(pwd):/game gba-dev:latest "$@"
