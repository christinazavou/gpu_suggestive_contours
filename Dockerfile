FROM gcc:4.9
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
RUN apt-get update \
    && apt-get install --yes \
        cmake \
        nano \
        libglu1-mesa-dev \
        freeglut3-dev \
        mesa-common-dev \
        libglew-dev \
        libsdl2-dev \
        libsdl2-image-dev \
        libglm-dev \
        libfreetype6-dev \
        mesa-utils
RUN mkdir buildD \
    && cd buildD \
    && cmake .. \
    && make
#RUN cat /etc/issue #to see linux version
#RUN glxinfo | grep OpenGL # to see opengl version
