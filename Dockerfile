FROM devkitpro/devkitarm:20250728

ENV DEVKITPRO=/opt/devkitpro \
    DEVKITARM=/opt/devkitpro/devkitARM \
    PATH=/opt/devkitpro/devkitARM/bin:$PATH

RUN mkdir -p /game
WORKDIR /game

CMD ["/bin/bash"]
