cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(TFM-Pvalue LANGUAGES CXX VERSION 1.00)
set (CMAKE_CXX_STANDARD 11)

option(BUILD_TEST "Run tests" ON)

add_compile_options(
    -O3
    -Wall
)

########################
## LIBRARY

# build a static library from core sources
add_library(
        tfmpvalue
        STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Matrix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ParseException.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/FileException.cpp
)

########################
## EXECUTABLES

# applies to all following targets
link_libraries(
        tfmpvalue
)

add_executable(
        TFMpvalue-pv2sc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ArgumentException.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/TFMpvalue.cpp
)
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
)
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
)
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
)
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
)
target_compile_definitions(
        TFMpvalue-lazydistrib
        PUBLIC
        JASPAR=1
        PROGRAM=5
)


########################
## TESTS

if (BUILD_TEST)
    enable_testing()

    add_test(
            NAME
            pv2sc
            COMMAND
            $<TARGET_FILE:TFMpvalue-pv2sc> -a 0.25 -t 0.25 -c 0.25 -g 0.25 -m ${CMAKE_CURRENT_SOURCE_DIR}/test/MA0045.pfm -p 1e-5
    )
    add_test(
            NAME
            sc2pv
            COMMAND
            $<TARGET_FILE:TFMpvalue-sc2pv> -a 0.25 -t 0.25 -c 0.25 -g 0.25 -m ${CMAKE_CURRENT_SOURCE_DIR}/test/MA0045.pfm -s 8.77
    )
    add_test(
            NAME
            distrib
            COMMAND
            $<TARGET_FILE:TFMpvalue-distrib> -a 0.25 -t 0.25 -c 0.25 -g 0.25 -m ${CMAKE_CURRENT_SOURCE_DIR}/test/MA0045.pfm -s 8 -S 9 -G 1e-5
    )

endif(BUILD_TEST)
