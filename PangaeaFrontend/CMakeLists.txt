cmake_minimum_required(VERSION 3.21.1)

find_package(Qt6 REQUIRED COMPONENTS Core)

if(ANDROID)
    project(Pangaea-mobile VERSION 0.12.8 LANGUAGES CXX)
    set(VERSION_CODE 36)

    set(PLATFORM_SPECIFIC_SOURCES
        Mobile/android/src/activityresultmanager.cpp Mobile/android/src/activityresultmanager.h
        Mobile/android/src/androidutils.cpp Mobile/android/src/androidutils.h
    )

    qt6_add_resources(RCC_FILES
        "Mobile/qml.qrc"
        "Mobile/translations.qrc"
        "docs.qrc"
        "firmwares.qrc"
    )

endif()

if(WIN32)
    project(PangaeaCPPA VERSION 1.0.2000.1 LANGUAGES CXX)

    qt6_add_resources(RCC_FILES
        "Desktop/qml.qrc"
        "Desktop/translations.qrc"
        "docs.qrc"
        "firmwares.qrc"
    )
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


qt_add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE
    main.cpp
    uicore.cpp uicore.h
    uisettings.cpp uisettings.h

    ../PangaeaBackend/threadcontroller.cpp ../PangaeaBackend/threadcontroller.h
    ../PangaeaBackend/uihelpers/uiinterfacemanager.cpp ../PangaeaBackend/uihelpers/uiinterfacemanager.h
    ../PangaeaBackend/utils/logger.cpp ../PangaeaBackend/utils/logger.h

    ${PLATFORM_SPECIFIC_SOURCES}
    ${RCC_FILES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ../PangaeaBackend/
    ../PangaeaBackend/cores
    ../PangaeaBackend/device
    ../PangaeaBackend/interfaces/
    ../PangaeaBackend/utils
    ../PangaeaBackend/qmlEnums
    ../PangaeaBackend/models/
    ../PangaeaBackend/uihelpers/

    ../WavConverterLib/
)

find_package(Qt6 REQUIRED COMPONENTS Bluetooth Gui Network Positioning Quick QuickControls2 LinguistTools Svg SerialPort Widgets Concurrent)
target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt6::Core
    Qt6::Bluetooth
    Qt6::SerialPort
    Qt6::Gui
    Qt6::Widgets
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Svg
    Qt6::Positioning

    PangaeaBackend
    PangaeaBackendplugin
    converter-lib
)

qt_policy(SET QTP0002 OLD)

set(QT_QML_GENERATE_QMLLS_INI ON)
set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/PangaeaFrontend/qml)
set(QML_IMPORT_PATH ${QT_QML_OUTPUT_DIRECTORY} CACHE STRING "Import paths for Qt Creator's code model" FORCE)

if(ANDROID OR IOS)
    add_subdirectory(Mobile)
else()
    add_subdirectory(qml/ControlGroups/)
    add_subdirectory(qml/CustomOverlays/)
    add_subdirectory(qml/Elements/)
    add_subdirectory(qml/Modules/)
    add_subdirectory(qml/Pages/)
    add_subdirectory(qml/StyleSettings/)
    add_subdirectory(qml/Tutorials/)
endif()

