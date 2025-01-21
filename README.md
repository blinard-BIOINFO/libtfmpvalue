# libtfmpvalue

A cmake overlay for TFMpvalue C++ library

This repo serves as a wrapper for the incredibly useful [TFM-Pvalue C++ program](http://bioinfo.lifl.fr/tfm-pvalue/tfm-pvalue.php). It allows users to determine score thresholds for a given transcription factor position frequency matrix associated with a specific p-value. Naturally, it can also perform the reverse, quickly calculating an accurate p-value from a score for a given motif matrix.

``libpytfmpval`` allows this functionality to be easily exploited via a cmake overlay.
This overlay does several things : 
  - core sources are built as a (static) library
  - package command-lines can be compiled via dedicated targets
  - tests from the original packages are implemented as cmake tests

Original sources can be found at http://bioinfo.lifl.fr/tfm-pvalue/tfm-pvalue.php.

**This project has been archived and is provided as-is, with no additional support or development.**

Cmake installation
---------------

To use the library, add these lines to your `CMakeLists.txt` file : 

```cmake

########################
## LIBRARY RETRIEVAL

# options that can be set ON or OFF
option(DEBUG_TFMPVALUE "If on, no optimisations and all warnings" OFF)
option(BUILD_TFMPVALUE_BINARIES "Build binaries on top of library (pv2sc,sc2pv,...)" OFF)
option(RUN_TFMPVALUE_TESTS "Run libtfmpvalue cmake tests" OFF)

# source retrival & static library build
FetchContent_Declare(
        libtfmpvalue
        GIT_REPOSITORY    "https://github.com/blinard-BIOINFO/libtfmpvalue.git"
        GIT_TAG           "35e68bc114a5339aed3d82ad5f01af641b2064ca"
)
FetchContent_MakeAvailable(libtfmpvalue)
add_library(libtfmpvalue STATIC IMPORTED)
set_target_properties(libtfmpvalue PROPERTIES IMPORTED_LOCATION ${libtfmpvalue_BINARY_DIR}/libtfmpvalue.a)


########################
## YOUR CMAKE TARGET

add_executable(
  mybinary
  src/main.cpp
)
target_include_directories(
  mybinary
  ${libtfmpvalue_SOURCE_DIR}/include       # <-- !
)
target_link_libraries(
    mybinary
    PRIVATE
    libtfmpvalue                           # <-- !
)
```

Contribute
---------------

Any and all contributions are welcome. 


Reference
--------------

| Efficient and accurate P-value computation for Position Weight Matrices
| H. Touzet and J.S. Varré
| *Algorithms for Molecular Biology 2007, 2:15*

License
-----------

This project is licensed under the GPL2 license, as the original sources. 
You are free to use, modify, and distribute it as you see fit.
The program is provided as is, with no guarantees.
