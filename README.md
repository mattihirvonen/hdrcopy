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
- **hdrcopy  source_directory_tree  destination directory**

Application real copy execution
- **hdrcopy  source_directory_tree  destination directory  -x**

## Application Build
Windows environment build will do with CodeBlocks (.cbp project file).
Used CodeBlocks package should contain MinGW compiler installation.

Linux build will do using standard command line command (make utility and G++ compiler)
- make linux


