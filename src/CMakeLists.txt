
set(PROJECT_SOURCES
    main.cpp



)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TheLastRonin
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        database/DatabaseManager.h
        database/DatabaseManager.cpp
        components/dialog/AddPlaylistDialog.h
        components/dialog/AddPlaylistDialog.cpp
        components/home/PlaylistCard.h
        components/home/PlaylistCard.cpp
        window/PlaylistWindow.h
        window/PlaylistWindow.cpp
        styles/ButtonStyle.h
        styles/ButtonStyle.cpp
        styles/InputStyle.h
        styles/InputStyle.cpp
        components/shared/CoverImageWidget.h
        components/shared/CoverImageWidget.cpp
        components/playlist/PlaylistDetails.h
        components/playlist/PlaylistDetails.cpp
        components/playlist/TrackList.h
        components/playlist/TrackList.cpp
        components/dialog/AddTrackDialog.h
        components/dialog/AddTrackDialog.cpp
        resources.qrc
        components/home/PlaylistGrid.h
        components/home/PlaylistGrid.cpp
        window/BaseWindow.h
        window/BaseWindow.cpp
        window/HomeWindow.h
        window/HomeWindow.cpp
        dao/PlaylistDAO.h
        dao/PlaylistDAO.cpp
        service/PlaylistService.h
        service/PlaylistService.cpp
        model/Playlist.h
        model/Track.h
        dao/TrackDAO.h
        dao/TrackDAO.cpp
        service/TrackService.h
        service/TrackService.cpp
        styles/ComboBoxStyle.h
        styles/ComboBoxStyle.cpp
        events/AppEvents.h
        events/AppEvents.cpp


        dao/UserDAO.h
        model/User.h
        dao/UserDAO.cpp
        window/MainWindow.h
        window/MainWindow.cpp
        components/shared/NavigationBar.h
        components/shared/NavigationBar.cpp
        components/shared/MiniPlayerWindow.h
        components/shared/MiniPlayerWindow.cpp
        components/shared/PlaybackBar.h
        components/shared/PlaybackBar.cpp
        styles/AppStyle.h
        styles/AppStyle.cpp
        styles/SliderStyle.h
        styles/SliderStyle.cpp


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
