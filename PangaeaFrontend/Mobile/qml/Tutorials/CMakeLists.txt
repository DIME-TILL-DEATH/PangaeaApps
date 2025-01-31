qt_add_library(qml_tutorials STATIC)

qt_add_qml_module(qml_tutorials
    URI "Tutorials"
    VERSION 1.0
    QML_FILES
        SimpleTutorialMessage.qml
        TutorialVirtualDevice.qml

        TutorialNG.qml
        TutorialCM.qml
        TutorialPR.qml
        TutorialPA.qml
        TutorialEQ.qml
        TutorialTR.qml
        TutorialCH.qml
        TutorialPH.qml
        TutorialIR.qml
        TutorialER.qml
        TutorialDL.qml
)
