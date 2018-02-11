## About
This directory contains the source files of the computing software, it is to be used on your computer.
It needs several third-party software as described in the compile section (installed using apt-get, if you use another package manager, you should find the same package on them).

## Compile
Node.js

    sudo apt-get install rabbitmq-server nodejs npm
    sudo ln -s /usr/bin/nodejs /usr/bin/node
    sudo npm install -g iced-coffee-script
    make -C /ble_interface

RabbitMQ-C

    sudo apt-get install cmake
    git clone https://github.com/alanxz/rabbitmq-c
    mkdir build
    cd build
    sudo cmake --build . --config Release --target install
    sudo ln -s /usr/local/lib/<path to librabbitmq.so.1> /usr/lib/

Computer

    sudo apt-get install libjsoncpp-dev libgsl0-dev libatlas-base-dev
    make -C /calculator

Blender : http://www.blender.org/download/

