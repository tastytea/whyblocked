cmake_minimum_required (VERSION 3.6)
project (whyblocked
         VERSION 0.9.0
         LANGUAGES CXX
)

include(GNUInstallDirs)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBXDG_BASEDIR REQUIRED libxdg-basedir)
if(NOT WITHOUT_GUI)
    find_package(Qt5Widgets CONFIG REQUIRED)
endif()
find_package(Qt5LinguistTools)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -Og")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_BINARY_DIR})

include_directories(${LIBXDG_BASEDIR_INCLUDE_DIRS})

link_directories(${LIBXDG_BASEDIR_LIBRARY_DIRS})

# Write version in header
configure_file (
    "${PROJECT_SOURCE_DIR}/src/version.hpp.in"
    "${PROJECT_BINARY_DIR}/version.hpp"
)

if(Qt5_LRELEASE_EXECUTABLE)
    file(GLOB TS_FILES "translations/*.ts")
    file(GLOB TS_SRC_FILES "src/*")
    qt5_create_translation(QM_FILES ${TS_SRC_FILES} ${TS_FILES} OPTIONS -no-obsolete)
    qt5_add_translation(QM_FILES ${TS_FILES})
endif()

add_executable(whyblocked src/interface_text.cpp src/whyblocked.cpp)
target_link_libraries(whyblocked
                      ${LIBXDG_BASEDIR_LIBRARIES} vsqlitepp stdc++fs)
install(TARGETS whyblocked DESTINATION ${CMAKE_INSTALL_BINDIR})

if(NOT WITHOUT_GUI)
    add_executable(whyblocked-gui src/interface_qt.cpp src/whyblocked.cpp
                   ${QM_FILES})
    target_link_libraries(whyblocked-gui
                          ${LIBXDG_BASEDIR_LIBRARIES} vsqlitepp stdc++fs
                          Qt5::Widgets)

    install(TARGETS whyblocked-gui DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(FILES de.tastytea.Whyblocked.desktop DESTINATION
            ${CMAKE_INSTALL_DATAROOTDIR}/applications)
    install(FILES ${QM_FILES} DESTINATION
            ${CMAKE_INSTALL_DATAROOTDIR}/qt5/translations)
endif()


# Packages
set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR ${mastodon-cpp_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${mastodon-cpp_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${mastodon-cpp_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION ${whyblocked_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "whyblocked reminds you why you blocked someone.")
set(CPACK_PACKAGE_CONTACT "tastytea <tastytea@tastytea.de>")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/README.md")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/\\\\.git"
                                      "/.gitignore"
                                      "/build/"
                                      "\\\\.sublime-"
                                      "/.drone.yml")
execute_process(COMMAND uname -m
                OUTPUT_VARIABLE CPACK_PACKAGE_ARCHITECTURE
                OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CPACK_PACKAGE_FILE_NAME
    "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}_${CPACK_PACKAGE_ARCHITECTURE}")
set(CPACK_GENERATOR "TGZ")
set(CPACK_SOURCE_GENERATOR "TGZ")

if (WITH_DEB)
    set(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
    set(CPACK_GENERATOR "DEB")
    set(CPACK_SOURCE_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://schlomp.space/tastytea/whyblocked")
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
    execute_process(COMMAND dpkg --print-architecture
                    OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(CPACK_PACKAGE_FILE_NAME
        "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}-0_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
    set(CPACK_SOURCE_PACKAGE_FILE_NAME
        "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}-0_src")
endif()

if (WITH_RPM)
    set(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
    set(CPACK_GENERATOR "RPM")
    set(CPACK_SOURCE_GENERATOR "RPM")
    set(CPACK_RPM_PACKAGE_LICENSE "GPL-3")
    set(CPACK_RPM_PACKAGE_URL "https://schlomp.space/tastytea/whyblocked")
    if(NOT WITHOUT_QT)
      set(CPACK_RPM_PACKAGE_REQUIRES "vsqlite++ >= 0.3.13, libxdg-basedir >= 1.2.0, qt >= 5.0")
    else()
      set(CPACK_RPM_PACKAGE_REQUIRES "vsqlite++ >= 0.3.13, libxdg-basedir >= 1.2.0")
    endif()
    set(CPACK_PACKAGE_FILE_NAME
         "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-0.${CPACK_PACKAGE_ARCHITECTURE}")
    set(CPACK_SOURCE_PACKAGE_FILE_NAME
        "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-0.src")
endif()

include(CPack)
