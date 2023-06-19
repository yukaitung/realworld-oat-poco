# Build
FROM ubuntu:22.04 AS build
# The abi does not not match in alpine when compling the mysqlclient

RUN apt update -qy
RUN apt upgrade -qy
RUN apt install -qy --no-install-recommends g++ make pip perl libmysqlclient-dev
RUN pip install conan cmake
RUN ln -s /usr/local/bin/cmake /usr/bin/cmake

ADD . /realworld
WORKDIR /realworld
RUN conan profile detect --force
RUN conan install . --output-folder=build --build=missing
WORKDIR /realworld/build
RUN cmake .. -DCMAKE_CXX_FLAGS=-isystem\ /usr/include/mysql -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
RUN cmake --build . -j`nproc`

# Export the executable
FROM scratch AS export
COPY --from=build /realworld/build/bin .

# Export the image
FROM ubuntu:22.04 AS image
RUN mkdir realworld
WORKDIR /realworld
COPY --from=build /realworld/build/bin .

EXPOSE 8000 8000

ENTRYPOINT ["./Realworld"]