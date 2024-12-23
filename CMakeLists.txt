cmake_minimum_required(VERSION 3.20)
project(VisualLeakDetector VERSION 2.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/setup)

# Add source files
set(SOURCES
    src/callstack.cpp
    src/dllspatches.cpp
    src/ntapi.cpp
    src/stdafx.cpp
    src/utility.cpp
    src/vld_hooks.cpp
    src/vld.cpp
    src/vldapi.cpp
    src/vldheap.cpp
)

# Add the library target
add_library(vld SHARED ${SOURCES})

# Set properties for the library
set_target_properties(vld PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# Add precompiled header
target_precompile_headers(vld PRIVATE src/stdafx.h)

target_compile_options(vld PRIVATE /W4 /WX)

# Define Unicode for the project
target_compile_definitions(vld PRIVATE UNICODE _UNICODE)

# Install headers
install(FILES
    src/vld.h
    src/vld_def.h
    DESTINATION include
)

# Install runtime files based on the target platform
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64-bit
    install(FILES
        setup/dbghelp/x64/dbghelp.dll
        setup/dbghelp/x64/Microsoft.DTfW.DHL.manifest
        vld.ini
        DESTINATION bin/x64
    )
else()
    # 32-bit
    install(FILES
        setup/dbghelp/x86/dbghelp.dll
        setup/dbghelp/x86/Microsoft.DTfW.DHL.manifest
        vld.ini
        DESTINATION bin/x86
    )
endif()

# Install the library target
install(TARGETS vld
    EXPORT vld-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# Create CMake package configuration
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/vldConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_file(
    vldConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/vldConfig.cmake"
    @ONLY
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/vldConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/vldConfigVersion.cmake"
    DESTINATION lib/cmake/vld
)

# Export targets for CMake
install(EXPORT vld-targets
    NAMESPACE vld::
    FILE vld-targets.cmake
    DESTINATION lib/cmake/vld
)

# Register the package in the user's package registry
export(PACKAGE vld)