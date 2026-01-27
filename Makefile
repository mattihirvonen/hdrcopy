
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
	./hdrcopy -vx  -d $(LIBRARIES_USER)     libhdr-user
	./hdrcopy -vx  -d $ LIBRARIES_IDE)      libhdr-ide
	./hdrcopy -vx  -d $(LIBRARIES_CORE)     libhdr-core
	./hdrcopy -vx  -d $(LIBRARIES_STAGING)  libhdr-staging

# Test single line copy to one common directory
singleCopy:
	./hdrcopy -v  -d libhdr-common  $(LIBRARIES_USER)  $(LIBRARIES_IDE)  $(LIBRARIES_CORE)  $(LIBRARIES_STAGING)

#----------------------------------------
# Windows

hdrcopy.exe: Makefile hdrcopy.cpp
	g++  -DWINDOWS  hdrcopy.cpp -o hdrcopy.exe


winClean:
	del   hdrcopy.exe


# ToDo....
winCopy:
	./hdrcopy.exe   $(LIBS_PLATFORMIO)  libhdr-win  -x
#	./hdrcopy       $(CORE_LIBRARIES)   libhdr      -x
#	./hdrcopy       $(USER_LIBRARIES)   libhdr      -x
