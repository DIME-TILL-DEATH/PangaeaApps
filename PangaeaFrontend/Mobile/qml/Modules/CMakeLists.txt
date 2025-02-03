qt_add_library(qml_modules STATIC)

qt_add_qml_module(qml_modules
    URI "Modules"
    VERSION 1.0
    QML_FILES
        BaseModule.qml
        ClipIndicator.qml
        CM.qml
        EQExtLegacy.qml
        EQExtModern.qml
        EQPreviewLegacy.qml
        EQPreviewModern.qml
        HP.qml
        IRCPLegacy.qml
        IRCPModern.qml
        LP.qml
        NG.qml
        OutputMode.qml
        PA.qml
        PR.qml
        PS.qml
        ER.qml
        DL.qml
        TR.qml
        CH.qml
        PH.qml
        VL.qml
)
