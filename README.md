# Luna
Simple Debian Windowing System without using X11 or Wayland. Not compatible with
either X11 or Wayland. It will probably work on other Linux flavours, though I
have not explicitly tested on other flavours. It is designed with a Client /
Server architecture with rendering performed client side. It also uses tripe
buffering and Vsync.

LunaCommon - The shared code used by both the client and server.
LunaServer - The server specific code.
LunaClient - The client specific code.

LunaEarth - A server implementation.
LunaMoon - A client implementation.

## Video Cards
The code uses KMS and DRM and as such the kernel and video drivers must support
this for the code to work. In Debian 9, KMS is default enabled and to get the
right video drivers is just a matter of installing the correct packages.

Unfortunately I only have so many video cards to test with and what I've tested
with are listed below. Of course, only install the video drivers that you need.
If you don't know what video card you have, use:

```
sudo lspci -v grep | VGA
```

The results should be formated as shown in the sections below.

### AMD / Radeon Video Cards

Video Card / Driver: Advanced Micro Devices, Inc. [AMD/ATI] RV710/M92 [Mobility 
Radeon HD 4530/4570/545v]

```
sudo apt-get install libdrm-radeon1 firmware-amd-graphics
```

### nVidia

TBD

### Intel

TBD

### Virtual Box

TBD


## Building for Debian 9

Install the required dependencies to compile the source code:

```
sudo apt-get install git build-essential binutils valgrind cmake uuid-dev \
  libconfig++-dev libudev-dev libdrm-dev
```

Clone the source code. Note that line below perform a shallow copy. If you need
the full code history, remove the --depth=1 parameter from the clone.

```
git clone https://github.com/wmarais/Luna.git --depth=1
```

Create a build directory:

```
mkdir Build
cd Build
```

Run CMake to generate the make file. Generally no parameters need to be
specified other than the build type, Debug or Release. Below we are configuring
a Release build.

```
cmake ../Luna -DCMAKE_BUILD_TYPE=Release
```

Compile it. Depending on how many cores you have, you may want to specify the
number of parallel threads. Below we use 8.

```
make -j8
```

This should compile fairly quickly, once compiled two binaries will exist,

LunaEarth - The Display Server.
LunaMoon - The Display Client.

Simply run LunaEarth at this point to start the Display / Session server. At the
moment it simply initialises UDEV and start hotplug monitoring.

I use the following quick commands when testing. Of course be super careful to
be in the Build directory because rm -r * is a very dangerous command. I test
on non production boxes that I regularly format so data loss is not a problem
for me.

To perform a clean after a git pull:
```
rm -r *; cmake ../Luna -DCMAKE_BUILD_TYPE=Debug; make -j8
```

To execute with logging (log function to be improved in future version):

```
./LunaEarth 2>&1 | tee log.txt
```

To quit, simply press Ctrl + C.

## Valgrind (Checking Memory Leaks)
The debug build is configured to support valgrind checks. To perform an
inspection, use the following command:

```
valgrind --leak-check=yes --leak-check=full --show-leak-kinds=all ./LunaEarth
```

## GProf (Code Profiling)

Version 6 of GCC has a bug that stops GProf from working. Though the debug
configuration is compiled with GProf support, it will only work with earlier
version of GCC. Should only need to first execute the application once:

```
./LunaEarth
```

Then execute GProf:

```
gprof LunaEarth gmon.out > analysis.txt
```

### Google Benchmark Support
To use google benchmark, it is necesary to install the libraries first:

```
sudo apt-get install libbenchmark-dev libbenchmark-tools
```

## Doxygen
In order to generate doxygen documentation, the doxygen tool must be installed,
if not installed the option to build the documentation will be automatically
disabled.

```
sudo apt-get install doxygen
```
