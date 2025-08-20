# gba-2048

To build the environent and docker

```sh
git submodule update --init
docker build -t gba-dev:latest .
```

To build run `mkgame.sh` or

```sh
docker run --rm -it -v $(pwd):/code gba-dev:latest
```
