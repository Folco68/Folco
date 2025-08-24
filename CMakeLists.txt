cmake_minimum_required(VERSION 3.16)

project(Folco VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)

set(PROJECT_SOURCES
        main.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Folco
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        # Main
        BeforeRelease.hpp
        Global.hpp
        Logger.hpp
        Logger.cpp
        Settings.hpp
        Settings.cpp

        # UI
        UI/Dialog.hpp
        UI/Dialog.cpp
        UI/DlgHelp.hpp
        UI/DlgHelp.cpp
        UI/DlgHelp.ui
        UI/DlgConfiguration.hpp
        UI/DlgConfiguration.cpp
        UI/DlgConfiguration.ui
        UI/DlgMergeConfigurations.hpp
        UI/DlgMergeConfigurations.cpp
        UI/DlgMergeConfigurations.ui
        UI/DlgPredefinedIP.hpp
        UI/DlgPredefinedIP.cpp
        UI/DlgPredefinedIP.ui
        UI/DlgSettings.hpp
        UI/DlgSettings.cpp
        UI/DlgSettings.ui
        UI/IPlineEdit.hpp
        UI/IPlineEdit.cpp
        UI/IPvalidator.hpp
        UI/IPvalidator.cpp
        UI/TrayIcon.hpp
        UI/TrayIcon.cpp

        # Network
        Network/Configuration.hpp
        Network/Configuration.cpp
        Network/ConfigurationList.hpp
        Network/ConfigurationList.cpp
        Network/PredefinedIP.hpp
        Network/PredefinedIP.cpp

        # Resources
        Resources/Icons.qrc
        Resources/Icons.rc
        Resources/IconBase.png
        Resources/IconBase.ico
#        Resources/Folco.exe.manifest
#        Resources/Folco.rc

        #Docs
        Docs/Docs.qrc
        Docs/Format
        Docs/LICENSE
        Docs/TODO
        Docs/Changelog
        Docs/README
)
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Folco APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Folco SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Folco
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Folco PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Folco PRIVATE Qt${QT_VERSION_MAJOR}::Network)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Folco)
endif()
set_target_properties(Folco PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
#set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/MANIFESTUAC:level=requireAdministrator")
#set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/MANIFESTUAC:level=requireAdministrator")

include(GNUInstallDirs)
install(TARGETS Folco
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Folco)
endif()
