#!/bin/sh

docker run --rm -it \
	-v $(pwd):/game \
	--user "$(id -u):$(id -g)" \
	gba-dev:latest "$@"
