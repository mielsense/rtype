# syntax=docker/dockerfile:1

FROM ubuntu:latest AS base

FROM base AS base-vcpkg
RUN apt update
RUN apt install -y git curl zip unzip tar cmake build-essential pkg-config autoconf libtool
RUN apt install -y libudev-dev libx11-dev libxrandr-dev libxcursor-dev freeglut3-dev
RUN mkdir -p -m 0600 ~/.ssh && ssh-keyscan github.com >> ~/.ssh/known_hosts
WORKDIR /opt
RUN git clone https://github.com/microsoft/vcpkg.git
RUN /opt/vcpkg/bootstrap-vcpkg.sh

from base-vcpkg AS base-vcpkg-libs

COPY vcpkg.json /opt/vcpkg/vcpkg.json
WORKDIR /opt/vcpkg
RUN /opt/vcpkg/vcpkg install

FROM base-vcpkg-libs AS compile

WORKDIR /app

COPY . .

WORKDIR B-CPP-500-LYN-5-1-rtype-sebastien.lucas

RUN cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake
RUN cmake --build build

FROM alpine:latest AS r-type_server

RUN apk add gcompat libstdc++

COPY --from=compile /app/B-CPP-500-LYN-5-1-rtype-sebastien.lucas/build/game_server/r-type_server /usr/local/bin/r-type_server

ENTRYPOINT [ "/usr/local/bin/r-type_server" ]

FROM alpine:latest AS r-type_master_server

RUN apk add gcompat libstdc++

COPY --from=compile /app/B-CPP-500-LYN-5-1-rtype-sebastien.lucas/build/master_server/r-type_master_server /usr/local/bin/r-type_master_server

ENTRYPOINT [ "/usr/local/bin/r-type_master_server" ]