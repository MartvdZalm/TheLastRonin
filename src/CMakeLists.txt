
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TheLastRonin
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        dialog/AddAlbumDialog.h
        dialog/AddAlbumDialog.cpp
        database/DatabaseManager.h
        database/DatabaseManager.cpp
        album/AlbumManager.h
        album/AlbumManager.cpp
        album/Album.h
        album/AlbumCard.h
        album/AlbumCard.cpp
        page/AlbumPage.h

        page/AlbumPage.cpp
    )
else()
    add_executable(TheLastRonin
        ${PROJECT_SOURCES}
    )
endif()

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

target_link_libraries(TheLastRonin PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

set_target_properties(TheLastRonin PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TheLastRonin)
endif()
