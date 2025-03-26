#/bin/zsh
docker kill nm-VM
docker container rm nm-VM
docker image rm -f debian:nm