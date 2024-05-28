# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# search system variable VCPKG_ROOT which should path to vcpkg
if(NOT DEFINED ENV{VCPKG_ROOT})
    message(FATAL_ERROR "Environment variable VCPKG_ROOT is not set. Please set it to the path where vcpkg is installed.")
else()
    set(VCPKG_ROOT $ENV{VCPKG_ROOT})
endif()

# if CMAKE_TOOLCHAIN_FILE is not set, then set it using the VCPKG_ROOT system variable
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("MinorTowns")

# Include sub-projects.
add_subdirectory ("src")
