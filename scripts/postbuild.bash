shopt -s globstar
set -eu

sdk_level="35"
sdk="$ANDROID_HOME/platforms/android-$sdk_level/android.jar"

rm -rf build/bin
mkdir -p build/{bin,res}

aapt2 compile -o build/res --dir app/res
aapt2 link -I $sdk --manifest app/AndroidManifest.xml -o build/bin/app-unsigned.apk build/res/*.flat

cd build

zip -0 -ur bin/app-unsigned.apk lib

cd obj

zip -0 -ur ../bin/app-unsigned.apk *.dex

cd ../../app

zip -0 -ur ../build/bin/app-unsigned.apk assets

cd ..

zipalign -v 4 build/bin/app-unsigned.apk build/bin/app-aligned.apk
