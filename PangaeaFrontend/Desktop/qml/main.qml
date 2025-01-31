import QtQuick 2.15
import QtQuick.Controls.Fusion
import QtQuick.Dialogs
import QtCore

import Qt.labs.platform 1.1 as Labs

import QtQuick.Window 2.15

import StyleSettings 1.0
import Layouts 1.0

import CppObjects
import PangaeaBackend

ApplicationWindow
{
    id: main
    visible: true

    width: Screen.width * 0.75
    height: width * 0.4

    minimumWidth: width*0.75
    minimumHeight: height*0.75
    maximumWidth: width*1.25
    maximumHeight: height*1.25

    // palette.highlight: "orange"

    // color: Style.backgroundColor

    property string markEdit: UiCore.currentDevice.deviceParamsModified ? " * ":" "
    property string devName: UiCore.currentDevice.firmwareName
    property string devVersion: ""

    property string interfaceDescription: ""
    property string markConnect: connected ? qsTr("Connected to ") + interfaceDescription : qsTr("Disconnected")

    property bool connected: false
    property bool wait: true
    property bool appClosing: false

    title: connected ? "AMT PangaeaCPPA " +  " v." + Qt.application.version + " "
                + markConnect + devName + " (firmware v." + devVersion +") " + markEdit
                : "AMT PangaeaCPPA " + " v." + Qt.application.version + " " + markConnect


    header: MainMenu{
        visible: main.connected
        enabled: !mBusy.visible
    }

    StartLayout
    {
        id: startUi
    }

    Loader{
        id: controlLayoutLoader

        anchors.fill: parent
    }

    MessageDialog
    {
        id: msgPresetChangeSave

        property int newBank
        property int newPreset

        text: qsTr("Do you want to save changes?")
        title: qsTr("Save preset")

        buttons: MessageDialog.Save | MessageDialog.No | MessageDialog.Cancel
        onButtonClicked: function(button, role){
            switch(button)
            {
                case MessageDialog.Save:
                {
                    UiCore.currentDevice.saveChanges();
                    if(!main.appClosing) UiCore.currentDevice.changePreset(newBank, newPreset, true);
                    break;
                }
                case MessageDialog.No:
                {
                    if(!main.appClosing)
                    {
                        UiCore.currentDevice.changePreset(newBank, newPreset, true);

                        UiCore.currentDevice.escImpulse();
                    }
                    else
                    {
                        UiCore.currentDevice.comparePreset(); // restore preset TODO: честный restore
                    }
                    break;
                }
                case MessageDialog.Cancel:
                {
                    appClosing = false;
                    UiCore.currentDevice.deviceRestoreValues();
                    break;
                }
            }
        }
    }

    MessageDialog{
        id: msgIncorretIR

        title: qsTr("Incorrect wav format")

        buttons: MessageDialog.Yes | MessageDialog.No

        onButtonClicked: function (button, role) {
            switch(button){
            case MessageDialog.Yes:
                var cleanPath = irFileDialog.currentFile.toString();
                cleanPath = (Qt.platform.os==="windows")?decodeURIComponent(cleanPath.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"")):decodeURIComponent(cleanPath.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,""));
                UiCore.convertAndUploadImpulse(cleanPath);
                break;
            }
        }
    }

    Labs.MessageDialog
    {
        id: msgError

        title: qsTr("Error")
        text: qsTr("Device is disconnected")

        modality: Qt.ApplicationModal
        onOkClicked: {
            mBusy.visible = false;
        }
    }

    Labs.MessageDialog
    {
        id: msgExchangeError

        title: qsTr("Error")
        text: qsTr("Device is disconnected")

        modality: Qt.ApplicationModal
        onOkClicked: {
            InterfaceManager.disconnectFromDevice();
        }
    }

    MessageDialog
    {
        id: _msgVersionInform        
    }

    // modality works only in Labs
    Labs.MessageDialog
    {
        id: operationCompleteDialog

        title: qsTr("Operation complete")

        text: qsTr("Operation complete. Please, reconnect to device")

        modality: Qt.ApplicationModal

        onOkClicked: {
            mBusy.visible = false;
            InterfaceManager.disconnectFromDevice();
        }
    }

    MBusy
    {
        id: mBusy
        z:1
    }

    Connections
    {
        target: UiCore

        function onSgSetUIText(nameParam, inString)
        {
            if(nameParam === "not_supported_ir")
            {
                msgIncorretIR.text = qsTr("Pangaea doesn't support this wav format:") + "\n" +
                                     inString + "\n" +
                                     qsTr("Do you want to convert it before upload?")
                msgIncorretIR.open();
            }

            if(nameParam === "preset_import_unsuccecfull")
            {
                msgError.text = qsTr("Not a Pangaea preset file!")
                msgError.open();
            }

            if(nameParam === "devVersion")
            {
                devVersion = inString;
            }

            if(nameParam === "firmware_version_error")
            {
                 var versionArray = inString.split(',');

                _msgVersionInform.title = qsTr("Warning")
                _msgVersionInform.text = qsTr("Version error!")

                _msgVersionInform.informativeText = qsTr("Firmware version of your device is ") +
                        versionArray[0] + "\n" +
                        qsTr("Minimum requsted version is ") + versionArray[1]
                _msgVersionInform.visible = true;
            }

            if(nameParam === "new_firmware_avaliable")
            {
                versionArray = inString.split(',');

                _msgVersionInform.title = qsTr("Info")
                _msgVersionInform.text = ""
                _msgVersionInform.informativeText = qsTr("New firmware version(v.") +
                        versionArray[1] +
                        qsTr(") avaliable on the server")
                _msgVersionInform.visible = true;
            }

            if(nameParam === "new_app_version_avaliable")
            {
                _msgVersionInform.title = qsTr("Info")
                _msgVersionInform.text = ""
                _msgVersionInform.informativeText = qsTr("New application version(v.") +
                        inString +
                        qsTr(") avaliable on the server")
                _msgVersionInform.visible = true;
            }

            if(nameParam === "preset_not_saved")
            {
                msgError.text = qsTr("You must save preset before export");
                msgError.open();
            }

            if(nameParam === "impulse_save_error")
            {
                msgError.text = qsTr("Error while saving IR. Please, try to reload impulse.");
                msgError.open();
            }
        }

        function onSgSetUIParameter(nameParam, value)
        {
            if(nameParam === "wait")
            {
                wait = value;
                if(main.appClosing)
                {
                    if(!wait && !msgPresetChangeSave.visible)
                        Qt.quit();
                }
            }

            if(nameParam === "fw_update_enabled")
            {
                if(!value)
                {
                    operationCompleteDialog.open();
                }
            }
        }
    }

    Connections{
        target: UiCore

        function onSgQmlRequestChangePreset(bank, preset)
        {
            if(UiCore.currentDevice.bank !== bank || UiCore.currentDevice.preset !== preset)
            {
                if(UiCore.currentDevice.deviceParamsModified)
                {
                    msgPresetChangeSave.newBank = bank;
                    msgPresetChangeSave.newPreset = preset;
                    msgPresetChangeSave.open();
                }
                else
                {
                    UiCore.currentDevice.changePreset(bank, preset);
                }
            }
        }

        function onCurrentDeviceChanged(){

            console.log("Current device chabged", UiCore.currentDevice.deviceType)

            switch(UiCore.currentDevice.deviceType){
            case DeviceType.UNKNOWN_DEVICE:{
                controlLayoutLoader.source = "";
                break;
            }

            case DeviceType.MODERN_CP:{
                break;
            }

            default:{
                startUi.visible = false;
                controlLayoutLoader.source = "/Layouts/ControlLayoutLegacy.qml";
            }
            }

        }
    }

    Connections{
        target: InterfaceManager

        function onSgDeviceUnavaliable(senderType, reason)
        {
            msgError.text = qsTr("Device is unavaliable")
            msgError.open();
        }

        function onSgExchangeError()
        {
            msgExchangeError.text = qsTr("Command exchange error")
            msgExchangeError.open();
        }

        function onSgInterfaceConnected(interfaceDescription)
        {
            connected = true;
            interfaceDescription = interfaceDescription.address
            msgError.close();
        }

        function onSgInterfaceError(errorDescription)
        {
            connected = false;
            startUi.visible = true;;
            msgError.text = qsTr("Device disconnected\n" + errorDescription)
            msgError.open();

            InterfaceManager.startScanning(DeviceConnectionType.BLE);
            InterfaceManager.startScanning(DeviceConnectionType.USB);
        }        

        function onSgInterfaceDisconnected()
        {
            connected = false;
            startUi.visible = true;

            InterfaceManager.startScanning(DeviceConnectionType.BLE);
            InterfaceManager.startScanning(DeviceConnectionType.USB);
        }
    }

    Component.onCompleted: {
        UiCore.setupApplication();
        UiSettings.setupApplication();
        InterfaceManager.startScanning(DeviceConnectionType.BLE);
        InterfaceManager.startScanning(DeviceConnectionType.USB);

        if(UiSettings.windowWidth!==0 && UiSettings.windowHeight!==0)
        {
            main.width = UiSettings.windowWidth;
            main.height = UiSettings.windowHeight;
        }

        main.x = Screen.width/2 - main.width/2;
        main.y = Screen.height/2 - main.height/2;
    }

    onClosing: function(close)
    {
        UiSettings.saveSetting("window_width", main.width);
        UiSettings.saveSetting("window_height", main.height);

        if(main.edit && main.connected)
        {
            main.appClosing = true;
            msgPresetChangeSave.visible = true;
            close.accepted = false;
        }
    }
}
