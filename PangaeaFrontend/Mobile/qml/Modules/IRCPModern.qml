import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.0
//import QtQuick.Dialogs 1.3

import CustomOverlays 1.0
import StyleSettings 1.0
import Elements 1.0
import Tutorials

import CppObjects
import PangaeaBackend

BaseModule
{
    id: main

    property CabSim module

    showDescription: false

    signal openIrManagementWindow();

    contentItem: Column{
        CustomSlider
        {
            height: parent.height/3
            width: parent.width

            ctrlValInstance: module.send

            moduleOn: main.on
        }

        Item{
            width: parent.width
            height: parent.height/3
            MText{
                id: _impulseName

                width: parent.width

                text: (module.impulseName == "") ? qsTr("empty") : module.impulseName
                color: on ? Style.colorText : Style.currentTheme.colorTextDisabled

                elide: Text.ElideMiddle

                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter

                anchors.horizontalCenter: parent.horizontalCenter
                // anchors.verticalCenter: parent.verticalCenter

                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        switch(UiCore.currentDevice.deviceType)
                        {
                            case DeviceType.LA3:
                            case DeviceType.MODERN_CP:
                            {
                                // _irManagement.open();
                                openIrManagementWindow();
                                // _irManagement.visible = true;
                                break;
                            }
                            default:
                            {
                                UiCore.uploadIr("");
                                break;
                            }
                        }

                    }
                }
            }
        }

        ClipIndicator{
            id: _clipInd

            on: main.on

            width: parent.width
            height: parent.height/3

            Connections{
                target: UiCore.currentDevice

                function onIrClipped(inClips, outClips){
                    _clipInd.setInIndicator(inClips);
                    _clipInd.setOutIndicator(outClips);
                }
            }
        }
    }

    CustomMessageDialog{
        id: _msgNotSupportedIrFormat

        closeOnDisconnect: true

        headerText: qsTr("Not supported format")

        buttons: Dialog.Yes | Dialog.No

        property string dstPath: (_irManagement === undefined) ? "" : _irManagement.dstIrPath

        onAccepted:
        {
            UiCore.convertAndUploadIr("", dstPath);
        }
    }

    CustomMessageDialog{
        id: _msgTrimFileDialog

        closeOnDisconnect: true

        headerText: qsTr("Trim IR file")

        buttons: Dialog.Yes | Dialog.No

        property string srcPath
        property string dstPath

        onAccepted:
        {
            UiCore.currentDevice.startIrUpload(srcPath, dstPath, true);
        }

        onRejected:
        {
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
