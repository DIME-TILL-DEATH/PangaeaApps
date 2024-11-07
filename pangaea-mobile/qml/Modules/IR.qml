import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.0
//import QtQuick.Dialogs 1.3

import StyleSettings 1.0
import Elements 1.0

import CppObjects

BaseModule
{
    id: main

    property CabSim module

    moduleDescription: qsTr("Impulse response")

    showDescription: false

    contentItem:Item{
        MText{
            id: _impulseName

            width: parent.width

            text: (module.impulseName == "") ? qsTr("empty") : module.impulseName
            color: on ? Style.colorText : Style.currentTheme.colorTextDisabled

            elide: Text.ElideMiddle

            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    UiCore.setImpuls("");
                }
            }
        }
    }

    CustomMessageDialog{
        id: _msgNotSupportedIrFormat

        closeOnDisconnect: true

        headerText: qsTr("Not supported format")

        buttons: Dialog.Yes | Dialog.No

        onAccepted:
        {
            UiCore.convertAndUploadImpulse("");
        }
    }

    Connections
    {
        target: UiCore
        function onSgSetUIText(nameParam, value)
        {
            if(nameParam === "not_supported_ir")
            {
                _msgNotSupportedIrFormat.text = qsTr("Pangaea doesn't support this wav format:") + "\n" +
                                     value + "\n" +
                                     qsTr("Do you want to convert it before upload?")
                _msgNotSupportedIrFormat.open();
            }
        }
    }
}
