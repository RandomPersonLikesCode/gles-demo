# OpenGL ES Demo
## About

This is a basic OpenGL ES 3.2 demo using C++ and SDL3
for Android.

## Building
### Prerequisites

- Unix-like system
- Bash
- XMake
- OpenJDK
- Android SDK
- Android NDK

Make sure to have `libSDL3.a` for arm64-v8a and armeabi-v7a, and also
`SDL3.jar` inside `$ANDROID_HOME/lib/SDL3`. make sure to also put necessary
SDL3 headers inside `$ANDROID_HOME/include/SDL3`. i.e, the `$ANDROID_HOME/lib`
directory must looks something like:

```
$ANDROID_HOME/lib/
└── SDL3
    ├── SDL3.jar
    ├── arm64-v8a
    │   └── libSDL3.a
    └── armeabi-v7a
        └── libSDL3.a
```

and for the SDL3 headers:

```
$ANDROID_HOME/include/
└── SDL3
    ├── SDL.h
    ├── SDL_assert.h
    ├── SDL_asyncio.h
    etc
```

### Building the APK

```shell
xmake f -p android
bash ./scripts/prebuild.bash

xmake build

bash ./scripts/postbuild.bash
```

### Signing the APK

See the [apksigner documentation](https://developer.android.com/tools/apksigner) for
more information.

## License

This project is licensed under the MIT License, this project
also include some third party libraries, more information can
be found inside `THIRD_PARTY_LICENSE`.
