cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(TFM-Pvalue LANGUAGES CXX VERSION 1.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) #For absl lib, On GCC/Clang, it avoids -std=gnu++17, Instead uses -std=c++17

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")


unset(DEBUG_TFMPVALUE CACHE)
unset(BUILD_TFMPVALUE_BINARIES CACHE)
unset(RUN_TFMPVALUE_TESTS CACHE)
unset(RUN_TFMPVALUE_TESTS_VERBOSE CACHE)

########################
### modifiable options
option(DEBUG_TFMPVALUE "If on, no optimisations and all warnings" ON)
option(BUILD_TFMPVALUE_BINARIES "Build binaries on top of library (pv2sc,sc2pv,...)" OFF)
option(RUN_TFMPVALUE_TESTS "Run libtfmpvalue cmake tests" ON)
option(RUN_TFMPVALUE_TESTS_VERBOSE "Run libtfmpvalue cmake tests, with high verbosity" OFF)
### modifiable options
########################

if(RUN_TFMPVALUE_TESTS)
    set(BUILD_TFMPVALUE_BINARIES ON)
endif(RUN_TFMPVALUE_TESTS)

if (DEBUG_TFMPVALUE)
    add_compile_options(
        -O0
        -Wall
    )
else()
    add_compile_options(
        -O3
    )
endif (DEBUG_TFMPVALUE)

########################
## Dependency
include(FetchContent)

set(ABSL_BUILD_TESTING OFF)
set(ABSL_ENABLE_INSTALL OFF)
set(ABSL_USE_EXTERNAL_GOOGLETEST OFF)
FetchContent_Declare(
        absl
        GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
        GIT_TAG 20260107.1
)

FetchContent_MakeAvailable(absl)


########################
## LIBRARY

# build a static library from core sources
add_library(
        tfmpvalue
        STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Matrix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ParseException.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/FileException.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Parsers.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Parsers.h)
target_link_libraries(
        tfmpvalue
        PUBLIC
        absl::btree)

########################
## EXECUTABLES

if (BUILD_TFMPVALUE_BINARIES)
    # static linking applies to all following targets
    link_libraries(
        tfmpvalue
    )

    add_executable(
            TFMpvalue-pv2sc
            ${CMAKE_CURRENT_SOURCE_DIR}/src/ArgumentException.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/TFMpvalue.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Parsers.cpp include/Parsers.h)
    target_compile_definitions(
            TFMpvalue-pv2sc
            PUBLIC
            JASPAR=1
            PROGRAM=0
    )
    add_executable(
            TFMpvalue-sc2pv
            ${CMAKE_CURRENT_SOURCE_DIR}/src/ArgumentException.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/TFMpvalue.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Parsers.cpp include/Parsers.h)
    target_compile_definitions(
            TFMpvalue-sc2pv
            PUBLIC
            JASPAR=1
            PROGRAM=1
    )
    add_executable(
            TFMpvalue-distrib
            ${CMAKE_CURRENT_SOURCE_DIR}/src/ArgumentException.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/TFMpvalue.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Parsers.cpp include/Parsers.h)
    target_compile_definitions(
            TFMpvalue-distrib
            PUBLIC
            JASPAR=1
            PROGRAM=3
    )
    add_executable(
            TFMpvalue-fastpvalue
            ${CMAKE_CURRENT_SOURCE_DIR}/src/ArgumentException.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/TFMpvalue.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Parsers.cpp include/Parsers.h)
    target_compile_definitions(
            TFMpvalue-fastpvalue
            PUBLIC
            JASPAR=1
            PROGRAM=4
    )
    add_executable(
            TFMpvalue-lazydistrib
            ${CMAKE_CURRENT_SOURCE_DIR}/src/ArgumentException.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/TFMpvalue.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Parsers.cpp include/Parsers.h)
    target_compile_definitions(
            TFMpvalue-lazydistrib
            PUBLIC
            JASPAR=1
            PROGRAM=5
    )
endif(BUILD_TFMPVALUE_BINARIES)

########################
## TESTS

if (RUN_TFMPVALUE_TESTS)
    enable_testing()

    if (RUN_TFMPVALUE_TESTS_VERBOSE)
        add_compile_definitions(
            PUBLIC
            VERBOSE=1
        )
    endif(RUN_TFMPVALUE_TESTS_VERBOSE)

    add_test(
            NAME
            pv2sc
            COMMAND
            $<TARGET_FILE:TFMpvalue-pv2sc> -a 0.25 -t 0.25 -c 0.25 -g 0.25 -m ${CMAKE_CURRENT_SOURCE_DIR}/test/MA0045.pfm -p 1e-5 -h
    )
    set_tests_properties(
            pv2sc
            PROPERTIES
            PASS_REGULAR_EXPRESSION "8.77371" #score
            PASS_REGULAR_EXPRESSION "9.99263e-06" #pvalue
    )
    add_test(
            NAME
            sc2pv
            COMMAND
            $<TARGET_FILE:TFMpvalue-sc2pv> -a 0.25 -t 0.25 -c 0.25 -g 0.25 -m ${CMAKE_CURRENT_SOURCE_DIR}/test/MA0045.pfm -s 8.77 -h
    )
    set_tests_properties(
            sc2pv
            PROPERTIES
            PASS_REGULAR_EXPRESSION "8.77" #score
            PASS_REGULAR_EXPRESSION "1.00716e-05" #pvalue
    )
    add_test(
            NAME
            distrib
            COMMAND
            $<TARGET_FILE:TFMpvalue-distrib> -a 0.25 -t 0.25 -c 0.25 -g 0.25 -m ${CMAKE_CURRENT_SOURCE_DIR}/test/MA0045.pfm -s 8 -S 9 -G 1e-5
    )
    set_tests_properties(
            distrib
            PROPERTIES
            PASS_REGULAR_EXPRESSION "8.00005 3.18082e-05 2.32831e-10" #1st line of distrib output
            PASS_REGULAR_EXPRESSION "8.00065 3.18079e-05 4.37722e-08" #2nd
            PASS_REGULAR_EXPRESSION "8.00083 3.17642e-05 3.72529e-09" #3rd
    )
    add_test(
            NAME
            fastpvalue
            COMMAND
            $<TARGET_FILE:TFMpvalue-fastpvalue> -a 0.25 -t 0.25 -c 0.25 -g 0.25 -m ${CMAKE_CURRENT_SOURCE_DIR}/test/MA0045.pfm -s 8 -G 1e-5 -h
    )
    set_tests_properties(
            fastpvalue
            PROPERTIES
            PASS_REGULAR_EXPRESSION "8" #score
            PASS_REGULAR_EXPRESSION "136615" #pvalue
    )

endif(RUN_TFMPVALUE_TESTS)
