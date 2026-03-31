FROM ubuntu:24.04 AS builder
RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake ninja-build g++ git ca-certificates libssl-dev \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /src
COPY CMakeLists.txt ./
COPY cmake/ cmake/
COPY include/ include/
COPY src/ src/
COPY test/ test/
RUN cmake -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DProjectNestor_BUILD_TESTING=OFF \
    -DProjectNestor_ENABLE_CLANG_TIDY=OFF \
    -DProjectNestor_ENABLE_CPPCHECK=OFF \
    && cmake --build build --target ProjectNestor_server

FROM ubuntu:24.04
RUN apt-get update && apt-get install -y --no-install-recommends \
    libssl3 wget \
    && rm -rf /var/lib/apt/lists/*
COPY --from=builder /src/build/ProjectNestor_server /usr/local/bin/
EXPOSE 8081
ENV NESTOR_PORT=8081
ENV NATS_URL=nats://localhost:4222

RUN useradd -r -s /usr/sbin/nologin nestor
USER nestor

CMD ["ProjectNestor_server"]
