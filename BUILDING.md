 ### Windows
 1. Open Visual Studio 2019 and clone this repository
 2. The default configuration should be "x86-Release" for the 32bit version\
 If you want the 64bit version select the "x64-Release" configuration
 3. Select `Build` -> `Build All`
 4. The .dll should now be in `gmod-discord-rpc/cpp/build/\*-Release/`
 
 ### Linux
 1. Make sure you have git, cmake, and gcc installed
 2. Open a terminal and run
 ```git clone https://github.com/fluffy-servers/gmod-discord-rpc --recursive
 cd gmod-discord-rpc/cpp
 mkdir build && cd build
 cmake .. && cmake --build .
 ```
 3. The .dll should now be in `gmod-discord-rpc/cpp/build/`
