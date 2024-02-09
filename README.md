# VisionEve
A platform for eye health research

## Coding convension
Follow Google C++ Style Guide
### checking coding style tool
Use cpplint with filter
example: cpplint --filter=-build/c++11

## Pre-requirements
sudo apt install libglfw3 libglfw3-dev libspdlog-dev libsqlite3-dev libopencv-dev

## Build
mkdir build
cd build
cmake ..
make
