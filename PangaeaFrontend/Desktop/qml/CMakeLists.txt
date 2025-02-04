qt6_add_qml_module(${PROJECT_NAME}
    URI "PangaeaFrontend"
    VERSION 1.0
    QML_FILES
        Main.qml
    SOURCES
        ../../uicore.cpp ../../uicore.h
        ../../uisettings.cpp ../../uisettings.h
)
