#/bin/zsh
docker build -t debian:nm .
docker run -d -it -v `pwd`:/mount/ --name nm-VM debian:nm