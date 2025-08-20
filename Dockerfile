FROM devkitpro/devkitarm:20250728

ENV DEVKITPRO=/opt/devkitpro \
    DEVKITARM=/opt/devkitpro/devkitARM \
    PATH=/opt/devkitpro/devkitARM/bin:$PATH

RUN apt-get install python3 python-is-python3

RUN mkdir -p /game

WORKDIR /game

CMD ["/bin/bash"]
