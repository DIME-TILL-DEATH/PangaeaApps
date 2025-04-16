qt_add_library(qml_modules_fx STATIC)

qt_add_qml_module(qml_modules_fx
    URI "ModulesFX"
    VERSION 1.0
    QML_FILES
        BaseModule.qml
        PR.qml
        QML_FILES PA.qml
    )
