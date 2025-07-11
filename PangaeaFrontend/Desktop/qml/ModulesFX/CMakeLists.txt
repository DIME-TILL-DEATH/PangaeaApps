qt_add_library(qml_modules_fx STATIC)

qt_add_qml_module(qml_modules_fx
    URI "ModulesFX"
    VERSION 1.0
    QML_FILES
        BaseModule.qml
        RF.qml
        NG.qml
        CM.qml
        PR.qml
        PA.qml
        IR.qml
        EQ.qml
        CH.qml
        DL.qml
        FL.qml
        PH.qml
        ER.qml
        RV.qml
        TR.qml
        MEQ.qml
    )
