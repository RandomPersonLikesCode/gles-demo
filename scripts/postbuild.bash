shopt -s globstar
set -eu

sdk_level="35"
sdk="$ANDROID_HOME/platforms/android-$sdk_level/android.jar"

cd ..

rm -r bin
mkdir bin

aapt2 compile -o res --dir ../app/res
aapt2 link -I $sdk --manifest ../app/AndroidManifest.xml -o bin/apk-unsigned.apk res/*.flat
zip -0 -ur bin/apk-unsigned.apk lib

cd obj

zip -0 -ur ../bin/apk-unsigned.apk *.dex

cd ../../app

zip -0 -ur ../.build/bin/apk-unsigned.apk assets

cd ../.build/bin

zipalign -v 4 apk-unsigned.apk apk-aligned.apk
