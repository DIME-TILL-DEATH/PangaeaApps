import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls.Material

import QtQuick.Layouts 1.3

import CustomOverlays 1.0
import ControlGroups 1.0
import Elements 1.0
import StyleSettings 1.0

import CppObjects
import PangaeaBackend

Item
{
    id: _main

    signal openConnectPage()
    signal openSettingsWindow()

    Rectangle
    {
        anchors.fill: parent
        color: "#2B2A29"
    }

    ColumnLayout
    {
        anchors.fill: parent
        anchors.topMargin: parent.height/10
        anchors.bottomMargin: parent.height/10

        MButton
        {
            Layout.preferredWidth: parent.width*2/3
            Layout.preferredHeight: parent.height*3/20
            Layout.alignment: Qt.AlignHCenter

            //: Button text. Format device
            textButton: qsTr("Format")
            onMbPressed:
            {
                messageFormatStart.open();
            }
        }

        MButton
        {
            Layout.preferredWidth: parent.width*2/3
            Layout.preferredHeight: parent.height*3/20
            Layout.alignment: Qt.AlignHCenter

            //: Button text
            textButton: qsTr("Update Firmware")
            onMbPressed:
            {
                if(Qt.platform === "android")
                    UiCore.pickFirmwareFile();
                else
                    _iosFileDialog.open();
            }
        }

        MButton
        {
            Layout.preferredWidth: parent.width*2/3
            Layout.preferredHeight: parent.height*3/20
            Layout.alignment: Qt.AlignHCenter

            //: Button text
            textButton: qsTr("Set unique name")
            onMbPressed:
            {
                nameDialog.open()
            }
        }

        MButton
        {
            Layout.preferredWidth: parent.width*2/3
            Layout.preferredHeight: parent.height*3/20
            Layout.alignment: Qt.AlignHCenter

            //: Button text
            textButton: qsTr("Application settings")
            onMbPressed:
            {
                _main.openSettingsWindow();
            }
        }

        MButton
        {
            Layout.preferredWidth: parent.width*2/3
            Layout.preferredHeight: parent.height*3/20
            Layout.alignment: Qt.AlignHCenter

            //: Button text
            textButton: qsTr("Documentation")
            onMbPressed:
            {
                documentationWindow.open();
            }
        }

        MButton
        {
            Layout.preferredWidth: parent.width*2/3
            Layout.preferredHeight: parent.height*3/20
            Layout.alignment: Qt.AlignHCenter

            //: Button text
            textButton: qsTr("About...")
            onMbPressed:
            {
                mesAbout.open();
            }
        }
    }

    FileDialog{
        id: _iosFileDialog

        onAccepted:
        {
            UiCore.slFirmwareFilePicked(_iosFileDialog.currentFile, "")

        }
    }

    DocumentationWindow
    {
        id: documentationWindow
    }

    CustomMessageDialog
    {
        id: rssiNotificationDialog

        headerText: qsTr("Attention!")
        // text: qsTr("Firmware upload process requires a stable connection. Please place your phone as close to the device as possible. Make sure the signal level is greater than -75 dBm. (RSSI -35...-75 dBm)\n\nCurrent RSSI: ") + InterfaceManager.bleRssi + " dBm"
        // wrapMode: Text.WordWrap
        standardButtons: Dialog.Ok

        height: (_text1.textHeight +3 )*_text1.font.pixelSize*0.6
                + header.height + footer.height

        contentItem:  Column {
               id: _textLabel

               width: rssiNotificationDialog.width
               height: rssiNotificationDialog.height-rssiNotificationDialog.header.height-rssiNotificationDialog.footer.height

               spacing: 10

               MText{
                   id: _text1

                   wrapMode: Text.WordWrap
                   width: parent.width

                   text: qsTr("Firmware upload process requires a stable connection. Please place your phone as close to the device as possible. Make sure the signal level is greater than -75 dBm. (RSSI -35...-75 dBm)\n")

                   color: Style.colorText
                   anchors.horizontalCenter: _textLabel.horizontalCenter
               }

               MText{
                   id: _text2

                   width: parent.width

                   text: qsTr("Current RSSI: ") + InterfaceManager.bleRssi + " dBm"

                   color: (InterfaceManager.bleRssi>-75) ? "lightgreen" : "red"
                   // anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
               }
        }

        onAccepted:
        {
            InterfaceManager.rssiMeasuring(false);
            rssiNotificationDialog.close()
            messageAcceptFile.open()  
        }
    }

    CustomMessageDialog
    {
        id: messageAcceptFile

        headerText: qsTr("Continue?")
        text: qsTr("Do you want to load firmware file:")
        wrapMode: Text.WrapAnywhere
        standardButtons: Dialog.Ok  | Dialog.Cancel

        property var filePath

        onAccepted:
        {
            messageAcceptFile.close()
            UiCore.currentDevice.setFirmware(filePath);
        }
        onRejected:
        {
            messageAcceptFile.close()
        }
    }

    FirmwareLoadingWindow{
        id: mLoadFirmwareScreen

        z:1
    }

    CustomMessageDialog
    {
        id: messageDialog

        //: Complete formatting operation
        headerText: qsTr("Operation complete")
        text: qsTr("Format complete")

        closeOnDisconnect: true

        onRejected:
        {
            _main.openConnectPage();
            UiCore.disconnectFromDevice();
        }
    }

    CustomMessageDialog
    {
        id: mesNoFW

        headerText: qsTr("Error")
        text: qsTr("This file is not a firmware file")

        closeOnDisconnect: true

        buttons: Dialog.Ok
    }

    CustomMessageDialog
    {
        id: mesAbout
        headerText: qsTr("About...")

        property string appString: (Qt.platform === "android") ? qsTr("AMT Pangaea CP-16/CP-100\nAndroid application\nVersion: ")
                                                               : qsTr("AMT Pangaea CP-16/CP-100\niOS application\nVersion: ")
        text: appString + Qt.application.version + "\n"
              + "(c) 2025\nwebsite: https://amtelectronics.com"

        closeOnDisconnect: true

        buttons: Dialog.Ok
    }

    CustomMessageDialog
    {
        id: messageFormatStart

        headerText: qsTr("Formatting")
        text: qsTr("Formatting! User data will be lost!")
        standardButtons: Dialog.Cancel | Dialog.Ok

        closeOnDisconnect: true

        onAccepted: UiCore.currentDevice.formatMemory();
    }

    EditTextDialog
    {
        id: nameDialog

        contentText: UiCore.moduleName

        headerText: qsTr("Set module name")
        text: qsTr("Set unique name for current module:")

        onAccepted:
        {
            UiCore.moduleName = contentText;
        }
        onRejected:
        {
            contentText = UiCore.moduleName;
        }
    }

    Connections
    {
        target: UiCore

        function onSgSetUIText(nameParam, value)
        {
            if(nameParam === "firmware_file_picked")
            {
                var dataArray = value.split(',');

                messageAcceptFile.text = qsTr("Do you want to load firmware file:\n") + dataArray[1];
                messageAcceptFile.filePath = dataArray[0];
                // messageAcceptFile.open()
                rssiNotificationDialog.open()
                InterfaceManager.rssiMeasuring(true);
            }
        }
    }

    Connections
    {
        target: UiCore.currentDevice

        function onSgDeviceMessage(type, description, params)
        {
            switch(type)
            {
                case DeviceMessageType.FirmwareUpdateStarted:
                {
                    mLoadFirmwareScreen.open()
                    break;
                }

                case DeviceMessageType.FirmwareUpdateFinished:
                {
                    mLoadFirmwareScreen.close()

                    messageDialog.headerText = qsTr("Operation complete");
                    messageDialog.text = qsTr("The Firmware file has been transmitted\nPlease, reconnect to device");
                    messageDialog.standardButtons = Dialog.Close;
                    messageDialog.open();
                    break;
                }

                case DeviceMessageType.FormatMemoryFinished:
                {
                    console.log("message recieved")
                    messageDialog.headerText = qsTr("Operation complete");
                    messageDialog.text = qsTr("Formatting is complete\nPlease reset the device power and reconnect");
                    messageDialog.standardButtons = Dialog.Close;
                    messageDialog.open();
                    break;
                }
            }
        }

        function onSgDeviceError(type, description, params)
        {
            switch(type)
            {
                case DeviceErrorType.FormatMemoryError:
                {
                    messageDialog.headerText = qsTr("Error");
                    messageDialog.text = qsTr("Format error");
                    messageDialog.standardButtons = StandardButton.Ok;
                    messageDialog.open();
                    break;
                }

                case DeviceErrorType.FirmwareFileError:
                {
                    mesNoFW.text = qsTr("Firmware file error:  " + description)
                    mesNoFW.open()
                }
            }
        }
    }
}
