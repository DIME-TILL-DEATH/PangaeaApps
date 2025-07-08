cmake_minimum_required(VERSION 3.21.1)

set(MY_PRODUCT_NUMBER 1)
set(MY_PRODUCT_VERSION 1)
set(MY_BUILD_NUMBER 2507)

find_package(Qt6 REQUIRED COMPONENTS Core)

if(ANDROID)
    project(Pangaea-mobile VERSION 1.1.1 LANGUAGES CXX)
    set(VERSION_CODE 44)

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
elseif(IOS)
    project(Pangaea-mobile VERSION 1.1.1 LANGUAGES CXX)

    set(PLATFORM_SPECIFIC_SOURCES
        Mobile/ios/src/iosfileutils.hpp
        Mobile/ios/src/iosfileutils.mm)

    qt6_add_resources(RCC_FILES
        "Mobile/qml.qrc"
        "Mobile/translations.qrc"
        "docs.qrc"
        "firmwares.qrc"
    )
elseif(WIN32)
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
      ${CMAKE_CURRENT_SOURCE_DIR}/version.rc
      @ONLY)

    project(PangaeaCPPA VERSION ${MY_PRODUCT_NUMBER}.${MY_PRODUCT_VERSION}.${MY_BUILD_NUMBER}.1 LANGUAGES CXX)

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
elseif(APPLE) # MacOS
    project(PangaeaCPPA VERSION ${MY_PRODUCT_NUMBER}.${MY_PRODUCT_VERSION}.${MY_BUILD_NUMBER} LANGUAGES CXX)

    qt6_add_resources(RCC_FILES
        "Desktop/images.qrc"
        "Desktop/translations.qrc"
        "docs.qrc"
        "firmwares.qrc"
    )

   set(PLATFORM_SPECIFIC_SOURCES
       "${PROJECT_SOURCE_DIR}/Desktop/icons/pangaea.icns")

   set_source_files_properties(${PLATFORM_SPECIFIC_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
elseif(LINUX)
    project(PangaeaCPPA VERSION ${MY_PRODUCT_NUMBER}.${MY_PRODUCT_VERSION}.${MY_BUILD_NUMBER} LANGUAGES CXX)

    qt6_add_resources(RCC_FILES
        "Desktop/images.qrc"
        "Desktop/translations.qrc"
        "docs.qrc"
        "firmwares.qrc"
    )

    add_definitions(-DVERSION_STRING="${MY_PRODUCT_NUMBER}.${MY_PRODUCT_VERSION}.${MY_BUILD_NUMBER}")
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


find_package(Qt6 REQUIRED COMPONENTS Qml Gui Network Positioning
    Quick LinguistTools Svg Concurrent)
target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::Svg
    Qt6::Qml

    PangaeaBackend
    PangaeaBackendplugin
    converter-lib

    # qml_layoutsplugin
    # qml_elementsplugin
    # qml_modulesplugin
    # qml_styleplugin
)


qt_policy(SET QTP0001 OLD)
qt_policy(SET QTP0002 OLD)

set(QT_QML_GENERATE_QMLLS_INI ON)
set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/PangaeaFrontend/qml)
set(QML_IMPORT_PATH ${QT_QML_OUTPUT_DIRECTORY} CACHE STRING "Import paths for Qt Creator's code model" FORCE)

if(ANDROID OR IOS)
    add_subdirectory(Mobile)

    if(IOS)
        set_target_properties(${PROJECT_NAME} PROPERTIES
        #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appTableControl
            MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Mobile/ios/Info.plist"
            QT_IOS_LAUNCH_SCREEN "${CMAKE_CURRENT_SOURCE_DIR}/Mobile/ios/LaunchScreen.storyboard"
            XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME AppIcon
            MACOSX_BUNDLE TRUE
            WIN32_EXECUTABLE TRUE
        )

        # set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Mobile/ios/Info.plist")
        # set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME AppIcon)

        set(asset_catalog_path "Mobile/ios/Assets.xcassets")

        target_sources(${PROJECT_NAME} PRIVATE "${asset_catalog_path}")
        set_source_files_properties(
            ${asset_catalog_path}
            PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

        qt6_add_resources(${PROJECT_NAME} "IosResources"
            PREFIX
                "/"
            FILES

                Mobile/ios/Assets.xcassets
        )
    endif()

else()
    add_subdirectory(Desktop)

    if(WIN32)
        if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
            set(DEPLOY_PATH ${PROJECT_SOURCE_DIR}/../deploy_windows/)

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
    endif()

    if(APPLE)
        set_target_properties(${PROJECT_NAME} PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Desktop/MacOS/Info.plist"
            MACOSX_BUNDLE_BUNDLE_VERSION "${MY_PRODUCT_NUMBER}.${MY_PRODUCT_VERSION}.${MY_BUILD_NUMBER}")

        if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
            set(RELEASE_APP_PATH ${CMAKE_BINARY_DIR}/PangaeaFrontend/PangaeaCPPA.app)

            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${PROJECT_SOURCE_DIR}/docs"
                    "${RELEASE_APP_PATH}/Contents/MacOS/docs")

            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    "${CMAKE_BINARY_DIR}/WavConverterShell/IrConverter.app/Contents/MacOS/IrConverter"
                    "${RELEASE_APP_PATH}/Contents/MacOS/")

            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${MACDEPLOYQT_EXECUTABLE} "${RELEASE_APP_PATH}"
                                        "-qmldir=${PROJECT_SOURCE_DIR}/Desktop/qml"
                                        "-libpath=${CMAKE_BINARY_DIR}/../WavConverterLib/")

            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${RELEASE_APP_PATH}"
                    "${PROJECT_SOURCE_DIR}/../deploy_mac/dmg_content/PangaeaCPPA.app")

            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND cd "${PROJECT_SOURCE_DIR}/../deploy_mac"
                    COMMAND sh "package_script.sh")
        endif()
    endif()

    if(LINUX)
        set(DEPLOY_PATH ${PROJECT_SOURCE_DIR}/../deploy_linux/)

        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_BINARY_DIR}/PangaeaFrontend/PangaeaCPPA"
            "${DEPLOY_PATH}/app/bin/PangaeaCPPA")

        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PROJECT_SOURCE_DIR}/docs"
            "${DEPLOY_PATH}/app/docs/")

        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${DEPLOY_PATH}/libxcb-cursor.so.0"
            "${DEPLOY_PATH}/app/lib/libxcb-cursor.so.0")


        # using cqtdeployer, installed from snap
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND cqtdeployer -bin "${DEPLOY_PATH}/app/bin/PangaeaCPPA"
                                -targetDir "${DEPLOY_PATH}/app/"
                                -libDir "${PROJECT_SOURCE_DIR}/sox_lib/lib.linux"
                                -qmlDir "${PROJECT_SOURCE_DIR}/Desktop/qml"
                                -qmake "~/Qt/6.8.1/gcc_64/bin/qmake")

        # make tarball archive
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND tar -czvf "${DEPLOY_PATH}/PangaeaCPPA.tar.gz" "${DEPLOY_PATH}/app/")

        # make .deb package
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${DEPLOY_PATH}/app/"
            "${DEPLOY_PATH}/debian_deploy/PangaeaCPPA/usr/local/PangaeaCPPA/")

        configure_file(
            ${DEPLOY_PATH}/DEBIAN/control
            ${DEPLOY_PATH}/debian_deploy/PangaeaCPPA/DEBIAN/control
            @ONLY)

        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${DEPLOY_PATH}/usr/"
            "${DEPLOY_PATH}/debian_deploy/PangaeaCPPA/usr/")

        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND dpkg-deb --build "${DEPLOY_PATH}/debian_deploy/PangaeaCPPA/")
    endif()

endif()
