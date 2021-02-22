FROM ubuntu:20.04
ENV DEBIAN_FRONTEND=noninteractive
WORKDIR /labs
RUN apt-get update
RUN apt-get install -y \
        git \
        make \
        wget \
        unzip \
        gcc-arm-none-eabi \
        build-essential \
        vim \
        stlink-tools
RUN wget -N http://web.eece.maine.edu/~hummels/classes/ece486/src/stmdevel.zip
RUN unzip -d stmdevel stmdevel.zip 
WORKDIR /labs/stmdevel/build
RUN make && make install
RUN rm /labs/* -r
WORKDIR /labs


### To build the docker image:
# docker build . -t <name>
#
### To run image (in bash):
# docker run -dit -v /dev:/dev -v $(pwd):/labs --privileged <name>
#
# This runs the image in privileged mode and mounts /dev to the container, giving it access to the devices
# The current working directory is also mounted to the container under /labs
#
### To flash the board:
# Find the name of the container using `docker ps`
# docker exec -it <container name> bash
# 
# In the container, the example code can be downloaded and flashed using `make flash`
# The mounted directory is also under /labs
