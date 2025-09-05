#!/bin/bash
source ./parse_args.sh
parse_flags "$@"

CACHE=myapp/.cache
source ./config.sh
if [[ ! -f $CACHE && ! -d $MYAPP ]]; then
   echo -e "${RED} creating the working directory ${NC}"
   
   echo """
FULL_PROJECT_NAME=$DOMAIN/$PROJECT_NAME
MYAPP=$PWD/$PROJECT_NAME
ANDROID_VERSION=$VERSION
BUILD_TOOLS="$SDK/build-tools/$VERSION.0.0"
PLATFORM="$SDK/platforms/android-$VERSION"
SRC=$PWD/$PROJECT_NAME/jni
DOMAIN=$DOMAIN
PROJECT_NAME=$PROJECT_NAME
   """ >> params.sh
   echo -e "${YELLOW} copying template files to $MYAPP... ${NC}\n"

   mkdir -p $PWD/myapp/build/{gen,obj,apk/lib/$NATIVE_LIB_PLATFORM}
   cp $PWD/template/* $PWD/myapp -r
fi

source ./config.sh

echo -e "${RED} project full path is: $FULL_PROJECT_NAME"
echo " name: $PROJECT_NAME, source path: $MYAPP"
echo -e " android version: $VERSION ${NC}\n\n"


if [ -d "$SDK" ]; then
    echo -e "${GREEN} updating kit... ${NC}"
else
    
    mkdir -p "$SDK"
    echo -e "${YELLOW} installing commandline tools ${NC}"
    curl -LO https://dl.google.com/android/repository/commandlinetools-linux-11076708_latest.zip
    unzip -q commandlinetools-linux-11076708_latest.zip -d "$SDK"
fi

echo -e "${CYAN} installing SDK components...${NC}"
echo y | "$SDK/cmdline-tools/bin/sdkmanager" --sdk_root="$SDK" --install \
      "platforms;android-$ANDROID_VERSION" "build-tools;$ANDROID_VERSION.0.0" "platform-tools" "ndk;26.2.11394342"

if [ -f "keystore.jks" ]; then
    echo -e "${GREEN} keystore exists${NC}"
else
    echo -e "${YELLOW} generating keystore...${NC}"
    keytool -genkeypair \
      -keystore keystore.jks \
      -alias android_key \
      -validity 10000 \
      -keyalg RSA \
      -keysize 2048 \
      -storepass android \
      -keypass android \
      -dname "CN=Your Name, OU=Your Unit, O=Your Organization, L=Your City, S=Your State, C=Your Country"
fi

if [ -d "$SDK/ndk/android-ndk-r26c" ]; then
    echo -e "${YELLOW} moving NDK directory...${NC}"
    mv "$SDK/ndk/android-ndk-r26c" "$NDK"
fi


if [[ ! -f $CACHE && ! -d $MYAPP ]]; then
   touch $CACHE 
   
   echo """
   FULL_PROJECT_NAME=$DOMAIN/$PROJECT_NAME
   MYAPP=$PWD/$PROJECT_NAME
   ANDROID_VERSION=$VERSION
   BUILD_TOOLS="$SDK/build-tools/$VERSION.0.0"
   PLATFORM="$SDK/platforms/android-$VERSION"
   SRC=$PWD/$PROJECT_NAME/jni
   DOMAIN=$DOMAIN
   PROJECT_NAME=$PROJECT_NAME
   """ >> $CACHE 

   echo -e "${CYAN} replacing project identifiers in templates...${NC}"

   echo -e "${RED} changing example_myapp to ${DOMAIN}_${PROJECT_NAME} ${NC}"
   grep -rl 'example_myapp' myapp | xargs sed -i s/example_myapp/${DOMAIN}_${PROJECT_NAME}/g

   echo -e "${RED} changing example.myapp to ${DOMAIN}.${PROJECT_NAME} ${NC}"
   grep -rl 'example.myapp' myapp | xargs sed -i s/example\.myapp/${DOMAIN}.${PROJECT_NAME}/g

   echo -e "${RED} changing myapp to $PROJECT_NAME ${NC}"
   grep -rl 'myapp' myapp | xargs sed -i s/myapp/$PROJECT_NAME/g


   echo -e "${RED} renaming files...${NC}"
   find myapp -type f -name '*myapp*' | while read file; do
     dir=$(dirname "$file")
     base=$(basename "$file")
     new_base=${base//myapp/$PROJECT_NAME}
     mv "$file" "$dir/$new_base"
   done

   echo -e "${RED} renaming directories...${NC}"
   mv myapp $MYAPP

   find $MYAPP -type d -name '*myapp*' | while read dir; do
     parent_dir=$(dirname "$dir")
     base_name=$(basename "$dir")
     new_name=${base_name//myapp/$PROJECT_NAME}
     mv "$dir" "$parent_dir/$new_name"
   done
   find $MYAPP -type d -name '*example*' | while read dir; do
     parent_dir=$(dirname "$dir")
     base_name=$(basename "$dir")
     new_name=${base_name//example/$DOMAIN}
     mv "$dir" "$parent_dir/$new_name"
   done

fi
echo -e "${GREEN} all operations are completed!${NC}"

