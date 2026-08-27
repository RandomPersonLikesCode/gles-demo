shopt -s globstar
set -eu

sdk_level="35"
sdk="$ANDROID_HOME/platforms/android-$sdk_level/android.jar"

cd ..

rm -r bin
mkdir bin

aapt2 compile -o res --dir ../app/res
aapt2 link -I $sdk --manifest ../app/AndroidManifest.xml -o bin/app-unsigned.apk res/*.flat
zip -0 -ur bin/app-unsigned.apk lib

cd obj

zip -0 -ur ../bin/app-unsigned.apk *.dex

cd ../../app

zip -0 -ur ../.build/bin/app-unsigned.apk assets

cd ../.build/bin

zipalign -v 4 app-unsigned.apk app-aligned.apk
