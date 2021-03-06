FROM ubuntu:16.04

# number of concurrent threads during build
# usage: docker build --build-arg PARALLELISM=8 -t name/name .
ARG PARALLELISM=1

ENV IROHA_HOME /opt/iroha
ENV IROHA_BUILD /opt/iroha/build

RUN apt-get update; \
    apt-get -y upgrade; \
    apt-get -y --no-install-recommends install apt-utils; \
    apt-get -y clean

RUN apt-get -y --no-install-recommends install build-essential python-software-properties \
        automake libtool \
        # dev dependencies
        libssl-dev zlib1g-dev libboost-all-dev libc6-dbg golang \
        # CircleCI dependencies
        git ssh tar gzip ca-certificates python3 python3-pip python3-setuptools \
        # code coverage
        lcov \
        # other
        wget curl cmake file unzip gdb iputils-ping vim ccache \
        gcovr vera++ cppcheck doxygen graphviz graphviz-dev; \
    apt-get -y clean

RUN pip3 install --upgrade pip; \
    pip3 install flask requests

# install protobuf
RUN git clone https://github.com/google/protobuf /tmp/protobuf; \
    (cd /tmp/protobuf ; git checkout 80a37e0782d2d702d52234b62dd4b9ec74fd2c95); \
    cmake -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_BUILD_SHARED_LIBS=ON -H/tmp/protobuf/cmake -B/tmp/protobuf/.build; \
    cmake --build /tmp/protobuf/.build --target install -- -j${PARALLELISM}; \
    ldconfig; \
    rm -rf /tmp/protobuf

# install gflags
RUN git clone https://github.com/gflags/gflags /tmp/gflags; \
    (cd /tmp/gflags ; git checkout f8a0efe03aa69b3336d8e228b37d4ccb17324b88); \
    cmake -H/tmp/gflags -B/tmp/gflags/build; \
    cmake --build /tmp/gflags/build --target install -- -j${PARALLELISM}; \
    rm -rf /tmp/gflags

# install grpc
RUN git clone https://github.com/c-ares/c-ares /tmp/c-ares; \
    (cd /tmp/c-ares ; git checkout 3be1924221e1326df520f8498d704a5c4c8d0cce); \
    cmake -H/tmp/c-ares -B/tmp/c-ares/build; \
    cmake --build /tmp/c-ares/build --target install -- -j${PARALLELISM}; \
    ldconfig; \
    rm -rf /tmp/c-ares; \
    git clone https://github.com/grpc/grpc /tmp/grpc; \
    (cd /tmp/grpc ; git checkout bfcbad3b86c7912968dc8e64f2121c920dad4dfb); \
    (cd /tmp/grpc ; git submodule update --init third_party/benchmark); \
    cmake -DgRPC_ZLIB_PROVIDER=package -DgRPC_CARES_PROVIDER=package -DgRPC_SSL_PROVIDER=package \
        -DgRPC_PROTOBUF_PROVIDER=package -DgRPC_GFLAGS_PROVIDER=package -DBUILD_SHARED_LIBS=ON -H/tmp/grpc -B/tmp/grpc/.build; \
    cmake --build /tmp/grpc/.build --target install -- -j${PARALLELISM}; \
    ldconfig; \
    rm -rf /tmp/grpc

# install gtest
RUN git clone https://github.com/google/googletest /tmp/googletest; \
    (cd /tmp/googletest ; git checkout ec44c6c1675c25b9827aacd08c02433cccde7780); \
    cmake -H/tmp/googletest -B/tmp/googletest/build; \
    cmake --build /tmp/googletest/build --target install -- -j${PARALLELISM}; \
    rm -rf /tmp/googletest

# install spdlog
RUN git clone https://github.com/gabime/spdlog /tmp/spdlog; \
    (cd /tmp/spdlog ; git checkout f85a08622e20b74bff34381cafcb8ef8167b29d0); \
    cmake -DSPDLOG_BUILD_TESTING=OFF -H/tmp/spdlog -B/tmp/spdlog/build; \
    cmake --build /tmp/spdlog/build --target install; \
    rm -rf /tmp/spdlog

# install rxcpp
RUN git clone https://github.com/Reactive-Extensions/RxCpp /tmp/RxCpp; \
    (cd /tmp/RxCpp ; git checkout 1b2e0589f19cb34d8cd58803677701dcf2161876); \
    cmake -H/tmp/RxCpp -B/tmp/RxCpp/build; \
    cmake --build /tmp/RxCpp/build --target install; \
    rm -rf /tmp/RxCpp

# install rapidjson
RUN git clone https://github.com/miloyip/rapidjson /tmp/rapidjson; \
    (cd /tmp/rapidjson ; git checkout f54b0e47a08782a6131cc3d60f94d038fa6e0a51); \
    cmake -DRAPIDJSON_BUILD_EXAMPLES=OFF -H/tmp/rapidjson -B/tmp/rapidjson/build; \
    cmake --build /tmp/rapidjson/build --target install; \
    rm -rf /tmp/rapidjson

# install optional
RUN git clone https://github.com/martinmoene/optional-lite /tmp/optional-lite; \
    (cd /tmp/optional-lite ; git checkout a0ddabb8b52e1eaaf0dd1515bb85698b747528e4); \
    cp -r /tmp/optional-lite/include/nonstd /usr/local/include; \
    rm -rf /tmp/optional-lite

# install libpq
RUN git clone https://git.postgresql.org/git/postgresql.git /tmp/postgresql; \
    (cd /tmp/postgresql ; git checkout 029386ccbddd0a33d481b94e511f5219b03e6636); \
    (cd /tmp/postgresql ; /tmp/postgresql/configure --without-readline --prefix=/usr/local); \
    make -j${PARALLELISM} -C /tmp/postgresql/src/bin/pg_config; \
    make -j${PARALLELISM} -C /tmp/postgresql/src/interfaces/libpq; \
    make -C /tmp/postgresql/src/bin/pg_config install; \
    make -C /tmp/postgresql/src/interfaces/libpq install; \
    make -C /tmp/postgresql/src/include install; \
    rm -rf /tmp/postgresql

# install pqxx
RUN git clone https://github.com/jtv/libpqxx /tmp/libpqxx; \
    (cd /tmp/libpqxx ; git checkout 5b17abce5ac2b1a2f8278718405b7ade8bb30ae9); \
    (cd /tmp/libpqxx ; /tmp/libpqxx/configure --disable-documentation --with-pic); \
    make -j${PARALLELISM} -C /tmp/libpqxx; \
    make -C /tmp/libpqxx install; \
    rm -rf /tmp/libpqxx

# install cpp_redis with tacopie
RUN git clone https://github.com/Cylix/cpp_redis /tmp/cpp_redis; \
    (cd /tmp/cpp_redis ; git checkout 727aa5f06c8ce498168cbab5a023cad5b9c00bc0); \
    git clone https://github.com/Cylix/tacopie /tmp/cpp_redis/tacopie; \
    (cd /tmp/cpp_redis/tacopie ; git checkout 290dc38681f346adae41d3cc8feabbe534424675); \
    cmake -H/tmp/cpp_redis -B/tmp/cpp_redis/build; \
    cmake --build /tmp/cpp_redis/build --target install -- -j${PARALLELISM}; \
    rm -rf /tmp/cpp_redis

# install libuv
RUN git clone https://github.com/libuv/libuv /tmp/libuv; \
    (cd /tmp/libuv ; git checkout 2bb4b68758f07cd8617838e68c44c125bc567ba6); \
    /tmp/libuv/autogen.sh; \
    (cd /tmp/libuv ; /tmp/libuv/configure); \
    make -j${PARALLELISM} -C /tmp/libuv; \
    make -C /tmp/libuv install; \
    rm -rf /tmp/libuv

# install uvw
RUN git clone https://github.com/skypjack/uvw /tmp/uvw; \
    (cd /tmp/uvw ; git checkout 00de1f1110ce4a9803a85a214af5326529f11312); \
    cp -r /tmp/uvw/src/* /usr/local/include; \
    rm -rf /tmp/uvw

# install tbb
RUN git clone https://github.com/01org/tbb /tmp/tbb; \
    (cd /tmp/tbb ; git checkout eb6336ad29450f2a64af5123ca1b9429ff6bc11d); \
    make -j${PARALLELISM} -C /tmp/tbb tbb_build_prefix=build; \
    cp /tmp/tbb/build/build_debug/*.so* /usr/local/lib; \
    cp /tmp/tbb/build/build_release/*.so* /usr/local/lib; \
    cp -r /tmp/tbb/include/* /usr/local/include; \
    ldconfig; \
    rm -rf /tmp/tbb

# install docker
ENV DOCKER_VERSION=17.09.0-ce
RUN curl -L -o /tmp/docker-${DOCKER_VERSION}.tgz https://download.docker.com/linux/static/stable/x86_64/docker-${DOCKER_VERSION}.tgz; \
    tar -xz -C /tmp -f /tmp/docker-${DOCKER_VERSION}.tgz; \
    mv /tmp/docker/* /usr/bin; \
    rm /tmp/docker-${DOCKER_VERSION}.tgz; \
    rm -rf /tmp/docker

# install sonar cli
ENV SONAR_CLI_VERSION=3.0.3.778
RUN mkdir -p /opt/sonar; \
    curl -L -o /tmp/sonar.zip https://sonarsource.bintray.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-${SONAR_CLI_VERSION}-linux.zip; \
    unzip -o -d /tmp/sonar-scanner /tmp/sonar.zip; \
    mv /tmp/sonar-scanner/sonar-scanner-${SONAR_CLI_VERSION}-linux /opt/sonar/scanner; \
    ln -s -f /opt/sonar/scanner/bin/sonar-scanner /usr/local/bin/sonar-scanner; \
    rm -rf /tmp/sonar*

# install sonar build wrapper
RUN curl -L -o /tmp/sonar-build-wrapper.zip https://sonarcloud.io/static/cpp/build-wrapper-linux-x86.zip; \
    unzip -o -d /tmp/sonar-build-wrapper /tmp/sonar-build-wrapper.zip; \
    mv /tmp/sonar-build-wrapper/build-wrapper-linux-x86 /opt/sonar/build-wrapper; \
    ln -s -f /opt/sonar/build-wrapper/build-wrapper-linux-x86-64 /usr/local/bin/sonar-build-wrapper; \
    rm -rf /tmp/sonar*

# install latest valgrind
RUN git clone git://sourceware.org/git/valgrind.git /tmp/valgrind; \
    (cd /tmp/valgrind ; /tmp/valgrind/autogen.sh); \
    (cd /tmp/valgrind ; /tmp/valgrind/configure); \
    make -j${PARALLELISM} -C /tmp/valgrind; \
    make -C /tmp/valgrind install; \
    rm -rf /tmp/valgrind

WORKDIR /opt/iroha
CMD ["/bin/bash"]
