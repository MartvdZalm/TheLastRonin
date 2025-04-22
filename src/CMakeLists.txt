
set(PROJECT_SOURCES
    main.cpp



)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TheLastRonin
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        database/DatabaseManager.h
        database/DatabaseManager.cpp
        playlist/Playlist.h
        playlist/Track.h
        playlist/PlaylistManager.h
        playlist/PlaylistManager.cpp
        dialog/PlaylistDialog.h
        dialog/PlaylistDialog.cpp
        playlist/PlaylistCard.h
        playlist/PlaylistCard.cpp
        window/PlaylistWindow.h
        window/PlaylistWindow.cpp
        styles/ButtonStyle.h
        styles/ButtonStyle.cpp
        styles/InputStyle.h
        styles/InputStyle.cpp
        components/CoverImageWidget.h
        components/CoverImageWidget.cpp
        playlist/PlaylistDetailsWidget.h
        playlist/PlaylistDetailsWidget.cpp
        playlist/TrackListWidget.h
        playlist/TrackListWidget.cpp
        dialog/AddTrackDialog.h
        dialog/AddTrackDialog.cpp
        resources.qrc
        repository/PlaylistRepository.h
        repository/PlaylistRepository.cpp
        playlist/PlaylistGridController.h
        playlist/PlaylistGridController.cpp
        styles/AppStyle.h
        styles/AppStyle.cpp
        window/BaseWindow.h
        window/BaseWindow.cpp
        window/HomeWindow.h
        window/HomeWindow.cpp




    )
else()
    add_executable(TheLastRonin
        ${PROJECT_SOURCES}
    )
endif()

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql Multimedia)

target_link_libraries(TheLastRonin PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Multimedia
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
