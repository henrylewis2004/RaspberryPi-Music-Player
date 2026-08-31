# Install

## Download uf2 file 
NOTE: to be added when project is complete

## Install from source
To install, clone the repo with
> <code>git clone https://github.com/henrylewis2004/RaspberryPi-Music-Player.git</code>

Then add the submodules with
> <code>git submodules --init --recursive</code>

Download the Raspberry Pi C/C++ SDK
> <code>git clone https://github.com/raspberrypi/pico-sdk.git</code>
> NOTE: make sure to set the sdk path in CMakeLists.txt with - <code>set(PICO_SDK_PATH "/path/to/sdk/" CACHE PATH "Path to the Pico SDK")</code>

Compile the executable by 
> <code>mkdir build</code>
> <code>cd build</code>
> <code>cmake ..</code>
> <code>make Pico-Music</code>
