FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        g++ \
        libboost-all-dev \
        python3-dev \
        python3-pip \
        python3-pybind11 \
        ca-certificates \
    && rm -rf /var/lib/apt/lists/*

RUN pip3 install --no-cache-dir pybind11-global numpy

WORKDIR /app
COPY . /app

RUN cmake -B build -S . -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build -j$(nproc)

RUN cd socket \
    && g++ -O3 server.cpp -o server \
    && chmod +x ./server

EXPOSE 8080
WORKDIR /app/socket
CMD ["./server"]
