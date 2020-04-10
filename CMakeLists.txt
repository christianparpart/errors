cmake_minimum_required(VERSION 3.10)

project(errors VERSION "0.0.0" LANGUAGES CXX)

set(MASTER_PROJECT OFF)
if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    set(MASTER_PROJECT ON)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ${MASTER_PROJECT})

option(ERRORS_TESTING "errors: enables building of unit tests (requires catch2) [default: ${MASTER_PROJECT}" ${MASTER_PROJECT})
option(ERRORS_EMBEDDED_CATCH2 "errors: uses embedded catch2 for testing [default: ${MASTER_PROJECT}]" ${MASTER_PROJECT})
option(ERRORS_EMBEDDED_FMTLIB "errors: uses embedded fmtlib [default: ${MASTER_PROJECT}" ${MASTER_PROJECT})
option(ERRORS_COVERAGE "errors: Builds with codecov [default: OFF]" OFF)

# setting defaults
if (NOT("${CMAKE_CXX_STANDARD}"))
    set(CMAKE_CXX_STANDARD 20)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang"))
    add_compile_options(-Wall)
    add_compile_options(-Wextra)
elseif(DEFINED MSVC)
    add_definitions(-DNOMINMAX)
endif()

option(ERRORS_ENABLE_TIDY "Enable clang-tidy [default: OFF]" OFF)
if(ERRORS_ENABLE_TIDY)
    find_program(CLANG_TIDY_EXE
        NAMES clang-tidy-10 clang-tidy-9 clang-tidy-8 clang-tidy-7 clang-tidy
        DOC "Path to clang-tidy executable")
    if(NOT CLANG_TIDY_EXE)
        message(STATUS "clang-tidy not found.")
    else()
        message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
        set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    endif()
endif()

if(ERRORS_COVERAGE AND NOT MSVC)
    add_compile_options(-g --coverage)
    add_definitions(-DERRORS_COVERAGE=1)
    set(CMAKE_EXE_LINKER_FLAGS "--coverage ${CMAKE_EXE_LINKER_FLAGS}")
    message("-- [code coverage] Enabled.")
else()
    message("-- [code coverage] Disabled.")
endif()

# ----------------------------------------------------------------------------
# dependencies

if(ERRORS_EMBEDDED_FMTLIB)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/fmt" EXCLUDE_FROM_ALL)
    add_definitions(-DFMT_USE_WINDOWS_H=0)
else()
    # master project must provide its own fmtlib
endif()

if(ERRORS_TESTING AND ERRORS_EMBEDDED_CATCH2)
    set(CATCH_INSTALL_DOCS OFF CACHE BOOL "Catch2: Install documentation.")
    set(CATCH_INSTALL_HELPERS OFF CACHE BOOL "Catch2: Install helpers.")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/catch2")
else()
    # master project must provide its own catch2
endif()

# ----------------------------------------------------------------------------
# LIBRARY

set(ERRORS_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/errors.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/errors/Category.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/errors/Message.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/errors/Registry.h
)

add_library(errors INTERFACE)
target_sources(errors INTERFACE ${ERRORS_SRCS})

target_include_directories(errors INTERFACE
    $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>)
target_compile_features(errors INTERFACE cxx_std_17)

# ----------------------------------------------------------------------------
# TEST

if(ERRORS_TESTING)
    enable_testing()
    add_executable(test_errors
        test/test_main.cpp
        test/test_simple.cpp
        test/test_example_solidity.cpp
    )
    target_link_libraries(test_errors errors fmt::fmt-header-only Catch2::Catch2)
    add_test(test_errors ./test_errors)
endif()
