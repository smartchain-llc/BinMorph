# syntax=docker/dockerfile:1.7
ARG BASE_IMAGE=debian:bookworm-slim

FROM ${BASE_IMAGE} AS toolchain
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update \
 && apt-get install -y --no-install-recommends \
      ca-certificates clang cmake g++ git lcov ninja-build python3 xz-utils \
 && rm -rf /var/lib/apt/lists/*

ARG USER_ID=10001
ARG GROUP_ID=10001
RUN groupadd --gid "${GROUP_ID}" builder \
 && useradd --uid "${USER_ID}" --gid builder --create-home --shell /bin/bash builder
WORKDIR /workspace

FROM toolchain AS source
COPY --chown=builder:builder . /workspace/src
RUN mkdir -p /workspace/build /workspace/artifacts \
 && chown -R builder:builder /workspace
USER builder
WORKDIR /workspace/src

FROM source AS configure
ARG CONFIGURE_PRESET=docker-gcc-debug
RUN --mount=type=cache,target=/home/builder/.cache,uid=10001,gid=10001 \
    cmake --preset "${CONFIGURE_PRESET}"

FROM configure AS build
ARG BUILD_PRESET=docker-gcc-debug
RUN --mount=type=cache,target=/home/builder/.cache,uid=10001,gid=10001 \
    cmake --build --preset "${BUILD_PRESET}" --parallel

FROM build AS test
ARG TEST_PRESET=docker-gcc-debug
RUN mkdir -p /workspace/artifacts/junit \
 && ctest --preset "${TEST_PRESET}" --output-on-failure \
      --output-junit /workspace/artifacts/junit/results.xml

FROM scratch AS artifacts
COPY --from=test /workspace/artifacts/ /
