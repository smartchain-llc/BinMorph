# syntax=docker/dockerfile:1.7
ARG BASE_IMAGE=debian:bookworm-slim

FROM ${BASE_IMAGE} AS toolchain
ARG USER_ID=10001
ARG GROUP_ID=10001
RUN --mount=type=cache,target=/var/cache/apt,sharing=locked \
    --mount=type=cache,target=/var/lib/apt/lists,sharing=locked \
    export DEBIAN_FRONTEND=noninteractive \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        bash \
        ca-certificates \
        clang \
        cmake \
        curl \
        g++ \
        git \
        netcat-openbsd \
        ninja-build \
        python3 \
    && rm -rf /var/lib/apt/lists/*
RUN groupadd --gid "${GROUP_ID}" builder \
    && useradd --uid "${USER_ID}" --gid builder --create-home --shell /bin/bash builder
WORKDIR /workspace

FROM toolchain AS source
COPY --chown=builder:builder CMakeLists.txt CMakePresets.json README.md /workspace/src/
COPY --chown=builder:builder include /workspace/src/include
COPY --chown=builder:builder src /workspace/src/src
COPY --chown=builder:builder tests /workspace/src/tests
COPY --chown=builder:builder scripts /workspace/src/scripts
COPY --chown=builder:builder docker /workspace/src/docker
RUN mkdir -p \
        /workspace/artifacts/coverage \
        /workspace/artifacts/fuzz \
        /workspace/artifacts/generated \
        /workspace/artifacts/junit \
        /workspace/artifacts/logs \
        /workspace/artifacts/metadata \
        /workspace/artifacts/packages \
    && chown -R builder:builder /workspace
USER builder
WORKDIR /workspace/src
ENV BINMORPH_ARTIFACT_DIR=/workspace/artifacts

FROM source AS configure
ARG USER_ID=10001
ARG GROUP_ID=10001
ARG CONFIGURE_PRESET=docker-gcc-debug
RUN --mount=type=cache,target=/home/builder/.cache,uid=${USER_ID},gid=${GROUP_ID} \
    ./scripts/container/configure.sh "${CONFIGURE_PRESET}"

FROM configure AS build
ARG USER_ID=10001
ARG GROUP_ID=10001
ARG BUILD_PRESET=docker-gcc-debug
RUN --mount=type=cache,target=/home/builder/.cache,uid=${USER_ID},gid=${GROUP_ID} \
    ./scripts/container/build.sh "${BUILD_PRESET}"

FROM build AS test
ARG TEST_PRESET=docker-gcc-debug
RUN ./scripts/container/test.sh "${TEST_PRESET}"
CMD ["./scripts/container/test.sh", "docker-gcc-debug"]

FROM debian:bookworm-slim AS artifacts
ARG BUILD_PRESET=docker-gcc-debug
COPY --from=test /workspace/artifacts /artifacts
COPY --from=test /workspace/src/build/${BUILD_PRESET}/binmorph /opt/binmorph/binmorph
WORKDIR /artifacts
ENTRYPOINT ["/opt/binmorph/binmorph"]
