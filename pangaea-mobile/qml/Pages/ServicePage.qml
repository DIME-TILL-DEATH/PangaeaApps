import QtQuick 2.15
//import QtQuick.Controls 2.15
import QtQuick.Controls.Material

import QtQuick.Layouts 1.3

import ControlGroups 1.0
import Elements 1.0
import StyleSettings 1.0

import CppObjects

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
                //fileFirmWare.open();
                UiCore.pickFirmwareFile();
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

        property var cleanPath

        onAccepted:
        {
            messageAcceptFile.close()
            UiCore.setFirmware(cleanPath);
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
        id: message

        //: Complete formatting operation
        headerText: qsTr("Operation complete")
        text: qsTr("Format complete")

        closeOnDisconnect: true

        onRejected:
        {
            _main.openConnectPage();
            // UiCore.sgSetUIParameter("fw_update_complete", true);
            InterfaceManager.disconnectFromDevice();
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
        text: qsTr("AMT Pangaea CP-16/CP-100\nAndroid application\nVersion: ")
              + Qt.application.version + "\n"
              + qsTr("(c) 2024\nwebsite: https://amtelectronics.com")

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

        onAccepted: AppProperties.formatFlash();
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

        function onSgSetUIParameter(nameParam, inValue)
        {
            if( nameParam === "format_complete" )
            {
                message.headerText = qsTr("Operation complete");
                message.text = qsTr("Formatting is complete\nPlease reset the device power and reconnect");
                message.standardButtons = Dialog.Close;
                message.open();
            }

            if( nameParam === "format_error" )
            {
                message.headerText = qsTr("Error");
                message.text = qsTr("Format error");
                message.standardButtons = StandardButton.Ok;
                message.open();
            }

            if(nameParam === ("fw_update_enabled"))
            {
                if(inValue === 1)
                {
                    mLoadFirmwareScreen.open()
                }
                else
                {
                    mLoadFirmwareScreen.close()

                    message.headerText = qsTr("Operation complete");
                    message.text = qsTr("The Firmware file has been transmitted\nPlease, reconnect to device");
                    message.standardButtons = Dialog.Close;
                    message.open();
                }
            }
        }

        function onSgSetUIText(nameParam, value)
        {
            if(nameParam === "open_fw_file_error")
            {
                mesNoFW.text = qsTr("Can't open file " + value)
                mesNoFW.open()
            }

            if(nameParam === "firmware_file_picked")
            {
                var dataArray = value.split(',');

                messageAcceptFile.text = qsTr("Do you want to load firmware file:\n") + dataArray[1];
                messageAcceptFile.cleanPath = dataArray[0];
                // messageAcceptFile.open()
                rssiNotificationDialog.open()
                InterfaceManager.rssiMeasuring(true);
            }
        }
    }
}
