# Text Games System

A tiny board with a PIC16F876, an I2C EEPROM and a serial port.
See http://adrien-ricciardi.pagesperso-orange.fr/Projects/Text_Games_System/Text_Games_System.html for more details.

## Getting sources
Use the following command to clone the repository and all git submodules in the same time :
```
git clone --recursive https://github.com/RICCIARDI-Adrien/Text_Games_System
```

## Build tools
* The microcontroller firmwares are compiled with Sourceboost BoostC 7.22.
* The PC programs and tools are compiled with MinGW gcc.

## Build environment
You have to define a Windows environment variable called "SOURCEBOOST_PATH" to tell the build system where are the build tools.
The variable must contain the path to the SourceBoost installation directory, like "C:\Program Files\SourceBoost". Be careful, the path must be surrounded by quotes (").
