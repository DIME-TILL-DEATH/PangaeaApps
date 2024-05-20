cmake_minimum_required(VERSION 3.21.1)
project(Pangaea-mobile VERSION 0.10.2 LANGUAGES CXX)

message(VERBOSE "Architecture: ${ANDROID_TARGET_ARCH}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

qt6_add_resources(SOURCES
        "qml.qrc"
        "docs.qrc"
        "firmwares.qrc"
        "translations.qrc"
)

qt_add_executable(Pangaea-mobile WIN32 MACOSX_BUNDLE
    main.cpp
    uicore.cpp uicore.h

    android/src/activityresultmanager.cpp android/src/activityresultmanager.h
    android/src/androidutils.cpp android/src/androidutils.h

    ../pangaea-backend/threadcontroller.cpp ../pangaea-backend/threadcontroller.h
    ../pangaea-backend/uihelpers/uiinterfacemanager.cpp ../pangaea-backend/uihelpers/uiinterfacemanager.h
    ../pangaea-backend/utils/logger.cpp ../pangaea-backend/utils/logger.h

    ${SOURCES}
)

target_include_directories(Pangaea-mobile PUBLIC
    android/src/

    ../pangaea-backend/
    ../pangaea-backend/cores
    ../pangaea-backend/device
    ../pangaea-backend/interfaces/
    ../pangaea-backend/utils
    ../pangaea-backend/qmlEnums
    ../pangaea-backend/models/
    ../pangaea-backend/uihelpers/

    ../WavConverterLib/
)

find_package(Qt6 REQUIRED COMPONENTS Bluetooth Gui Network Positioning Quick QuickControls2 LinguistTools Svg SerialPort)
target_link_libraries(Pangaea-mobile PUBLIC
    Qt6::Core
    Qt6::Bluetooth
    Qt6::SerialPort
    Qt6::Gui
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Svg

    pangaea-backend
    converter-lib
)

if(ANDROID)
    target_link_libraries(Pangaea-mobile PUBLIC
        Qt::CorePrivate)

    set_target_properties(Pangaea-mobile PROPERTIES
        QT_ANDROID_VERSION_NAME ${PROJECT_VERSION}
        QT_ANDROID_VERSION_CODE 1
        QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
    )

endif()

set_property(TARGET Pangaea-mobile PROPERTY QT_ANDROID_EXTRA_LIBS
    ${CMAKE_CURRENT_SOURCE_DIR}/android_ssl_libs/${CMAKE_ANDROID_ARCH_ABI}/libcrypto_1_1.so
    ${CMAKE_CURRENT_SOURCE_DIR}/android_ssl_libs/${CMAKE_ANDROID_ARCH_ABI}/libssl_1_1.so
    ${CMAKE_CURRENT_SOURCE_DIR}/android_openmp_libs/${CMAKE_ANDROID_ARCH_ABI}/libomp.so #needed for android sox lib

    ${CMAKE_CURRENT_SOURCE_DIR}/../sox_lib/lib.android/${CMAKE_ANDROID_ARCH_ABI}/libsox.so
)

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

# APK на этот момент ещё не создан
#add_custom_command(TARGET Pangaea-mobile POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/android-build/build/outputs/apk/debug/android-build-debug.apk
#                                     ${CMAKE_CURRENT_SOURCE_DIR}/../deploy_android/Pangaea-mobile_v${CMAKE_PROJECT_VERSION}.apk
#)
