FROM debian:bookworm
RUN apt-get update -y && apt-get upgrade -y && apt-get install gcc make gcc-multilib -y && apt install file -y
WORKDIR /mount/