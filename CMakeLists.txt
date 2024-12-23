cmake_minimum_required(VERSION 3.20)
project(VisualLeakDetector VERSION 2.0 LANGUAGES CXX)

# Install headers
install(FILES
    src/vld.h
    src/vld_def.h
    DESTINATION include
)

# Install runtime files for x64
install(FILES
    bin/x64/Release/vld_x64.dll
    bin/x64/Release/vld_x64.pdb
    bin/x64/Release/vld.lib
    vld.ini
    setup/dbghelp/x64/dbghelp.dll
    setup/dbghelp/x64/Microsoft.DTfW.DHL.manifest
    DESTINATION bin/x64
)

# Install runtime files for Win32
install(FILES
    bin/Win32/Release/vld_x86.dll
    bin/Win32/Release/vld_x86.pdb
    bin/Win32/Release/vld.lib
    vld.ini
    setup/dbghelp/x86/dbghelp.dll
    setup/dbghelp/x86/Microsoft.DTfW.DHL.manifest
    DESTINATION bin/Win32
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