FROM ubuntu

WORKDIR /opt/dev

USER root

RUN apt update
RUN apt install git curl qtbase5-dev qt6-base-dev build-essential libssl-dev -y 
RUN git config --global user.name "André Lehto"
RUN git config --global user.email "a.lehto@relux.com"

RUN mkdir /root/.ssh
RUN touch /root/.ssh/known_hosts

##Install latest CMake version
RUN curl -OL https://github.com/Kitware/CMake/releases/download/v3.25.1/cmake-3.25.1.tar.gz && \
    tar xf cmake-3.25.1.tar.gz && \
    cd cmake-3.25.1 && \
    ./configure && \
    make -j4 && \
    make install 

## Cuda version
ARG cuda_version=9.0
ENV DISPLAY=lehto.local:0.0
ENV NVIDIA_DRIVER_CAPABILITIES compute,utility,display