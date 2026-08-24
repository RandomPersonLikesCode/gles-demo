shopt -s globstar
set -eu

min_api="26"
sdk_level="35"
sdk="$ANDROID_HOME/platforms/android-$sdk_level/android.jar"
sdl="app/src/java/third_party/SDL3/SDL3.jar"

find app/src -name "*.java" > .build/java_src.txt

javac --release 17 -cp "$sdk:$sdl" -d .build/obj @.build/java_src.txt
d8 --min-api $min_api --lib $sdk --output .build/obj/ .build/obj/**/*.class $sdl
