cmake_minimum_required (VERSION 3.2)
project (whyblocked
         VERSION 0.14.0
         LANGUAGES CXX
)

include(GNUInstallDirs)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBXDG_BASEDIR REQUIRED libxdg-basedir)
# sqlite3 is not a direct dependency, but vsqlite++ has no cmake- or pkg-config
# module. Since it installs in the same directories as sqlite3, I am adding the
# module here to add the include- and link directories below. It is not REQUIRED
# because the sqlite3 in Debian jessie doesn't come with a pkg-config module.
pkg_check_modules(SQLITE3 sqlite3)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
pkg_check_modules(LIBCONFIG REQUIRED libconfig++)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -Wpedantic -Wall -Wextra -g -Og -fno-omit-frame-pointer")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

include_directories(${PROJECT_BINARY_DIR})
include_directories(${LIBXDG_BASEDIR_INCLUDE_DIRS})
include_directories(${SQLITE3_INCLUDE_DIRS})
include_directories(${LIBCONFIG_INCLUDE_DIRS})

link_directories(${LIBXDG_BASEDIR_LIBRARY_DIRS})
link_directories(${SQLITE3_LIBRARY_DIRS})
link_directories(${LIBCONFIG_LIBRARY_DIRS})

# Write version in header
configure_file (
    "${PROJECT_SOURCE_DIR}/src/version.hpp.in"
    "${PROJECT_BINARY_DIR}/version.hpp"
)

set(COMMON_LIBRARIES
    )

add_executable(whyblocked-gui
               src/interface_qt.cpp src/whyblocked.cpp src/xdgcfg.cpp)
target_link_libraries(whyblocked-gui
                      ${LIBXDG_BASEDIR_LIBRARIES} vsqlitepp Qt5::Widgets
                      ${LIBCONFIG_LIBRARIES} stdc++fs)

install(TARGETS whyblocked-gui DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES de.tastytea.Whyblocked.desktop DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/applications)
install(FILES de.tastytea.Whyblocked.appdata.xml DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/metainfo)
install(FILES de.tastytea.Whyblocked.svg DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps)

if(NOT WITHOUT_TRANSLATIONS)
  add_subdirectory(translations)
endif()

include(packages.CMakeLists.txt)
