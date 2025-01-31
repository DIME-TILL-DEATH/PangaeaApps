qt6_add_qml_module(${PROJECT_NAME}
    URI PanageaFrontend
    VERSION 1.0
    RESOURCE_PREFIX /
    # OUTPUT_DIRECTORY qml/
    QML_FILES
        Main.qml
    SOURCES
        ../../uicore.cpp ../../uicore.h
        ../../uisettings.cpp ../../uisettings.h
)
