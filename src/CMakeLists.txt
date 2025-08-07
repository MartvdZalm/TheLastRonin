
set(PROJECT_SOURCES
    main.cpp



)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TheLastRonin
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resources.qrc

        # Database Layer
        database/Container.h
        database/Container.cpp
        database/DatabaseManager.h
        database/DatabaseManager.cpp

        # Models
        model/Model.h
        model/Playlist.h
        model/Setting.h
        model/Track.h

        # Repositories
        repository/IPlaylistRepository.h
        repository/ISettingRepository.h
        repository/ITrackRepository.h
        repository/PlaylistRepository.h
        repository/PlaylistRepository.cpp
        repository/SettingRepository.h
        repository/SettingRepository.cpp
        repository/TrackRepository.h
        repository/TrackRepository.cpp

        # Events
        events/AppEvents.h
        events/AppEvents.cpp

        # Services
        service/LanguageService.h
        service/LanguageService.cpp
        service/AudioMetadataService.h
        service/AudioMetadataService.cpp
        service/TrackService.h
        service/TrackService.cpp

        # Styles
        styles/AppStyle.h
        styles/AppStyle.cpp
        styles/ButtonStyle.h
        styles/ButtonStyle.cpp
        styles/ComboBoxStyle.h
        styles/ComboBoxStyle.cpp
        styles/InputStyle.h
        styles/InputStyle.cpp
        styles/ListStyle.h
        styles/ListStyle.cpp
        styles/SliderStyle.h
        styles/SliderStyle.cpp

        # Shared Components
        components/shared/CoverImageWidget.h
        components/shared/CoverImageWidget.cpp
        components/shared/MiniPlayerWindow.h
        components/shared/MiniPlayerWindow.cpp
        components/shared/NavigationBar.h
        components/shared/NavigationBar.cpp
        components/shared/PlaybackBar.h
        components/shared/PlaybackBar.cpp

        # Dialog Components
        components/dialog/AddPlaylistDialog.h
        components/dialog/AddPlaylistDialog.cpp
        components/dialog/AddTrackDialog.h
        components/dialog/AddTrackDialog.cpp

        # Playlist Components
        components/home/PlaylistCard.h
        components/home/PlaylistCard.cpp
        components/home/PlaylistGrid.h
        components/home/PlaylistGrid.cpp
        components/playlist/PlaylistDetails.h
        components/playlist/PlaylistDetails.cpp
        components/playlist/TrackList.h
        components/playlist/TrackList.cpp

        # Windows
        window/BaseWindow.h
        window/BaseWindow.cpp
        window/HomeWindow.h
        window/HomeWindow.cpp
        window/MainWindow.h
        window/MainWindow.cpp
        window/PlaylistWindow.h
        window/PlaylistWindow.cpp
        window/SettingsWindow.h
        window/SettingsWindow.cpp
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
