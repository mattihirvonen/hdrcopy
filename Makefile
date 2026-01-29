
#-------------------------------------------------------------------
# Windows

# \Users\user\Appdata\Local\Arduino15\libraries
# \Users\user\Appdata\Local\Arduino15\libraries\staging\libraries

#-------------------------------------------------------------------

USER   := mattihirvonen

PLATFORMIO_LIBRARY_VERSION      := 3.3.5
ARDUINO_LIBRARY_VERSION_LINUX   := 3.3.5
ARDUINO_LIBRARY_VERSION_WINDOWS := 3.3.6


# Windows Arduino "standard" libraries (PlatformIO, Windows)
# Project specific additional libraries import using "lib_deps" in "platformio.ini" configuration
#
LIBS_PLATFORMIO := /Users/$(USER)/.platformio/packages/framework-arduinoespressif32/libraries


# Linux Arduino IDE "libraries" paths (single file Application package executable)
#
LIBRARIES_USER     := ~/Arduino/libraries
LIBRARIES_STD      := ~/.arduino15/packages/esp32/hardware/esp32/$(ARDUINO_LIBRARY_VERSION_LINUX)/libraries
LIBRARIES_CORE     := ~/.arduino15/packages/esp32/hardware/esp32/$(ARDUINO_LIBRARY_VERSION_LINUX)/cores/esp32
LIBRARIES_STAGING  := ~/.arduino15/libraries


# Windows keep libraries in different directory structure depend build tool chain
# - "installed" Arduino IDE/CLI or
# - "portable"  Arduino IDE
#
# Windows Arduino IDE/CLI "libraries" pahts (arduino-cli.yaml)
#
#directories:
#    builtin:
#        libraries: C:\Users\$(USER)\AppData\Local\Arduino15\libraries
#    data:          c:\Users\$(USER)\AppData\Local\Arduino15
#    downloads:     C:\Users\$(USER)\AppData\Local\Arduino15\staging
#    user:          c:\Users\$(USER)\Documents\Arduino

LIBS_USER_ARDUINO := C:\Users\$(USER)\Documents\Arduino\libraries
LIBS_STD_ARDUINO  := C:\Users\$(USER)\Appdata\Arduino15\Local\packages\esp32\hardware\esp32\$(ARDUINO_LIBRARY_VERSION_WINDOWS)\libraries
LIBS_CORE_ARDUINO := C:\Users\$(USER)\Appdata\Arduino15\Local\libraries

#-------------------------------------------------------------------

all:     linux
linux:   hdrcopy
windows: hdrcopy.exe

CFLAGS := -Wall -Wextra

#-------------------------------------------------------------------
# CodeBlocks tricks for windows

lnxDebug:    hdrcopy
lnxRelease:  hdrcopy

winDebug:    hdrcopy.exe
winRelease:  hdrcopy.exe

debugClean:    winClean
releaseClean:  winClean

#-------------------------------------------------------------------
#  Linux

hdrcopy: Makefile   hdrcopy.cpp
	g++  $(CFLACS)  hdrcopy.cpp -o hdrcopy


clean:
	rm -f  hdrcopy


lnxCopy:
	./hdrcopy -vxc -d $(LIBRARIES_USER)     libhdr-user
	./hdrcopy -vxc -d $(LIBRARIES_STD)      libhdr-std
	./hdrcopy -vxc -d $(LIBRARIES_CORE)     libhdr-core
	./hdrcopy -vxc -d $(LIBRARIES_STAGING)  libhdr-staging


# Test single line copy to one common directory
singleCopy:
#	./hdrcopy -vcx -d libhdr-arduino  $(LIBRARIES_STAGING)  $(LIBRARIES_CORE)  $(LIBRARIES_STD)
	./hdrcopy -vcx -d libhdr-common   $(LIBRARIES_STAGING)  $(LIBRARIES_CORE)  $(LIBRARIES_STD)  $(LIBRARIES_USER)

#-------------------------------------------------------------------
# Windows

hdrcopy.exe: Makefile   hdrcopy.cpp
	g++      $(CFLACS)  hdrcopy.cpp -o hdrcopy.exe


winClean:
	del  hdrcopy.exe


# ToDo....
winCopy:
	./hdrcopy.exe   $(LIBS_PLATFORMIO)  libhdr-win  -x
#	./hdrcopy       $(CORE_LIBRARIES)   libhdr      -x
#	./hdrcopy       $(USER_LIBRARIES)   libhdr      -x
