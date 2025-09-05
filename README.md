# LIGHT ANDROID

## Overview

Lightweight alternative for Android application development and compilation, completely **bypassing the need for the full Android Studio IDE**.
Include  **Native Development Kit (NDK) support** for integrating C/C++ code and a **custom XML editor** to simplify the creation of basic application structures and UI layouts.

## Features

*   **Standalone Android Compilation:** Compile `.apk` files directly from your project source, without requiring Android Studio. 
*   **Full NDK Support:** enabling the compilation and linking of native libraries into Android applications.
*   **Custom XML Editor:** lightweight XML editor to edit the layouts

## Usage
### Create a template
```sh 
sh setup.sh --name your_name --domain your_domain --version your_android_version
```
It ensures that all needed libraries and tools are installed, and creates a template folder to code your app in.
```sh
Usage: setup.sh [OPTIONS]

Options:
  --name NAME        Specify the name
  --domain DOMAIN    Specify the domain
  --version VERSION  Specify the android version (34 by default)
  --help             Show this help message
```
### Compilation and running of application 
```sh
sh run.sh
```
It compiles the code and runs it on your android device.

### Run the editor
To create and edit new layouts, use the editor, located in ``editor`` folder.
Compile and run it with 
```sh
sh run.sh
```
## Preview
<img width="887"  alt="image" src="https://github.com/user-attachments/assets/c8579eff-950b-48a9-b00b-f3304e7cc376" />
<img width="805" height="348" alt="image2" src="https://github.com/user-attachments/assets/e46e003d-b12b-4a28-93cd-f584be908c9b" />


## TODO
- [ ] fix frame layout (add proper alignment and searlize it too)
- [ ] fix relative layout 
    - [ ] add linkages 
    - [ ] add proper stacking 
    - [ ] fix to widget to "connect"
- [ ] styling properties 
    - [ ] add icons, images 
- [ ] more widgets 
    - [ ] `ImageView`
    - [ ] `ImageButton`
    - [ ] `CheckBox`
    - [ ] `RadioButton`
    - [ ] `ProgressBar`
    - [ ] `Switch`
    - [ ] `VideoView`
    - [ ] `ListView`
- [ ] add themes 
- [ ] add custom attributes (ability to add your own's)
- [ ] docker container
- [ ] tests...


##  License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
