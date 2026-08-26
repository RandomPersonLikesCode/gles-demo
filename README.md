# OpenGL ES Demo
## About

This is a basic OpenGL ES 3.2 demo using C++ and SDL3
for Android.

## Building
### Prerequisites

- Unix-like system
- Bash
- Meson
- Ninja
- OpenJDK
- Android SDK
- Android NDK

### Cross file setup

Create 2 Meson cross file for 32 bit and 64 bit ARM Android device:

```ini
[host_machine]
system = 'android'
cpu_family = '<aarch64|arm>'
cpu = '<armv8-a|armv7-a>'
endian = 'little'

[constants]
api_level = '35'
ndk = '<ndk root path>/'
ndk_bin = ndk + 'toolchains/llvm/prebuilt/linux-x86_64/bin/'

[binaries]
ar = ndk_bin + 'llvm-ar'
as = ndk_bin + 'llvm-as'
c = ndk_bin + ' <aarch64|arm>-linux-<android|androideabi>' + api_level + '-clang'
cpp = ndk_bin + '<aarch64|arm>-linux-<android|androideabi>' + api_level + '-clang++'
ld = ndk_bin + 'ld'
ranlib = ndk_bin + 'llvm-ranlib'
strip = ndk_bin + 'llvm-strip'
pkg-config = 'false'
```

### Building the APK

```shell
meson setup --cross-file <path to cross file> -Dbuildtype=debug .build/obj
meson compile -C .build/obj
```

### Signing the APK

See the [apksigner documentation](https://developer.android.com/tools/apksigner) for
more information.

## License

This project is licensed under the MIT License, this project
also include some third party libraries, more information can
be found inside `THIRD_PARTY_LICENSE`.
