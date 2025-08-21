# gba-2048

To build the environent and docker

```sh
git submodule update --init
docker build -t gba-dev:latest .
```

You can use the `run.sh` script to run things directly inside the docker.
It just does this:

```sh
docker run --rm -it -v $(pwd):/code gba-dev:latest
```

## Colors

- 2 `#EEE4DA`
- 4 `#EBD8B6`
- 8 `#F2AE73`
- 16 `#F2AE73`
- 32 `#F57556`
- 64 `#F55A36`
- 128 `#F2CF56`
- 256 `#F4CC49`
- 512 `#F6C83C`
- 1024 `#`
- 2048 `#`
