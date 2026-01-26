
# Linux "libraries" path
CORE_LIBRARIES := ~/.arduino15/libraries
USER_LIBRARIES := ~/Arduino/libraries

all:     linux
linux:   hdrcopy
windows: hdrcopy.exe

#----------------------------------------
# CodeBlocks tricks for windows

Debug:   hdrcopy.exe
Release: hdrcopy.exe

debugClean:    winClean
releaseClean:  winClean

#----------------------------------------
#  Linux

hdrcopy: Makefile hdrcopy.cpp
	g++  -DLINUX  hdrcopy.cpp -o hdrcopy


clean:
	rm -f  hdrcopy


copy:
	./hdrcopy   $(CORE_LIBRARIES)   libhdr  -x
	./hdrcopy   $(USER_LIBRARIES)   libhdr  -x


#----------------------------------------
# Windows

hdrcopy.exe: Makefile hdrcopy.cpp
	g++  -DWINDOWS    hdrcopy.cpp -o hdrcopy.exe


winClean:
	del   hdrcopy.exe


# ToDo....
wincopy:
	./hdrcopy   $(CORE_LIBRARIES)   libhdr  -x
	./hdrcopy   $(USER_LIBRARIES)   libhdr  -x
