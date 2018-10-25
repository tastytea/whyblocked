set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM 1)
find_package(Qt5LinguistTools REQUIRED)

file(GLOB TS_FILES "${PROJECT_SOURCE_DIR}/translations/*.ts")
file(GLOB TS_SRC_FILES "${PROJECT_SOURCE_DIR}/src/*")
qt5_create_translation(QM_FILES ${TS_SRC_FILES} ${TS_FILES}
                       OPTIONS -no-obsolete)
# qt5_add_translation(QM_FILES ${TS_FILES})
add_custom_target(translations ALL DEPENDS ${QM_FILES})
add_dependencies(${CMAKE_PROJECT_NAME} translations)
add_dependencies(${CMAKE_PROJECT_NAME}-gui translations)
install(FILES ${QM_FILES} DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/qt5/translations)
