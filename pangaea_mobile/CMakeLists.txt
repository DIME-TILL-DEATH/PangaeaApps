cmake_minimum_required(VERSION 3.21.1)
project(Pangaea-mobile VERSION 0.9.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_definitions(-DCP16_FIRMWARE_VERSION="1.04.00")
add_definitions(-DCP16PA_FIRMWARE_VERSION="1.04.00")
add_definitions(-DCP100_FIRMWARE_VERSION="2.08.02")
add_definitions(-DCP100PA_FIRMWARE_VERSION="6.08.04")

find_package(Qt6 REQUIRED COMPONENTS Core)

qt6_add_resources(SOURCES
        "qml.qrc"
        "docs.qrc"
        "firmwares.qrc"
        "translations.qrc"
)

qt_add_executable(Pangaea-mobile WIN32 MACOSX_BUNDLE
    bluetooth/bluetoothleuart.cpp bluetooth/bluetoothleuart.h
    cores/core.cpp cores/core.h
    cores/netcore.cpp cores/netcore.h
    cores/uicore.cpp cores/uicore.h
    device/answerworker.cpp device/answerworker.h
    device/defines.h
    device/device.cpp device/device.h
    device/deviceanswer.cpp device/deviceanswer.h
    device/devicecontrols.cpp device/devicecontrols.h
    device/deviceparameter.cpp device/deviceparameter.h
    device/firmware.cpp device/firmware.h
    device/parser.cpp device/parser.h
    device/preset.cpp device/preset.h
    device/presetlistmodel.cpp device/presetlistmodel.h
    main.cpp
    threadcontroller.cpp threadcontroller.h
    utils/activityresultmanager.cpp utils/activityresultmanager.h
    utils/androidutils.cpp utils/androidutils.h
    utils/irworker.cpp utils/irworker.h
    utils/logger.cpp utils/logger.h
    utils/presetmanager.cpp utils/presetmanager.h

    ${SOURCES}
)

target_include_directories(Pangaea-mobile PUBLIC
    bluetooth
    cores
    device
    utils
)

find_package(Qt6 REQUIRED COMPONENTS Bluetooth Gui Network Positioning Quick QuickControls2 LinguistTools)
target_link_libraries(Pangaea-mobile PUBLIC
    Qt6::Core
    Qt6::Bluetooth
    Qt6::Gui
    Qt6::Network
    Qt6::Positioning
    Qt6::Quick
    Qt6::QuickControls2
)

if(ANDROID)
    target_link_libraries(Pangaea-mobile PUBLIC Qt::CorePrivate)

    set_target_properties(Pangaea-mobile PROPERTIES
        QT_ANDROID_VERSION_NAME ${PROJECT_VERSION}
        QT_ANDROID_VERSION_CODE 1
        QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
    )
endif()

if(ANDROID_TARGET_ARCH STREQUAL "arm64-v8a")
        set_property(TARGET Pangaea-mobile PROPERTY QT_ANDROID_EXTRA_LIBS
            ${CMAKE_CURRENT_SOURCE_DIR}/android_ssl_libs/arm64-v8a/libcrypto_1_1.so
            ${CMAKE_CURRENT_SOURCE_DIR}/android_ssl_libs/arm64-v8a/libssl_1_1.so)
elseif(ANDROID_TARGET_ARCH STREQUAL "armeabi-v7a")
    set_property(TARGET Pangaea-mobile PROPERTY QT_ANDROID_EXTRA_LIBS
        ${CMAKE_CURRENT_SOURCE_DIR}/android_ssl_libs/armeabi-v7a/libcrypto_1_1.so
        ${CMAKE_CURRENT_SOURCE_DIR}/android_ssl_libs/armeabi-v7a/libssl_1_1.so)
endif()

qt6_add_translations(Pangaea-mobile TS_FILES
    translations/pangaea-mobile_de.ts
    translations/pangaea-mobile_en.ts
    translations/pangaea-mobile_it.ts
    translations/pangaea-mobile_ru.ts )

qt6_add_resources(Pangaea-mobile "AndroidResources"
    PREFIX
        "/"
    FILES
        android/src/JavaFile.java

        android/AndroidManifest.xml
        android/build.gradle
)

#for(abi, ANDROID_ABIS): ANDROID_EXTRA_LIBS += $$PWD/android_ssl_libs/$${abi}/libcrypto_1_1.so \
#                                      $$PWD/android_ssl_libs/$${abi}/libssl_1_1.so

