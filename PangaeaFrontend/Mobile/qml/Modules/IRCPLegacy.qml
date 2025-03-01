import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Dialogs

import CustomOverlays
import StyleSettings
import Elements
import Tutorials

import CppObjects
import PangaeaBackend

BaseModule{
    id: main

    property CabSim module

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

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    if(Qt.platform === "android")
                        UiCore.uploadIr("", dstIrPath);
                    else
                        _iosFileDialog.open();
                }
            }

            FileDialog{
                id: _iosFileDialog

                onAccepted:
                {
                    UiCore.importPreset(_iosFileDialog.currentFile, _root.dstIrPath);

                }
            }
        }
    }

    CustomMessageDialog{
        id: _msgNotSupportedIrFormat

        closeOnDisconnect: true

        headerText: qsTr("Not supported format")

        buttons: Dialog.Yes | Dialog.No

        onAccepted:{
            UiCore.convertAndUploadIr("", "");
        }
    }

    CustomMessageDialog{
        id: _msgTrimFileDialog

        closeOnDisconnect: true

        headerText: qsTr("Trim IR file")

        buttons: Dialog.Yes | Dialog.No

        property string srcPath
        property string dstPath

        onAccepted:{
            UiCore.currentDevice.startIrUpload(srcPath, dstPath, true);
        }

        onRejected:{
            UiCore.currentDevice.startIrUpload(srcPath, dstPath, false);
        }
    }

    CustomMessageDialog{
        id: _msgDialog

        closeOnDisconnect: true

        headerText: qsTr("IR file exists")

        buttons: Dialog.Ok
    }


    Connections
    {
        target: UiCore.currentDevice

        function onSgDeviceError(type, description, params)
        {
            switch(type)
            {
            case DeviceErrorType.IrFormatNotSupported:
            {
                _msgNotSupportedIrFormat.text = qsTr("Pangaea doesn't support this wav format:") + "\n" +
                                     description + "\n" +
                                     qsTr("Do you want to convert it before upload?")
                _msgNotSupportedIrFormat.open();
                break;
            }

            case DeviceErrorType.FileExists:
            {
                _msgDialog.text = qsTr("File ") + params + qsTr(" already on device.")
                _msgDialog.open();
                break;
            }
            }
        }
    }

    Connections
    {
        target: UiCore

        function onSgUiMessage(type, message, messageParams)
        {
            if(type === UiMessageType.PROPOSE_IR_TRIM)
            {
                _msgTrimFileDialog.text = qsTr("The length of the selected file is greater than what is used when processing the signal. Would you like to trim impulse to speed up uploading and save space in device memory?")
                _msgTrimFileDialog.srcPath = messageParams[1]
                _msgTrimFileDialog.dstPath = messageParams[2]
                _msgTrimFileDialog.open()
            }
        }

    }

    tutorialItem: TutorialIR{}
}
