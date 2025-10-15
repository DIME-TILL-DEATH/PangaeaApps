import QtQuick 2.15
import QtQuick.Controls.Fusion
import QtQuick.Dialogs
import QtQuick.Controls as CTRLS
import QtCore

import QtQuick.Window 2.15

import StyleSettings 1.0
import Layouts 1.0
import CP100FX 1.0
import CP16 1.0

import CppObjects
import PangaeaBackend

ApplicationWindow
{
    id: main
    visible: true

    width: Screen.width * 0.75
    height: width * 0.4

    minimumWidth: Screen.width * 0.8 * 0.75
    minimumHeight: Screen.width * 0.75 * 0.4 * 0.75
    maximumWidth: Screen.width * 0.8 * 1.25
    maximumHeight: Screen.width * 0.75 * 0.4 * 1.25

    // minimumWidth: width*0.75
    // minimumHeight: height*0.75
    // maximumWidth: width*1.25
    // maximumHeight: height*1.25

    // palette.highlight: "orange"

    color: Style.currentTheme.backgroundColor

    property string markEdit: UiCore.currentDevice.deviceParamsModified ? " * ":" "
    property string devName: UiCore.currentDevice.firmwareName

    property string interfaceDescription: ""
    property string markConnect: connected ? qsTr("Connected to ") + interfaceDescription : qsTr("Disconnected")
    property string interfaceType

    property bool connected: false
    property bool appClosing: false

    title: connected ? "AMT PangaeaCPPA " +  " v." + Qt.application.version + " "
                + markConnect + devName + " (" + interfaceType + ")" + markEdit
                : "AMT PangaeaCPPA " + " v." + Qt.application.version + " " + markConnect


    header: MainMenu{
        id: _mainMenu

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

    NativeMessageDialog{
        id: msgPresetChangeSave

        property int newBank
        property int newPreset

        title: qsTr("Save preset")
        text: qsTr("Do you want to save changes?")

        buttons: DialogButtonBox.Save | DialogButtonBox.No | DialogButtonBox.Cancel

        onButtonClicked: function(button){
            switch(button.DialogButtonBox.buttonRole)
            {
                case DialogButtonBox.AcceptRole:
                {
                    UiCore.currentDevice.saveChanges();
                    if(!main.appClosing) UiCore.currentDevice.changePreset(newBank, newPreset, true);
                    break;
                }
                case DialogButtonBox.NoRole:
                {
                    if(!main.appClosing)
                    {
                        UiCore.currentDevice.changePreset(newBank, newPreset, true);
                        if(UiCore.currentDevice.deviceType < DeviceType.LEGACY_DEVICES)
                            UiCore.currentDevice.escImpulse();
                    }
                    else
                    {
                        UiCore.currentDevice.comparePreset();
                    }
                    break;
                }
                case DialogButtonBox.RejectRole:
                {
                    appClosing = false;
                    UiCore.currentDevice.deviceRestoreValues();
                    break;
                }
            }
        }
    }

    NativeMessageDialog
    {
        id: msgInfo

        title: qsTr("Error")
        text: qsTr("Device is disconnected")

        modality: Qt.ApplicationModal
        onAccepted: {
            mBusy.visible = false;
        }
    }

    NativeMessageDialog
    {
        id: msgExchangeError

        title: qsTr("Error")
        text: qsTr("Device is disconnected")

    }

    NativeMessageDialog
    {
        id: _msgVersionInform        
    }

    // modality works only in Labs
    NativeMessageDialog
    {
        id: operationCompleteDialog

        title: qsTr("Operation complete")

        text: qsTr("Operation complete. Please, reconnect to device")

        onAccepted: {
            mBusy.visible = false;
            UiCore.disconnectFromDevice();
        }
    }

    MBusy
    {
        id: mBusy
        z: main.z + 50
    }

    Connections
    {
        target: UiCore

        function onSgNewFirmwareAvaliable(firmwareVersionString)
        {
            _msgVersionInform.title = qsTr("Info")
            _msgVersionInform.text = qsTr("New firmware version(v.") +
                    firmwareVersionString +
                    qsTr(") avaliable on the server")
            _msgVersionInform.open()
        }

        function onSgNewAppVersionAvaliable(appVersionString)
        {
            _msgVersionInform.title = qsTr("Info")
            _msgVersionInform.text = qsTr("New application version(v.") +
                    appVersionString +
                    qsTr(") avaliable on the server")
            _msgVersionInform.open()
        }

        function onSgQmlRequestChangePreset(bank, preset)
        {
            if(UiCore.currentDevice.bank !== bank || UiCore.currentDevice.preset !== preset)
            {
                if(UiCore.currentDevice.deviceParamsModified)
                {
                    msgPresetChangeSave.newBank = bank;
                    msgPresetChangeSave.newPreset = preset;

                    msgPresetChangeSave.open()
                }
                else
                {
                    UiCore.currentDevice.changePreset(bank, preset);
                }
            }
        }

        function onCurrentDeviceChanged(){

            switch(UiCore.currentDevice.deviceType){
            case DeviceType.UNKNOWN_DEVICE:{
                controlLayoutLoader.source = "";
                break;
            }

            case DeviceType.LA3:
            case DeviceType.MODERN_CP:{
                startUi.visible = false;
                controlLayoutLoader.source = "/CP16/ControlLayoutCPModern.qml";
                break;
            }

            case DeviceType.CP100FX:{
                startUi.visible = false;
                controlLayoutLoader.source = "/CP100FX/ControlLayoutCP100FX.qml";
                break;
            }

            default:{
                startUi.visible = false;
                controlLayoutLoader.source = "/CP16/ControlLayoutLegacy.qml";
            }
            }

        }
    }

    Connections{
        target: UiCore.currentDevice

        function onSgDeviceError(type, description, params)
        {
            switch(type)
            {
                case DeviceErrorType.UndefinedCommand:
                {
                    msgInfo.title = qsTr("Device parse error");
                    msgInfo.text = qsTr("Device can't recognize command:\n'" + params[1] + "'\nTry to update firmware");
                    msgInfo.open();
                    break;
                }

                case DeviceErrorType.IrSaveError:
                {
                    msgInfo.title = qsTr("Error");
                    msgInfo.text = qsTr("Error while saving IR. Please, try to reload impulse.");
                    msgInfo.open();
                    break;
                }

                case DeviceErrorType.PresetImportUnsuccesfull:
                {
                    msgInfo.title = qsTr("Error")
                    msgInfo.text = qsTr("Not a Pangaea preset file!")
                    msgInfo.open();
                    break;
                }

                case DeviceErrorType.PresetNotSaved:
                {
                    msgInfo.title = qsTr("Error")
                    msgInfo.text = qsTr("You must save preset before export");
                    msgInfo.open();
                    break;
                }

                case DeviceErrorType.FimrmwareVersionInsufficient:
                {
                    _msgVersionInform.title = qsTr("Warning")
                    _msgVersionInform.text = qsTr("Version error!")
                    _msgVersionInform.text = qsTr("Firmware version of your device is ") + params[0]
                            + qsTr("\nMinimum required version is ")
                            + params[1]
                            + qsTr("\nDo you want to update firmware now?\nWARNING!!! Updating firmware may take several minutes!")

                    _msgVersionInform.visible = true;
                    break;
                }

                case DeviceErrorType.CopyFileError:
                {
                    msgInfo.title = qsTr("Error");
                    msgInfo.text = qsTr("IR file copying error.")
                    msgInfo.visible = true;
                    break;
                }
            }
        }

        function onSgDeviceMessage(type, message, params)
        {
            switch(type)
            {
                case DeviceMessageType.PresetExportFinished:
                {
                    msgInfo.title = qsTr("Preset export finished");
                    msgInfo.text = qsTr("Preset exported to:\n" + message);
                    msgInfo.open();
                    break;
                }

                case DeviceMessageType.FirmwareUpdateFinished:
                {
                    operationCompleteDialog.open();
                    break;
                }
            }
        }
    }

    Connections{
        target: InterfaceManager

        function onSgDeviceUnavaliable(senderType, reason)
        {
            msgInfo.text = qsTr("Device is unavaliable")
            msgInfo.open();
        }

        function onSgExchangeError()
        {
            controlLayoutLoader.source = "";
            msgExchangeError.text = qsTr("Command exchange error")
            msgExchangeError.open();
        }

        function onSgInterfaceConnected(interfaceDescription)
        {
            connected = true;

            switch(interfaceDescription.connectionType){
                case DeviceConnectionType.USB: main.interfaceType = qsTr("USB"); break;
                case DeviceConnectionType.BLE: main.interfaceType = qsTr("BLUETOOTH"); break;
                case DeviceConnectionType.Offline: main.interfaceType = qsTr("VIRTUAL"); break;
                default: main.interfaceType = qsTr("Unknown interface"); break;
            }

            msgInfo.close();
        }

        function onSgInterfaceError(errorDescription)
        {
            connected = false;
            startUi.visible = true;;
            msgInfo.text = qsTr("Device disconnected\n" + errorDescription)
            msgInfo.open();

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

        switch(UiSettings.colorTheme){
            case UiSettings.ClassicBlue: Style.currentTheme = Style.themeClassicBlue; break;
            case UiSettings.DarkOrange: Style.currentTheme = Style.themeDarkOrange; break;
            case UiSettings.DarkBlue: Style.currentTheme = Style.themeDarkBlue; break;
            case UiSettings.DarkGreen: Style.currentTheme = Style.themeDarkGreen; break;
        }
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
