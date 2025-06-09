qt_add_library(qml_modules_classic STATIC)

qt_add_qml_module(qml_modules_classic
    URI "ModulesClassic"
    VERSION 1.0
    QML_FILES
        BaseModule.qml
        EqLegacy.qml
        EqModern.qml
        Cm.qml
        Er.qml
        Ng.qml
        Hp.qml
        In.qml
        Out.qml
        Ir.qml
        IrCpModern.qml
        Lp.qml
        Pa.qml
        Pr.qml
        Ps.qml
        Vl.qml
        Tr.qml
        Ch.qml
        Dl.qml
        Ph.qml
    )
