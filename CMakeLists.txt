cmake_minimum_required(VERSION 3.16)
project(TheLastRonin VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES
    translations/app_nl.ts
    translations/app_jp.ts
)

# Create translations directory if it doesn't exist
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/translations)

# Use Qt's built-in translation functions
if(QT_VERSION_MAJOR EQUAL 6)
    # Qt6 way - uses qt_add_translations
    qt_add_translations(TheLastRonin
        TS_FILES ${TS_FILES}
        SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src
        LUPDATE_OPTIONS -no-obsolete
        LRELEASE_OPTIONS -compress
        RESOURCE_PREFIX "/translations"
    )
else()
    # Qt5 way - uses qt5_add_translation
    qt5_add_translation(QM_FILES ${TS_FILES})

    # Custom target to update .ts files for Qt5
    find_program(QT_LUPDATE_EXECUTABLE lupdate HINTS ${Qt5_DIR}/../../../bin)
    if(QT_LUPDATE_EXECUTABLE)
        add_custom_target(update_translations
            COMMAND ${QT_LUPDATE_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/src -ts ${TS_FILES}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Updating translation files"
            VERBATIM
        )
    endif()

    # Ensure translations are built
    add_custom_target(translations ALL DEPENDS ${QM_FILES})

    # For Qt5, create a resource file for translations
    set(TRANSLATIONS_QRC "${CMAKE_CURRENT_BINARY_DIR}/translations.qrc")
    file(WRITE ${TRANSLATIONS_QRC} "<!DOCTYPE RCC>\n<RCC version=\"1.0\">\n<qresource prefix=\"/translations\">\n")
    foreach(qm_file ${QM_FILES})
        get_filename_component(qm_filename ${qm_file} NAME)
        file(APPEND ${TRANSLATIONS_QRC} "    <file>${qm_filename}</file>\n")
    endforeach()
    file(APPEND ${TRANSLATIONS_QRC} "</qresource>\n</RCC>\n")

    # Add the resource file to your target
    target_sources(TheLastRonin PRIVATE ${TRANSLATIONS_QRC})
endif()

add_subdirectory(src)

include(GNUInstallDirs)

install(TARGETS TheLastRonin
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    # Qt6 handles .qm file installation automatically when using qt_add_translations
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/translations
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        FILES_MATCHING PATTERN "*.qm"
    )
else()
    # Qt5 - install manually generated .qm files
    install(FILES ${QM_FILES}
        DESTINATION ${CMAKE_INSTALL_BINDIR}/translations
    )
endif()




# # Set the path to your Qt Installer Framework's binarycreator
# if(WIN32)
#     set(QTIFW_BINARYCREATOR "C:/Qt/QtIFW-4.8.1/bin/binarycreator.exe")
# elseif(APPLE)
#     set(QTIFW_BINARYCREATOR "/Users/youruser/Qt/QtIFW-4.8.1/bin/binarycreator")
# endif()

# # Set installer directories
# set(INSTALLER_DIR "${CMAKE_SOURCE_DIR}/installer")
# set(PACKAGE_DATA_DIR "${INSTALLER_DIR}/packages/com.thelastronin/data")

# # Custom target: copy built files to installer data directory
# add_custom_target(copy_to_installer ALL
#     COMMAND ${CMAKE_COMMAND} -E make_directory ${PACKAGE_DATA_DIR}
#     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:TheLastRonin> ${PACKAGE_DATA_DIR}
#     # Add more copy commands for dependencies/resources if needed
#     DEPENDS TheLastRonin
#     COMMENT "Copying built files to installer data directory"
# )

# # Custom target: run binarycreator to build the installer
# add_custom_target(build_installer ALL
#     COMMAND "${QTIFW_BINARYCREATOR}" --offline-only --config "${INSTALLER_DIR}/config/config.xml" --packages "${INSTALLER_DIR}/packages" "${CMAKE_BINARY_DIR}/TheLastRoninInstaller.exe"
#     DEPENDS copy_to_installer
#     COMMENT "Building installer with Qt Installer Framework"
# )


# Local deployment targets (optional, for testing)
# if(WIN32)
#     add_custom_target(deploy_local
#         COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/deploy
#         COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:TheLastRonin> ${CMAKE_BINARY_DIR}/deploy/
#         COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR}/deploy
#                 ${Qt${QT_VERSION_MAJOR}_DIR}/../../../bin/windeployqt.exe --release $<TARGET_FILE_NAME:TheLastRonin>
#         DEPENDS TheLastRonin
#         COMMENT "Deploying application locally for testing"
#     )
# elseif(APPLE)
#     add_custom_target(deploy_local
#         COMMAND ${Qt${QT_VERSION_MAJOR}_DIR}/../../../bin/macdeployqt $<TARGET_BUNDLE_DIR:TheLastRonin>
#         DEPENDS TheLastRonin
#         COMMENT "Deploying application locally for testing"
#     )
# endif()