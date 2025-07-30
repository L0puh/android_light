#!/bin/bash

source ./config.sh
set -e

echo -e "${YELLOW} Packing resources... ${NC}"

"$BUILD_TOOLS/aapt" package -f -m -J $MYAPP/build/gen/ -S $MYAPP/res \
      -M $MYAPP/AndroidManifest.xml -I "$PLATFORM/android.jar"

cd "$MYAPP/build"

cmake -DCMAKE_TOOLCHAIN_FILE="$NDK/build/cmake/android.toolchain.cmake" \
      -DANDROID_ABI=$NATIVE_LIB_PLATFORM \
      -DANDROID_PLATFORM=android-$ANDROID_VERSION \
      "$SRC" 

cmake --build . --target native-lib -- -j$(nproc) 

cd "$ROOT"
mv "$MYAPP/build/libnative-lib.so" "$MYAPP/build/apk/lib/$NATIVE_LIB_PLATFORM/"

echo -e "${YELLOW} Compiling Java code... ${NC}"

cd "$MYAPP"
echo -e "${BLUE} Generating R.java and compiling Java sources... ${NC}"
javac -h "$SRC" --release "$RELEASE" -classpath "$PLATFORM/android.jar" -d build/obj build/gen/$FULL_PROJECT_NAME/R.java java/$FULL_PROJECT_NAME/MainActivity.java

echo -e "${BLUE} Creating bytecode files... ${NC}"
"$BUILD_TOOLS/d8" --release --lib "$PLATFORM/android.jar" --output build/apk/ build/obj/$FULL_PROJECT_NAME/*.class

cd "$ROOT"
echo -e "${BLUE} Packaging APK... ${NC}"
"$BUILD_TOOLS/aapt" package -f -M "$MYAPP/AndroidManifest.xml" -S "$MYAPP/res/" -I "$PLATFORM/android.jar" -F "$MYAPP/build/$PROJECT_NAME.unsigned.apk" "$MYAPP/build/apk/"

echo -e "${BLUE} Signing APK... ${NC}"
"$BUILD_TOOLS/zipalign" -f -p 4 "$MYAPP/build/$PROJECT_NAME.unsigned.apk" "$MYAPP/build/$PROJECT_NAME.aligned.apk"

"$BUILD_TOOLS/apksigner" sign --ks keystore.jks --ks-key-alias android_key --ks-pass pass:android --key-pass pass:android --out "$MYAPP/build/$PROJECT_NAME.apk" "$MYAPP/build/$PROJECT_NAME.aligned.apk"

echo -e "${YELLOW}==============================${NC}"
echo -e "${YELLOW} $PROJECT_NAME is done! Your APK is at: ${NC}"
echo -e "${YELLOW} $MYAPP/build/$PROJECT_NAME.apk ${NC}"
echo -e "${YELLOW}==============================${NC}"

echo -e "${RED} Installing APK on device..."
"$SDK/platform-tools/adb" install -r "$MYAPP/build/$PROJECT_NAME.apk" 

echo -e "Launching app on ${DOMAIN}.${PROJECT_NAME}... ${NC}"

"$SDK/platform-tools/adb" shell am start -n ${DOMAIN}.${PROJECT_NAME}/.MainActivity
