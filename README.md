# libtfmpvalue

A cmake overlay for TFMpvalue C++ library

This repo serves as a wrapper for the incredibly useful [TFM-Pvalue C++ program](http://bioinfo.lifl.fr/tfm-pvalue/tfm-pvalue.php). It allows users to determine score thresholds for a given transcription factor position frequency matrix associated with a specific p-value. Naturally, it can also perform the reverse, quickly calculating an accurate p-value from a score for a given motif matrix.

``libtfmpvalue`` allows this functionality to be easily exploited in any cmake-based source builds.
It does several things, that can be turned on/off via cmake options : 
  - core sources are built as a (static) library  (minimum target)
  - package command-lines can be compiled via dedicated targets (optionnal)
  - tests from the original packages are implemented as cmake tests (optionnal)
  - some compilation flags can be turned on/off for debugging

Original sources can be found at http://bioinfo.lifl.fr/tfm-pvalue/tfm-pvalue.php.
**This project'sources has been archived here and are provided as-is, with no additional support or development.**


Cmake installation
---------------

To use the library add these lines to your `CMakeLists.txt` file : 

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
        GIT_TAG           "1.0.0"
)
FetchContent_MakeAvailable(libtfmpvalue)
add_library(libtfmpvalue STATIC IMPORTED)
set_target_properties(libtfmpvalue PROPERTIES IMPORTED_LOCATION ${libtfmpvalue_BINARY_DIR}/libtfmpvalue.a)


########################
## LINK YOUR CMAKE TARGET

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

Any and all contributions to the cmake overlay are welcome. 


Reference
--------------
Efficient and accurate P-value computation for Position Weight Matrices \
H. Touzet and J.S. Varré \
Algorithms for Molecular Biology 2007, 2:15 \
https://doi.org/10.1186/1748-7188-2-15


License
-----------

This project is licensed under the GPL2 license, as the original sources. 
You are free to use, modify, and distribute it as you see fit.
The program is provided as is, with no guarantees.
