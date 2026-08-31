# Install

## Download uf2 file 
NOTE: to be added when project is complete

## Install from source
### Download the project and dependencies
Clone the repo with

<code>git clone https://github.com/henrylewis2004/RaspberryPi-Music-Player.git</code>

Then inside the downloaded repo add the submodules with

<code>git submodules --init --recursive</code>

If missing the Raspberry Pi C/C++ SDK make sure to download with 

<code>git clone https://github.com/raspberrypi/pico-sdk.git</code>

> NOTE: make sure to set the sdk path in CMakeLists.txt with to the path of the downloaded sdk - <code>set(PICO_SDK_PATH "/path/to/sdk/" CACHE PATH "Path to the Pico SDK")</code>

### Compile and flash executable to Raspberry Pi 
Compile the executable by 

<code>mkdir build</code>

<code>cd build</code>

<code>cmake ..</code>

<code>make Pico-Music</code>

Finally, connect the Raspberry Pi to a PC (whilst holding the BOOTSEL button) and copy the Pico-Music.uf2 file to the Pi and wait for it to finish installing
