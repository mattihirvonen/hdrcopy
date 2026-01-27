
# \Users\mattihirvonen\.platformio\packages\framework-arduinoespressif32\libraries

USER := mattihirvonen

# Linux "libraries" path
LIBRARIES_USER     := ~/Arduino/libraries
LIBRARIES_IDE      := ~/.arduino15/packages/esp32/hardware/esp32/3.3.5/libraries
LIBRARIES_CORE     := ~/.arduino15/packages/esp32/hardware/esp32/3.3.5/cores/esp32
LIBRARIES_STAGING  := ~/.arduino15/libraries

# Windows libraries
LIBS_PLATFORMIO := /users/$(USER)/.platformio/packages/framework-arduinoespressif32/libraries

all:     linux
linux:   hdrcopy
windows: hdrcopy.exe

#----------------------------------------
# CodeBlocks tricks for windows

lnxDebug:    hdrcopy
lnxRelease:  hdrcopy

winDebug:    hdrcopy.exe
winRelease:  hdrcopy.exe

debugClean:    winClean
releaseClean:  winClean

#----------------------------------------
#  Linux

hdrcopy: Makefile hdrcopy.cpp
	g++  -DLINUX  hdrcopy.cpp -o hdrcopy


clean:
	rm -f  hdrcopy


lnxCopy:
	./hdrcopy   $(LIBRARIES_USER)     libhdr-user     -x
	./hdrcopy   $(LIBRARIES_IDE)      libhdr-ide      -x
	./hdrcopy   $(LIBRARIES_CORE)     libhdr-core     -x
	./hdrcopy   $(LIBRARIES_STAGING)  libhdr-staging  -x

#----------------------------------------
# Windows

hdrcopy.exe: Makefile hdrcopy.cpp
	g++  -DWINDOWS    hdrcopy.cpp -o hdrcopy.exe


winClean:
	del   hdrcopy.exe


# ToDo....
winCopy:
	./hdrcopy.exe   $(LIBS_PLATFORMIO)  libhdr-win  -x
#	./hdrcopy       $(CORE_LIBRARIES)   libhdr      -x
#	./hdrcopy       $(USER_LIBRARIES)   libhdr      -x
