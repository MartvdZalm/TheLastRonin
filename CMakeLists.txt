cmake_minimum_required(VERSION 3.16)
project(TheLastRonin VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES
    translations/app_nl.ts
    translations/app_jp.ts
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/translations)

qt_add_translations(TheLastRonin
    TS_FILES ${TS_FILES}
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src
    LUPDATE_OPTIONS -no-obsolete
    LRELEASE_OPTIONS -compress
    RESOURCE_PREFIX "/translations"
)

add_subdirectory(src)
