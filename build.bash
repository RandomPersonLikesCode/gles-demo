# SPDX-License-Identifier: MIT

shopt -s globstar
set -euo pipefail

#

print_usage() {
  echo "Usage: $0 [flags]"
  echo "Build options:"
  echo "  -d, --debug                Enable debug build (default)"
  echo "  -r, --release              Enable release build"
  echo "      --full-build           Build/rebuild everything (default)"
  echo "  -c, --only-cc              Build only the C++ code"
  echo "  -j, --only-java            Build only the Java code"
  echo "      --all-arch             Build for all architecture (default)"
  echo "      --32-bit               Build only for armeabi-v7a architecture"
  echo "      --64-bit               Build only for arm64-v8a architecture"
  echo "      --proc=[proc]          Enable parallelism for [proc] process (default is 4)"
  echo "      --conf=[file]          Use [file] as compile config file"
  echo
  echo "Other options:"
  echo "      --clean                Clean build artifacts"
  echo "  -h, --help                 Show help message"
}

#

build_dir="./.build"
cache_dir="./.cache"
res_dir="./app/res"
src_dir="./app/src"

java_src="$cache_dir/java_src.txt"
java_sdl="$src_dir/java/third_party/SDL3/libSDL3.jar"
java_classes="$cache_dir/**/*.class"

cc_dir="$src_dir/jni"
cc_third_party="$cc_dir/third_party"

incl="$cc_third_party/include"
sdl_32="$cc_third_party/lib/SDL3/armeabi-v7a"
sdl_64="$cc_third_party/lib/SDL3/arm64-v8a"

lib_dir_32="$cache_dir/lib/armeabi-v7a"
lib_dir_64="$cache_dir/lib/arm64-v8a"
lib_32="$lib_dir_32/libmain.so"
lib_64="$lib_dir_64/libmain.so"

dir_32="$cache_dir/32"
dir_64="$cache_dir/64"

apk_unsigned="$build_dir/app-unsigned.apk"
apk_aligned="$build_dir/app-aligned.apk"
apk_signed="$build_dir/app-signed.apk"

manifest="./app/AndroidManifest.xml"
flat="$cache_dir/*.flat"

build_type="debug"
build_which="all"
build_arch="all"

proc="4"

clean="false"

#

if [[ $# -eq 0 ]]; then
  print_usage
  exit 1
fi

while [[ $# -gt 0 ]]; do
  case "$1" in
    -h|--help)
      print_usage
      exit 0

      ;;
    -d|--debug)
      build_type="debug"
      
      shift
      ;;
    -r|--release)
      build_type="release"

      shift
      ;;
    --full-build)
      build_which="all"
      
      shift
      ;;
    -j|--only-java)
      build_which="java"

      shift
      ;;
    -c|--only-cc)
      build_which="cc"

      shift
      ;;
    --all-arch)
      build_arch="all"

      shift
      ;;
    --32-bit)
      build_arch="32-bit"

      shift
      ;;
    --64-bit)
      build_arch="64-bit"

      shift
      ;;
    --proc=*)
      proc="${1#*=}"

      shift
      ;;
      --conf=*)
      conf_file="${1#*=}"

      shift
      ;;
    --clean)
      clean="true"

      shift
      ;;
    *)
      echo "Unknown option: $1"
      exit 1

      ;;
  esac
done

#

if [[ "$clean" == "true" ]]; then
  echo "Cleaning build artifacts"
  rm -rf $build_dir $cache_dir

  echo "Done"
  exit 0
fi

#

source $conf_file

android_home="$HOME/.local/share/android"
sdk="$android_home/platforms/android-$target_sdk_ver/android.jar"
ndk="$android_home/ndk/$ndk_ver/toolchains/llvm/prebuilt/linux-x86_64"
ndk_bin="$ndk/bin"
ndk_lib="$ndk/sysroot/usr/lib"

#

aapt2_compile_flag="compile -o $cache_dir --dir $res_dir"
aapt2_link_flag="link -I $sdk --manifest $manifest -o $apk_unsigned $flat"

javac_flag="--release 17 -cp "$sdk:$java_sdl" -d $cache_dir @$java_src"

d8_flag="--min-api $min_sdk_ver --lib $sdk --output $cache_dir $java_classes $java_sdl"

cc_32="$ndk_bin/armv7a-linux-androideabi$target_sdk_ver-clang++"
cc_64="$ndk_bin/aarch64-linux-android$target_sdk_ver-clang++"

cc_flag_c_d="-std=c++20 -Wall -Wextra -Wpedantic -Werror -O0 -g3 -fno-omit-frame-pointer -fno-rtti -fno-exceptions -DSDL_ASSERT_LEVEL=3 -isystem ../.$incl -fcolor-diagnostics -c -fPIC"
cc_flag_c_r="-std=c++20 -Wall -Wextra -Wpedantic -Werror -O2 -flto=thin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -DNDEBUG -DSDL_ASSERT_LEVEL=0 -isystem ../.$incl -fcolor-diagnostics -c -fPIC"
cc_flag_l_d="-shared"
cc_flag_l_r="-O2 -flto=thin -Wl,--gc-sections -shared"

ld_flag_32="-L$lib_dir_32 -lc++_shared -lGLESv3 -lSDL3"
ld_flag_64="-L$lib_dir_64 -lc++_shared -lGLESv3 -lSDL3"

cc_strip="$ndk_bin/llvm-strip"

#

print_config() {
  echo "Build type: $build_type"
  echo "Build architecture: $build_arch"
  echo
} 

setup_libs() {
  if [[ "$build_which" == "all" ]]; then
    echo "Extracting libraries"
    tar -xvf $cc_third_party/lib/SDL3/libSDL3.tar.xz -C $cc_third_party/lib/SDL3/

    echo "Copying libraries"
    case "$build_arch" in
      "32-bit")
        mkdir -p $lib_dir_32
        cp $sdl_32/libSDL3.so $lib_dir_32
        cp $ndk_lib/arm-linux-androideabi/libc++_shared.so $lib_dir_32

        ;;
      "64-bit")
        mkdir -p $lib_dir_64
        cp $sdl_64/libSDL3.so $lib_dir_64
        cp $ndk_lib/aarch64-linux-android/libc++_shared.so $lib_dir_64

        ;;
      *)
        mkdir -p $lib_dir_32 $lib_dir_64
        cp $sdl_32/libSDL3.so $lib_dir_32
        cp $sdl_64/libSDL3.so $lib_dir_64
        cp $ndk_lib/arm-linux-androideabi/libc++_shared.so $lib_dir_32
        cp $ndk_lib/aarch64-linux-android/libc++_shared.so $lib_dir_64

        ;;
    esac
  fi
}

compile_link_res() {
  echo "Compiling resources"
  aapt2 $aapt2_compile_flag

  echo "Linking resources"
  aapt2 $aapt2_link_flag
}

compile_java() {
  if [[ "$build_which" == "java" || "$build_which" == "all" ]]; then
    echo "Compiling Java sources"
    find $src_dir -name "*.java" > $java_src
    javac $javac_flag

    echo "Dexing Java classes"
    d8 $d8_flag
  fi
}

_compile_cc_32_d() {
  cd $dir_32
  printf "../.$cc_dir/%s\n" "${cc_src[@]}" | parallel -j $proc $cc_32 $cc_flag_c_d {}
  cd $OLDPWD
}

_compile_cc_32_r() {
  cd $dir_32
  printf "../.$cc_dir/%s\n" "${cc_src[@]}" | parallel -j $proc $cc_32 $cc_flag_c_r {}
  cd $OLDPWD
}

_compile_cc_64_d() {
  cd $dir_64
  printf "../.$cc_dir/%s\n" "${cc_src[@]}" | parallel -j $proc $cc_64 $cc_flag_c_d {}
  cd $OLDPWD
}

_compile_cc_64_r() {
  cd $dir_64
  printf "../.$cc_dir/%s\n" "${cc_src[@]}" | parallel -j $proc $cc_64 $cc_flag_c_r {}
  cd $OLDPWD
}

_link_cc_32_d() {
  $cc_32 $cc_flag_l_d -o $lib_32 $dir_32/*.o $ld_flag_32
}

_link_cc_32_r() {
  $cc_32 $cc_flag_l_r -o $lib_32 $dir_32/*.o $ld_flag_32
}

_link_cc_64_d() {
  $cc_64 $cc_flag_l_d -o $lib_64 $dir_64/*.o $ld_flag_64
}

_link_cc_64_r() {
  $cc_64 $cc_flag_l_r -o $lib_64 $dir_64/*.o $ld_flag_64
}

cc_build_variant="${build_type}_${build_arch}"

compile_cc() {
  if [[ "$build_which" == "cc" || "$build_which" == "all" ]]; then
    case "$cc_build_variant" in
      "release_32-bit")
        mkdir -p $lib_dir_32 $dir_32
        echo "Compiling C++ sources"
        _compile_cc_32_r

        echo "Linking C++ objects"
        _link_cc_32_r

        ;;
      "release_64-bit")
        mkdir -p $lib_dir_64 $dir_64
        echo "Compiling C++ sources"
        _compile_cc_64_r

        echo "Linking C++ objects"
        _link_cc_64_r

        ;;
      "release_all")
        mkdir -p $lib_dir_32 $dir_32 $lib_dir_64 $dir_64
        echo "Compiling C++ sources"
        _compile_cc_32_r
        _compile_cc_64_r

        echo "Linking C++ objects"
        _link_cc_32_r
        _link_cc_64_r

        ;;
      "debug_32-bit")
        mkdir -p $lib_dir_32 $dir_32
        echo "Compiling C++ sources"
        _compile_cc_32_d

        echo "Linking C++ objects"
        _link_cc_32_d

        ;;
      "debug_64-bit")
        mkdir -p $lib_dir_64 $dir_64
        echo "Compiling C++ sources"
        _compile_cc_64_d

        echo "Linking C++ objects"
        _link_cc_64_d

        ;;
      *)
        mkdir -p $lib_dir_32 $dir_32 $lib_dir_64 $dir_64
        echo "Compiling C++ sources"
        _compile_cc_32_d
        _compile_cc_64_d

        echo "Linking C++ objects"
        _link_cc_32_d
        _link_cc_64_d

        ;;
    esac
  fi
}

strip_libs() {
  if [[ "$build_type" == "release" ]]; then
    echo "Stripping shared libraries"
    $cc_strip --strip-unneeded $cache_dir/**/*.so
  fi
}

zip_apk() {
  echo "Zipping APK"
  cd $cache_dir

  zip -0 -ur .$apk_unsigned ./lib ./*.dex

  cd ../app

  zip -0 -ur .$apk_unsigned ./assets

  cd ..
}

align_apk() {
  echo "Aligning APK"
  zipalign -v 4 $apk_unsigned $apk_aligned
}

sign_apk() {
  echo "Signing APK"
  apksigner sign --ks $ks_file --ks-key-alias $ks_alias --ks-pass pass:$ks_pass --out $apk_signed $apk_aligned
}

#

main() {
  rm -rf $build_dir
  mkdir -p $build_dir $cache_dir

  print_config

  setup_libs

  compile_link_res
  compile_java
  compile_cc
  strip_libs
  zip_apk
  align_apk
  sign_apk

  echo "Done"
}

main
