qt_add_library(qml_cp100fx STATIC)

qt_add_qml_module(qml_cp100fx
    URI "CP100FX"
    VERSION 1.0
    # NO_PLUGIN
    QML_FILES

        ControlLayoutCP100FX.qml
        HeadCP100FX.qml
        MapFx.qml
        PresetViewFx.qml
        SystemViewFx.qml
        FswMenu.qml
        ControllersMenu.qml
        MasterMenu.qml
)
