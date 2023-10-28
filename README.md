# Poker clone in C
ace-to-five high, draw poker

### Desktop

Use the following to build for desktop:

``` bash
cmake -B build
cmake --build build
```
or
```batch
build.bat
```

### Web

Compiling for the web requires the [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html):

``` bash
mkdir build
cd build
emcmake cmake .. -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS="-s USE_GLFW=3" -DCMAKE_EXECUTABLE_SUFFIX=".html"
emmake make
```
or
```batch
build-web.bat
```
