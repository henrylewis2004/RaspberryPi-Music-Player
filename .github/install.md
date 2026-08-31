# Install

## Download uf2 file 
NOTE: to be added when project is complete

## Install from source
To install, clone the repo with
> git clone https://github.com/henrylewis2004/RaspberryPi-Music-Player.git

Then add the submodules with
> git submodules --init --recursive

Download the Raspberry Pi C/C++ SDK
> git clone https://github.com/raspberrypi/pico-sdk.git
> NOTE: make sure to set the sdk path in CMakeLists.txt with - set(PICO_SDK_PATH "/path/to/sdk/" CACHE PATH "Path to the Pico SDK")

Compile the executable by 
> mkdir build
> cd build
> cmake ..
> make Pico-Music
