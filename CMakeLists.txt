cmake_minimum_required (VERSION 3.6)
project (whyblocked
         VERSION 0.10.6
         LANGUAGES CXX
)

include(GNUInstallDirs)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBXDG_BASEDIR REQUIRED libxdg-basedir)
pkg_check_modules(SQLITE3 REQUIRED sqlite3)
find_package(Qt5Core CONFIG REQUIRED)
if(NOT WITHOUT_GUI)
    find_package(Qt5Widgets CONFIG REQUIRED)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -Og")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

include_directories(${PROJECT_BINARY_DIR})
include_directories(${LIBXDG_BASEDIR_INCLUDE_DIRS})
include_directories(${SQLITE3_INCLUDE_DIRS})

link_directories(${LIBXDG_BASEDIR_LIBRARY_DIRS})
link_directories(${SQLITE3_LIBRARY_DIRS})

# Write version in header
configure_file (
    "${PROJECT_SOURCE_DIR}/src/version.hpp.in"
    "${PROJECT_BINARY_DIR}/version.hpp"
)

set(COMMON_LIBRARIES
    ${LIBXDG_BASEDIR_LIBRARIES} vsqlitepp stdc++fs)

add_executable(whyblocked src/interface_text.cpp src/whyblocked.cpp)
target_link_libraries(whyblocked ${COMMON_LIBRARIES} Qt5::Core)
install(TARGETS whyblocked DESTINATION ${CMAKE_INSTALL_BINDIR})

if(NOT WITHOUT_GUI)
    add_executable(whyblocked-gui src/interface_qt.cpp src/whyblocked.cpp)
    target_link_libraries(whyblocked-gui ${COMMON_LIBRARIES} Qt5::Widgets)

    install(TARGETS whyblocked-gui DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(FILES de.tastytea.Whyblocked.desktop DESTINATION
            ${CMAKE_INSTALL_DATAROOTDIR}/applications)
    install(FILES de.tastytea.Whyblocked.appdata.xml DESTINATION
            ${CMAKE_INSTALL_DATAROOTDIR}/metainfo)
    install(FILES de.tastytea.Whyblocked.svg DESTINATION
            ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps)
endif()

if(NOT WITHOUT_TRANSLATIONS)
  add_subdirectory(translations)
endif()

include(packages.CMakeLists.txt)
