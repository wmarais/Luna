# Luna
Simple Linux Windowing System without using X11 or Wayland.

## Building for Debian 9


```
sudo apt-get install git build-essential binutils valgrind cmake uuid-dev libconfig++-dev libudev-dev libdrm-dev
```

Clone the source code. Note that line below perform a shallow copy. If you need
the full code history, remove the --depth=1 parameter from the clone.

```
git clone https://github.com/wmarais/Luna.git --depth=1
```

Create a build directory:

```
mkdir Luna-Build
cd Luna-Build
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


## Valgrind (Checking Memory Leaks)
The debug build is configured to support valgrind checks. To perform an
inspection, use the following command:

```
valgrind --leak-check=yes --leak-check=full --show-leak-kinds=all ./LunaEarth
```

## GProf (Code Profiling)

Version 6 of GCC has a bug that stops GProf from working. Though the debug
configuraing is compiled with GProf support, it will only work with earlier
version of GCC. Should only need to first execute the application once:

```
./LunaEarth
```

Then execute GProf:

```
gprof LunaEarth gmon.out > analysis.txt
```
