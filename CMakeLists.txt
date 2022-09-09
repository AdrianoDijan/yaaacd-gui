cmake_minimum_required(VERSION 3.9)

project(yaaacd-gui CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

SET(FEATURE_qt3d_system_assimp OFF)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(
    STATUS
      "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(
    DOWNLOAD
    "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
    "${CMAKE_BINARY_DIR}/conan.cmake" TLS_VERIFY ON)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(
  REQUIRES
  cgal/5.3
  gmp/6.2.1
  qtbase/6.2.3@qt/everywhere
  qt3d/6.2.3@qt/everywhere
  yaaacd/1.0
  GENERATORS
  cmake_find_package
  OPTIONS
  qt3d:cmake_args_leaf_module="-DFEATURE_qt3d_system_assimp=OFF"
  )
conan_cmake_autodetect(settings)
conan_cmake_install(
  PATH_OR_REFERENCE
  .
  BUILD
  missing
  REMOTE
  conancenter
  SETTINGS
  ${settings})

find_package(Qt6 CONFIG REQUIRED COMPONENTS Core Gui Widgets 3DCore 3DExtras)
find_package(yaaacd REQUIRED)
find_package(CGAL CONFIG REQUIRED)
find_package(gmp REQUIRED)

file(GLOB SOURCE_FILES src/*.cpp include/*.hpp)
add_executable(yaaacd-gui ${SOURCE_FILES})
target_include_directories(yaaacd-gui PRIVATE ${yaaacd_INCLUDE_DIRS})
target_link_libraries(yaaacd-gui Qt6::Core Qt6::3DCore Qt6::3DExtras Qt6::Gui Qt6::Widgets ${yaaacd_LIBS} CGAL ${gmp_LIBRARIES})
