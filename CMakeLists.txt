cmake_minimum_required(VERSION 3.5)

set(ProjectName PCECDI)
project(${ProjectName} VERSION 9.5 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_COMPILER ${CMAKE_CXX_COMPILER})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PCECDI_TS_Files
    Translations/${ProjectName}_fr.ts
    Translations/${ProjectName}_ru.ts
    Translations/${ProjectName}_de.ts
)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Xml LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Xml LinguistTools)

set(PROJECT_SOURCES
    main.cpp
    MainUI.h
    SettingsManager.h
    SettingsUI.h
    CheckSums.h
    Ressources/Ressources.qrc
)

set(RESOURCE_FILES
    Ressources/version.rc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    if (WIN32 OR DEFINED ENV{MXE})
        qt_add_executable(${ProjectName}
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            ${RESOURCE_FILES}
        )
    else()
        qt_add_executable(${ProjectName}
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
        )
    endif()
else()
    add_executable(${ProjectName}
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(${ProjectName} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Xml)

set(BUNDLE_ID_OPTION)
set_target_properties(${ProjectName} PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE FALSE
    WIN32_EXECUTABLE TRUE
)

set_source_files_properties(
    ${PCECDI_TS_Files}
    PROPERTIES OUTPUT_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/translations"
)

qt_add_translations(${ProjectName} TS_FILES ${PCECDI_TS_Files})

include(GNUInstallDirs)
install(TARGETS ${ProjectName}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${ProjectName})
endif()
