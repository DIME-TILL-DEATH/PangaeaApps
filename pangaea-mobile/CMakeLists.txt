cmake_minimum_required(VERSION 3.21.1)

if(ANDROID)
    project(Pangaea-mobile VERSION 0.12.8 LANGUAGES CXX)
    set(VERSION_CODE 36)

    set(PLATFORM_SPECIFIC_SOURCES
        android/src/activityresultmanager.cpp android/src/activityresultmanager.h
        android/src/androidutils.cpp android/src/androidutils.h
    )
endif()

if(WIN32)
    project(PangaeaCPPA VERSION 1.0.2000.1 LANGUAGES CXX)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

qt6_add_resources(RCC_FILES
    "qml.qrc"
    "docs.qrc"
    "firmwares.qrc"
    "translations.qrc"
)

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

find_package(Qt6 REQUIRED COMPONENTS Bluetooth Gui Network Positioning Quick QuickControls2 LinguistTools Svg SerialPort Widgets)
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
set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/pangaea-mobile/qml)
set(QML_IMPORT_PATH ${QT_QML_OUTPUT_DIRECTORY} CACHE STRING "Import paths for Qt Creator's code model" FORCE)


#############################################--MOBILE--#####################################################
if(ANDROID)
    target_include_directories(${PROJECT_NAME} PUBLIC
        android/src/
    )

    target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt::CorePrivate)

    set_target_properties(${PROJECT_NAME} PROPERTIES
        QT_ANDROID_VERSION_NAME ${PROJECT_VERSION}
        QT_ANDROID_VERSION_CODE ${VERSION_CODE}
        QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
    )
    set_property(TARGET Pangaea-mobile PROPERTY QT_ANDROID_EXTRA_LIBS
       ${CMAKE_CURRENT_SOURCE_DIR}/android_ssl_libs/${CMAKE_ANDROID_ARCH_ABI}/libcrypto_1_1.so
       ${CMAKE_CURRENT_SOURCE_DIR}/android_ssl_libs/${CMAKE_ANDROID_ARCH_ABI}/libssl_1_1.so
       ${CMAKE_CURRENT_SOURCE_DIR}/android_openmp_libs/${CMAKE_ANDROID_ARCH_ABI}/libomp.so #needed for android sox lib

       ${CMAKE_CURRENT_SOURCE_DIR}/../sox_lib/lib.android/${CMAKE_ANDROID_ARCH_ABI}/libsox.so
    )

    qt6_add_resources(Pangaea-mobile "AndroidResources"
        PREFIX
            "/"
        FILES
            android/src/JavaFile.java

            android/AndroidManifest.xml
            android/build.gradle
    )
endif()

if(ANDROID OR IOS)
    add_subdirectory(qml/ControlGroups/)
    add_subdirectory(qml/CustomOverlays/)
    add_subdirectory(qml/Elements/)
    add_subdirectory(qml/Modules/)
    add_subdirectory(qml/Pages/)
    add_subdirectory(qml/StyleSettings/)
    add_subdirectory(qml/Tutorials/)
else()
    add_subdirectory(qml/ControlGroups/)
    add_subdirectory(qml/CustomOverlays/)
    add_subdirectory(qml/Elements/)
    add_subdirectory(qml/Modules/)
    add_subdirectory(qml/Pages/)
    add_subdirectory(qml/StyleSettings/)
    add_subdirectory(qml/Tutorials/)
endif()

