qt_add_library(qml_modules STATIC)

qt_add_qml_module(qml_modules
    URI "Modules"
    VERSION 1.0
    QML_FILES
        BaseModule.qml
        EqParametric.qml

        Cm.qml
        Er.qml
        Ng.qml
        Hp.qml
        In.qml
        Out.qml
        Ir.qml
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
