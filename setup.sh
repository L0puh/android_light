SDK="$PWD/android_sdk"
NDK="$SDK/ndk/26.2.11394342"

if [ -d "$SDK" ]; then

   echo "-------------------------------------------------------------------------"
   echo "                           UPDATING KIT                                  "
   echo "-------------------------------------------------------------------------"
else
   mkdir -p android_sdk
   echo "-------------------------------------------------------------------------"
   echo "                           INSTALLING CLI TOOLS                          "
   echo "-------------------------------------------------------------------------"
   curl -LO https://dl.google.com/android/repository/commandlinetools-linux-11076708_latest.zip
   unzip -q commandlinetools-linux-11076708_latest.zip -d "$SDK"
   echo "-------------------------------------------------------------------------"
   echo "                           INSTALLING PLATFORMS                          "
   echo "-------------------------------------------------------------------------"


fi

echo y | "$SDK/cmdline-tools/bin/sdkmanager" --sdk_root="$SDK" --install \
      "platforms;android-34" "build-tools;34.0.0" "platform-tools" "ndk;26.2.11394342"

if [ -f "keystore.jks" ];
then 
   echo "KEY EXISTS"
else
   keytool -genkeypair \
     -keystore keystore.jks \
     -alias androidkey \
     -validity 10000 \
     -keyalg RSA \
     -keysize 2048 \
     -storepass android \
     -keypass android \
     -dname "CN=Your Name, OU=Your Unit, O=Your Organization, L=Your City, S=Your State, C=Your Country"
fi

if [ -d "$SDK/ndk/android-ndk-r26c" ]; then
   mv "$SDK/ndk/android-ndk-r26c" "$NDK"
fi
