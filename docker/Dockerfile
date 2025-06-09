FROM nvidia/cuda:11.6.1-devel-ubuntu20.04

# # Linux updating key (https://forums.developer.nvidia.com/t/notice-cuda-linux-repository-key-rotation/212771)
# RUN apt-key del 7fa2af80
# COPY cuda/cuda-keyring_1.0-1_all.deb cuda-keyring_1.0-1_all.deb
# RUN dpkg -i cuda-keyring_1.0-1_all.deb
# RUN sed -i '/developer\.download\.nvidia\.com\/compute\/cuda\/repos/d' /etc/apt/sources.list.d/*
# RUN sed -i '/developer\.download\.nvidia\.com\/compute\/machine-learning\/repos/d' /etc/apt/sources.list.d/*
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update -y && apt-get upgrade -y

# INSTALL REQUIREMENTS
RUN apt-get install -y protobuf-compiler
RUN apt-get install libreadline-dev -y
# RUN apt-get install -y clang-format
RUN apt-get install -y build-essential libssl-dev
RUN apt-get install -y wget
RUN apt-get update && apt-get upgrade -y && apt-get install -y git
RUN apt-get install gcc-10 g++-10 -y
RUN rm /usr/bin/gcc /usr/bin/g++ /usr/bin/cc /usr/bin/c++
RUN ln -sv /usr/bin/gcc-10 /usr/bin/gcc
RUN ln -sv /usr/bin/g++-10 /usr/bin/g++
RUN ln -sv /usr/bin/gcc-10 /usr/bin/cc
RUN ln -sv /usr/bin/g++-10 /usr/bin/c++
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y gdb

# INSTALL CMAKE for gunrock
# renovate: datasource=github-releases depName=Kitware/CMake
ARG CMAKE_VERSION=3.22.1
RUN wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-Linux-x86_64.sh -q -O /tmp/cmake-install.sh && chmod u+x /tmp/cmake-install.sh && mkdir /usr/bin/cmake && /tmp/cmake-install.sh --skip-license --prefix=/usr/bin/cmake && rm /tmp/cmake-install.sh
ENV PATH="/usr/bin/cmake/bin:${PATH}"

RUN apt-get update -y
RUN apt-get install software-properties-common -y
RUN add-apt-repository ppa:deadsnakes/ppa
# Install py39 from deadsnakes repository
RUN apt-get install python3.9 -y
# Install pip from standard ubuntu packages
RUN apt-get install python3-pip -y

# Install ssh
RUN apt update && apt install -y openssh-server
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config

RUN useradd -m -s /bin/bash frosa
RUN echo "frosa:frosa" | chpasswd

EXPOSE 22

ENTRYPOINT service ssh start && bash

RUN wget https://sourceware.org/pub/valgrind/valgrind-3.24.0.tar.bz2
RUN tar xvf valgrind-3.24.0.tar.bz2 && cd valgrind-3.24.0 && ./configure && make && make install
