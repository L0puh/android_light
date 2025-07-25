#!/bin/bash
set -e

# Paths setup
SDK="$PWD/android_sdk"
NDK="$SDK/ndk/26.2.11394342"
BUILD_TOOLS="$SDK/build-tools/34.0.0"
PLATFORM="$SDK/platforms/android-34"
MYAPP="$PWD/myapp"
RELEASE=11
SRC="$MYAPP/src"
ROOT="$PWD"

#TODO create: apk/lib/x86_64}
mkdir -p $MYAPP/build/{gen,obj,apk} 
cp $PWD/template/* $MYAPP -r

echo "COMPILING APPLICATION..."

echo "packaging..."
"$BUILD_TOOLS/aapt" package -f -m -J $MYAPP/build/gen/ -S $MYAPP/res \
      -M $MYAPP/AndroidManifest.xml -I "$PLATFORM/android.jar"

# FIXME:
# cd "$MYAPP/build"

# cmake -DCMAKE_TOOLCHAIN_FILE="$NDK/build/cmake/android.toolchain.cmake" \
#       -DANDROID_ABI=x86_64 \
#       -DANDROID_PLATFORM=android-34 \
#       "$SRC"

# cmake --build . --target native-lib

# cd $ROOT 
# mv $MYAPP/build/libnative-lib.so "$MYAPP/build/apk/lib/x86_64/"

echo "compiling java..."

cd $MYAPP
javac --release $RELEASE -classpath "$PLATFORM/android.jar" -d \
    build/obj build/gen/example/myapp/R.java java/example/myapp/MainActivity.java

echo "creating byte files..."
"$BUILD_TOOLS/d8" --release --lib "$PLATFORM/android.jar" \
      --output build/apk/ build/obj/example/myapp/*.class
cd $ROOT

"$BUILD_TOOLS/aapt" package -f -M $MYAPP/AndroidManifest.xml -S $MYAPP/res/ \
      -I "$PLATFORM/android.jar" \
      -F $MYAPP/build/myapp.unsigned.apk $MYAPP/build/apk/

echo "SIGNING APK..."

"$BUILD_TOOLS/zipalign" -f -p 4 "$MYAPP/build/myapp.unsigned.apk" "$MYAPP/build/myapp.aligned.apk"

"$BUILD_TOOLS/apksigner" sign --ks keystore.jks \
  --ks-key-alias androidkey --ks-pass pass:android \
  --key-pass pass:android --out "$MYAPP/build/myapp.apk" \
  "$MYAPP/build/myapp.aligned.apk"

echo "DONE. CHECK OUT $MYAPP/build/myapp.apk"

"$SDK/platform-tools/adb" install -r $MYAPP/build/myapp.apk
"$SDK/platform-tools/adb" shell am start -n example.myapp/.MainActivity
