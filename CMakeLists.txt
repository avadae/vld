cmake_minimum_required(VERSION 3.20)
project(VisualLeakDetector VERSION 2.0 LANGUAGES CXX)

# Install headers
install(FILES
    src/vld.h
    src/vld_def.h
    DESTINATION include
)

# Install runtime files
install(FILES
    vld.dll
    dbghelp.dll
    vld.ini
    DESTINATION bin
)

# Create CMake package configuration
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/vldConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_file(
    cmake/vldConfig.cmake.in
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
    DESTINATION lib/cmake/vld
)
