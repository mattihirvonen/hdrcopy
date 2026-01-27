# Arduino IDE Library Header File Collector
This application copy recursively all source directory tree's "h" and "hpp"
header files into one destination directory. This allow CppCheck better check
code quality against header files (= include path for header files).

## Arduino Libraries Directory Structure
Arduino IDE libraries directory/file structure differs from traditional.
Following firectory structure is difficult to give as reference tree
for traditinal unix/linux oriented tool.
```
.../libraries/ ---+--- libbrary1/ --- src/ ---+--- *.cpp
                  |                           +--- *.h
                  |
                  +--- libbrary2/ --- src/ ---+--- *.cpp
                  |                           +--- *.h
                  |
                  +--- library3/ ...
```

## Application Usage
Application dryrun command (no real copy)
- **hdrcopy -v -d destinationDirectory sourceDirectoryTree**

Application real copy execution
- **hdrcopy -x -d destinationDirectory sourceDirectoryTree**

## Application Build
Application's Windows environment build has tested with CodeBlocks 25.03 (.cbp project file)
with bundled MinGW compiler package.
- Select from menu: **Build / Select target / winDebug**

Application's Linux build can do using standard command line command (make utility and G++ compiler).
Linux environment build can do also with CodeBlocks (windows installation)
- using CodeBlocks (.cbp project file) select from menu: **Build / Select target / lnxDebug**
- using ordinary Linux command line tools, use command: **make**

## ToDO:
- Fix duplicate file handling
- Add command line option "-c" copy also libraries C/C++ source code files for analyze (done)
- Add command line parsing to accept multiple source directories from command line (done)
