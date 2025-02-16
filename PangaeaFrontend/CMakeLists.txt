cmake_minimum_required(VERSION 3.21.1)

set(MY_PRODUCT_NUMBER 1)
set(MY_PRODUCT_VERSION 1)
set(MY_BUILD_NUMBER 2502)

find_package(Qt6 REQUIRED COMPONENTS Core)

if(ANDROID)
    project(Pangaea-mobile VERSION 1.00.00 LANGUAGES CXX)
    set(VERSION_CODE 41)

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
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
      ${CMAKE_CURRENT_SOURCE_DIR}/version.rc
      @ONLY)

    project(PangaeaCPPA VERSION ${MY_PRODUCT_NUMBER}.${MY_PRODUCT_VERSION}.${MY_BUILD_NUMBER}.0 LANGUAGES CXX)

    set(PLATFORM_SPECIFIC_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/Desktop/icons/icon.rc
        ${CMAKE_CURRENT_SOURCE_DIR}/version.rc
    )

    qt6_add_resources(RCC_FILES
        "Desktop/images.qrc"
        "Desktop/translations.qrc"
        "docs.qrc"
        "firmwares.qrc"
    )
endif()

if(APPLE)
    project(PangaeaCPPA VERSION ${MY_PRODUCT_NUMBER}.${MY_PRODUCT_VERSION}.${MY_BUILD_NUMBER} LANGUAGES CXX)

    qt6_add_resources(RCC_FILES
        "Desktop/images.qrc"
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

if(NOT IOS)
    find_package(Qt6 REQUIRED COMPONENTS Qml Bluetooth Gui Network Positioning
        Quick QuickControls2 LinguistTools Svg SerialPort Concurrent)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt6::Core
        Qt6::Bluetooth
        Qt6::SerialPort
        Qt6::Gui
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Svg
        Qt6::Positioning
        Qt6::Qml

        PangaeaBackend
        PangaeaBackendplugin
        converter-lib

        qml_layoutsplugin
        qml_elementsplugin
        qml_modulesplugin
        qml_styleplugin
    )
else()
    find_package(Qt6 REQUIRED COMPONENTS Qml Bluetooth Gui Network Positioning
        Quick QuickControls2 LinguistTools Svg Concurrent)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt6::Core
        Qt6::Bluetooth
        Qt6::Gui
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Svg
        Qt6::Positioning
        Qt6::Qml

        PangaeaBackend
        PangaeaBackendplugin
        converter-lib

        qml_layoutsplugin
        qml_elementsplugin
        qml_modulesplugin
        qml_styleplugin
    )
endif()

qt_policy(SET QTP0001 OLD)
qt_policy(SET QTP0002 OLD)

set(QT_QML_GENERATE_QMLLS_INI ON)
set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/PangaeaFrontend/qml)
set(QML_IMPORT_PATH ${QT_QML_OUTPUT_DIRECTORY} CACHE STRING "Import paths for Qt Creator's code model" FORCE)

if(ANDROID OR IOS)
    add_subdirectory(Mobile)
else()
    add_subdirectory(Desktop)


    if(WIN32)
        set(DEPLOY_PATH ${PROJECT_SOURCE_DIR}/../deploy_windows/)

        message("Build type: " ${CMAKE_BUILD_TYPE})
        message("Deploy path: " ${DEPLOY_PATH})

        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${CMAKE_BINARY_DIR}/PangaeaFrontend/PangaeaCPPA.exe"
                "${DEPLOY_PATH}/release/PangaeaCPPA.exe")

        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${PROJECT_SOURCE_DIR}/docs"
                "${DEPLOY_PATH}/release/docs/")

        configure_file(
          ${DEPLOY_PATH}/template_script.iss
          ${DEPLOY_PATH}/installer_script.iss
          @ONLY)

        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${WINDEPLOYQT_EXECUTABLE} "${DEPLOY_PATH}/release/PangaeaCPPA.exe"
                                    --qmldir "${PROJECT_SOURCE_DIR}/Desktop/qml")

        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ISCC.exe "/O${DEPLOY_PATH}/bin" "${DEPLOY_PATH}/installer_script.iss")
    endif()

    if(APPLE)
        set_target_properties(${PROJECT_NAME}
                PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Desktop/MacOS/Info.plist")

    endif()

endif()
