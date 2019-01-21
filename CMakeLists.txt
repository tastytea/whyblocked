cmake_minimum_required (VERSION 3.2)
project (whyblocked
         VERSION 0.15.0
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

add_executable(${CMAKE_PROJECT_NAME}
               src/qt/main.cpp src/qt/mainwindow.cpp src/qt/dialog_add.cpp
               src/whyblocked.cpp src/xdgcfg.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME}
                      ${LIBXDG_BASEDIR_LIBRARIES} vsqlitepp Qt5::Widgets
                      ${LIBCONFIG_LIBRARIES} stdc++fs)
set(WITH_MAN "YES" CACHE STRING "WITH_MAN defaults to \"YES\"")
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
                   WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
                   COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_PROJECT_NAME} whyblocked-gui)

if(WITH_MAN)
  add_custom_command(
                     OUTPUT "${PROJECT_BINARY_DIR}/whyblocked.1"
                     WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
                     DEPENDS "${CMAKE_SOURCE_DIR}/whyblocked.1.adoc"
                     COMMAND ${CMAKE_SOURCE_DIR}/build_manpage.sh
                     ARGS ${PROJECT_VERSION})
  add_custom_target(run ALL
                    DEPENDS "${PROJECT_BINARY_DIR}/whyblocked.1"
)
endif()

install(TARGETS ${CMAKE_PROJECT_NAME}
        DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES "${PROJECT_BINARY_DIR}/whyblocked-gui"
        DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(FILES de.tastytea.Whyblocked.desktop
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
install(FILES de.tastytea.Whyblocked.appdata.xml
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/metainfo)
install(FILES de.tastytea.Whyblocked.svg
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps)
if(WITH_MAN)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/whyblocked.1
          DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
endif()

set(WITH_TRANSLATIONS "YES" CACHE STRING "WITH_TRANSLATIONS defaults to \"YES\"")
if(WITH_TRANSLATIONS)
  add_subdirectory(translations)
endif()

include(packages.CMakeLists.txt)
