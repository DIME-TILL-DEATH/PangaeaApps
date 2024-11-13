import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.0
//import QtQuick.Dialogs 1.3

import StyleSettings 1.0
import Elements 1.0
import Tutorials

import CppObjects
import CppEnums

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
        target: UiCore.currentDevice

        function onSgDeviceError(type, description, params)
        {
            if(type === DeviceErrorType.IrFormatNotSupported)
            {
                _msgNotSupportedIrFormat.text = qsTr("Pangaea doesn't support this wav format:") + "\n" +
                                     description + "\n" +
                                     qsTr("Do you want to convert it before upload?")
                _msgNotSupportedIrFormat.open();
            }
        }
    }

    tutorialItem: SimpleTutorialMessage{
        buttons: Dialog.Ok

        headerText: qsTr("Impulse responce(cabinet simulator)")

        text: qsTr("Impulse Response or IR is a sonic fingerprint of a certain speaker, cabinet, mic, mic preamp and room where the profile was captured.");
    }
}
