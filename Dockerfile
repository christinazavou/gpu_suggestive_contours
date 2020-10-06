FROM gcc:4.9
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
RUN apt-get update \
    && apt-get install --yes \
        cmake \
        nano
# for GL/gl.h
RUN apt-get install --yes \
        mesa-common-dev
# for GL/glu.h
RUN apt-get install --yes \
        libglu1-mesa-dev \
        freeglut3-dev
# for GL/glew.h
RUN apt-get install --yes \
        libglew-dev

RUN mkdir buildD \
    && cd buildD \
    && cmake .. \
    && make
#RUN cat /etc/issue #to see linux version
#RUN apt-get install --yes mesa-utils && glxinfo | grep OpenGL # to see opengl version
