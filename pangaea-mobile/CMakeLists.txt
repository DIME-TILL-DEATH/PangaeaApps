cmake_minimum_required(VERSION 3.21.1)
project(Pangaea-mobile VERSION 0.12.7 LANGUAGES CXX)
set(VERSION_CODE 35)

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
    Qt6::Positioning

    pangaea-backend
    converter-lib
)

# qt_policy(SET QTP0001 OLD)
qt_policy(SET QTP0002 OLD)
# qt_policy(SET QTP0004 NEW)

if(ANDROID)
    target_link_libraries(Pangaea-mobile PUBLIC
        Qt::CorePrivate)

    set_target_properties(Pangaea-mobile PROPERTIES
        QT_ANDROID_VERSION_NAME ${PROJECT_VERSION}
        QT_ANDROID_VERSION_CODE ${VERSION_CODE}
        QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
    )

    set(QT_QML_GENERATE_QMLLS_INI ON)
    set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml)

    qt_add_qml_module(qml_elements
        URI "Elements"
        VERSION 1.0
        QML_FILES
           qml/Elements/ApplicationHeader.qml
           qml/Elements/BankSpin.qml
           qml/Elements/BleListDelegate.qml
           qml/Elements/BleListHighlight.qml
           qml/Elements/CustomComboBox.qml
           qml/Elements/CustomProgressBar.qml
           qml/Elements/CustomSlider.qml
           qml/Elements/CustomLogSlider.qml
           qml/Elements/EqPoint.qml
           qml/Elements/ModeSpin_LA.qml
           qml/Elements/MButton.qml
           qml/Elements/MComboBox.qml
           qml/Elements/MRadioButton.qml
           qml/Elements/MText.qml
           qml/Elements/PresetSpin.qml
           qml/Elements/PresetSpin_LA.qml
           qml/Elements/PresetsListDelegate.qml
    )

    qt_add_qml_module(qml_control_groups
        URI "ControlGroups"
        VERSION 1.0
        QML_FILES
            qml/ControlGroups/BCutControl.qml
            qml/ControlGroups/BPeakingControl.qml
            qml/ControlGroups/BPeakingShelfControl.qml
            qml/ControlGroups/ControlButtonsGroup.qml
            qml/ControlGroups/ConfigCP.qml
            qml/ControlGroups/DocumentationWindow.qml
            qml/ControlGroups/MapCPLegacy.qml
            qml/ControlGroups/MapCPModern.qml
            qml/ControlGroups/MasterControls_CP.qml
            qml/ControlGroups/MasterControls_LA.qml
            qml/ControlGroups/ConfigUnavaliable.qml
    )

    qt_add_qml_module(qml_custom_overlays
        URI "CustomOverlays"
        VERSION 1.0
        QML_FILES
            qml/CustomOverlays/BusyScreen.qml
            qml/CustomOverlays/CustomMessageDialog.qml
            qml/CustomOverlays/EditTextDialog.qml
            qml/CustomOverlays/EditValueDialog.qml
            qml/CustomOverlays/FirmwareLoadingWindow.qml
            qml/CustomOverlays/IrManagementWindow.qml
            qml/CustomOverlays/ModulsManagementWindow.qml
            qml/CustomOverlays/PresetsWindow.qml
    )

    qt_add_qml_module(qml_modules
        URI "Modules"
        VERSION 1.0
        QML_FILES
            qml/Modules/BaseModule.qml
            qml/Modules/ClipIndicator.qml
            qml/Modules/CM.qml
            qml/Modules/EQExtLegacy.qml
            qml/Modules/EQExtModern.qml
            qml/Modules/EQPreviewLegacy.qml
            qml/Modules/EQPreviewModern.qml
            qml/Modules/HP.qml
            qml/Modules/IR.qml
            qml/Modules/LP.qml
            qml/Modules/NG.qml
            qml/Modules/OutputMode.qml
            qml/Modules/PA.qml
            qml/Modules/PR.qml
            qml/Modules/PS.qml
            qml/Modules/ER.qml
            qml/Modules/DL.qml
            qml/Modules/TR.qml
            qml/Modules/CH.qml
            qml/Modules/PH.qml
            qml/Modules/VL.qml
    )

    qt_add_qml_module(qml_tutorials
        URI "Tutorials"
        VERSION 1.0
        QML_FILES
            qml/Tutorials/SimpleTutorialMessage.qml
            qml/Tutorials/TutorialVirtualDevice.qml

            qml/Tutorials/TutorialNG.qml
            qml/Tutorials/TutorialCM.qml
            qml/Tutorials/TutorialPR.qml
            qml/Tutorials/TutorialPA.qml
            qml/Tutorials/TutorialEQ.qml
            qml/Tutorials/TutorialTR.qml
            qml/Tutorials/TutorialCH.qml
            qml/Tutorials/TutorialPH.qml
            qml/Tutorials/TutorialIR.qml
            qml/Tutorials/TutorialER.qml
            qml/Tutorials/TutorialDL.qml
    )

    qt_add_qml_module(qml_pages
        URI "Pages"
        VERSION 1.0
        QML_FILES
           qml/Pages/ConnectPage.qml
           qml/Pages/MapPage.qml
           qml/Pages/ServicePage.qml
           qml/Pages/SettingsPage.qml
    )

    set_source_files_properties(qml/StyleSettings/Style.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)
    qt_add_qml_module(qml_style
        URI "StyleSettings"
        VERSION 1.0
        QML_FILES
            qml/StyleSettings/Style.qml
            qml/StyleSettings/Theme.qml
    )
endif()

set_property(TARGET Pangaea-mobile PROPERTY QT_ANDROID_EXTRA_LIBS
   ${CMAKE_CURRENT_SOURCE_DIR}/android_ssl_libs/${CMAKE_ANDROID_ARCH_ABI}/libcrypto_1_1.so
   ${CMAKE_CURRENT_SOURCE_DIR}/android_ssl_libs/${CMAKE_ANDROID_ARCH_ABI}/libssl_1_1.so
   ${CMAKE_CURRENT_SOURCE_DIR}/android_openmp_libs/${CMAKE_ANDROID_ARCH_ABI}/libomp.so #needed for android sox lib

   ${CMAKE_CURRENT_SOURCE_DIR}/../sox_lib/lib.android/${CMAKE_ANDROID_ARCH_ABI}/libsox.so
)

# TODO automatic update translations
# find_package(Qt6 REQUIRED COMPONENTS LinguistTools)
# qt_standard_project_setup(I18N_TRANSLATED_LANGUAGES ru de it)
# qt_add_lrelease(Pangaea-mobile
# TS_FILES
#    translations/pangaea-mobile_de.ts
#    translations/pangaea-mobile_en.ts
#    translations/pangaea-mobile_it.ts
#    translations/pangaea-mobile_ru.ts
# QM_FILES_OUTPUT_VARIABLE qm_files
# )


qt6_add_resources(Pangaea-mobile "AndroidResources"
    PREFIX
        "/"
    FILES
        android/src/JavaFile.java

        android/AndroidManifest.xml
        android/build.gradle
)
