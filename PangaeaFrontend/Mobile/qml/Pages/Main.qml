import QtQuick
import QtQuick.Controls.Material

import CustomOverlays 1.0
import Elements 1.0
import Pages 1.0

import StyleSettings 1.0

import CppObjects
import PangaeaBackend

ApplicationWindow
{
    id: _main

    visible: true

    color: "#EBECEC"

    Material.theme: Material.Dark
    Material.accent: Style.currentTheme.materialAccent

    Material.elevation: 4

    property bool connected: false

    property bool swipeEn: true

    property int curView: 0

    function openConnectPage()    {
        _swipeView.currentIndex=0
    }

    header: ApplicationHeader
    {
    }

    Rectangle
    {
        anchors.fill: parent
        color: "#2B2A29"
    }

    SwipeView
    {
        id: _swipeView

        width: parent.width
        height: parent.height//*0.98

        anchors.horizontalCenter: parent.horizontalCenter

        currentIndex: _bar.currentIndex
        interactive: false

        ConnectPage
        {
            id: _connectPage

        }

        MapPage
        {
            id: _mapPage
        }

        ServicePage
        {
            id: _servicePage

        }

        SettingsPage
        {
            id: _settingsPage
        }
    }

    footer: TabBar{
        id: _bar

        enabled: _connectPage.isConnected

        height: _main.height/20
        // currentIndex: _swipeView.currentIndex

        TabButton {
            height: parent.height
            anchors.top: parent.top
            text: qsTr("Connect")
            onClicked: {
                _main.openConnectPage();
            }
         }
         TabButton {
             height: parent.height
             anchors.top: parent.top
             text: qsTr("Controls")
             onClicked: {
                 _mapPage.setMapContent()
                 _swipeView.currentIndex=1
             }
         }
         TabButton {
             height: parent.height
             anchors.top: parent.top
             text: qsTr("Config")
             onClicked: {
                 _mapPage.setConfigContent()
                 _swipeView.currentIndex=1

             }
         }
         TabButton {
             height: parent.height
             anchors.top: parent.top
             text: qsTr("Service")
             onClicked: {
                 _swipeView.currentIndex=2
             }
         }
    }

    BusyScreen
    {
        id: mBusy
        z:5
    }

    CustomMessageDialog
    {
        id: _msgCommon

        buttons: Dialog.Ok
    }


    CustomMessageDialog
    {
        id: _msgExchangeError

        buttons: Dialog.Ok

        text: qsTr("Exchange error between the BLE module and the PANGAEA device has occurred. Please check the firmware and connections.");

        headerText: qsTr("Exchange error")
        onAccepted: {
            _main.openConnectPage();
            UiCore.disconnectFromDevice();
        }
    }

    CustomMessageDialog
    {
        id: _msgVersionError

        property string firmwareLocalPath

        headerText: qsTr("Warning")
        text: qsTr("Firmware version error!")
        buttons: Dialog.Ok|Dialog.No

        onAccepted: {
            // _swipeView.currentIndex=2;
            // _bar.currentIndex = 3
            UiCore.setFirmware(firmwareLocalPath);
            _msgVersionError.close();
        }

        onRejected: {
            _msgVersionError.close();
        }
    }

    CustomMessageDialog
    {
        id: _msgNetUpdateFirmware

        headerText: qsTr("Update firmware")
        text: qsTr("Newest firmware avaliable on the server.")
        buttons: Dialog.Ok|Dialog.No

        onAccepted: {
            _swipeView.currentIndex=2;
            _bar.currentIndex=3
            UiCore.doOnlineFirmwareUpdate();
        }

        onRejected: {
            _msgNetUpdateFirmware.close();
        }

    }

    CustomMessageDialog
    {
        id: _msgBluetoothNotReady

        headerText: qsTr("Warning")
        text: qsTr("Bluetooth module is off! Please turn on bluetooth module on your device and click ok")
        buttons: Dialog.Ok

        onAccepted:
        {
            InterfaceManager.startScanning(DeviceConnectionType.BLE)
        }
    }

    Component.onCompleted:
    {
        UiCore.setupApplication();
        InterfaceManager.startScanning(DeviceConnectionType.BLE);
    }

    Connections{
        target: _servicePage

        function onOpenSettingsWindow()
        {
            _swipeView.currentIndex=3
            _bar.currentIndex=3
        }

        function onOpenConnectPage()
        {
            _main.openConnectPage();
        }
    }

    Connections{
        target: _settingsPage

        function onCloseSettingsWindow()
        {
            _swipeView.currentIndex=2
            _bar.currentIndex=3
        }
    }

    Connections
    {
        target: UiCore

        function onSgSetUIText(nameParam, value)
        {
            var versionArray;

            if(nameParam===("firmware_local_path"))
            {
                _msgVersionError.firmwareLocalPath = value;
            }

            if(nameParam===("firmware_version_error"))
            {
                versionArray = value.split(',');
                _msgVersionError.text = qsTr("Firmware version of your device is ") + versionArray[0]
                        + qsTr("\nMinimum required version is ")
                        + versionArray[1]
                        + qsTr("\nDo you want to update firmware now?\nWARNING!!! Updating firmware may take several minutes!")

                _msgVersionError.visible = true;
            }

            if(nameParam===("new_firmware_avaliable"))
            {
                versionArray = value.split(',');
                _msgNetUpdateFirmware.text = qsTr("New firmware avaliable on the server.\nFirmware version of your device is: ") + versionArray[0]
                        + qsTr("\nNew firmware version is: ")
                        + versionArray[1]
                        + qsTr("\nDo you want to download and update firmware now?\nWARNING!!! Updating firmware may take several minutes!")

                _msgNetUpdateFirmware.visible = true;
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
                    _msgCommon.headerText = qsTr("Device parse error");
                    _msgCommon.text = qsTr("Device can't recognize command:\n'" + params[1] + "'\nTry to update firmware");
                    _msgCommon.open();
                    break;
                }

                case DeviceErrorType.IrSaveError:
                {
                    _msgCommon.headerText = qsTr("Error");
                    _msgCommon.text = qsTr("Error while saving IR. Please, try to reload impulse.");
                    _msgCommon.open();
                    break;
                }

                case DeviceErrorType.PresetImportUnsuccesfull:
                {
                    _msgCommon.headerText = qsTr("Warning");
                    _msgCommon.text = qsTr("Preset import error!");
                    _msgCommon.open();
                    break;
                }

                case DeviceErrorType.PresetNotSaved:
                {
                    _msgCommon.headerText = qsTr("Warning");
                    _msgCommon.text = qsTr("You must save preset before export");
                    _msgCommon.open();
                    break;
                }

                case DeviceErrorType.FimrmwareVersionInsufficient:
                {
                    _msgVersionError.text = qsTr("Firmware version of your device is ") + params[0]
                            + qsTr("\nMinimum required version is ")
                            + params[1]
                            + qsTr("\nDo you want to update firmware now?\nWARNING!!! Updating firmware may take several minutes!")

                    _msgVersionError.visible = true;
                    break;
                }

                case DeviceErrorType.CopyFileError:
                {
                    _msgCommon.headerText = qsTr("Error");
                    _msgCommon.text = qsTr("IR file copying error.")
                    _msgCommon.visible = true;
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
                    _msgCommon.headerText = qsTr("Preset export finished");
                    _msgCommon.text = qsTr("Preset exported to:\n" + message);
                    _msgCommon.open();
                    break;
                }
            }
        }
    }

    Connections{
        target: InterfaceManager

        function onSgInterfaceConnected(deviceDescription)
        {
            _swipeView.setCurrentIndex(1);
            _bar.currentIndex = 1;
            _main.connected = true;
        }

        function onSgExchangeError()
        {
            _msgExchangeError.open();
            mBusy.visible = false;
        }

        function onSgInterfaceUnavaliable(interfaceType, Msg)
        {
            if(Msg === "HostPoweredOff")
            {
                _msgBluetoothNotReady.text = qsTr("Bluetooth module is off! Please turn on bluetooth module on your device and click ok")
            }
            if(Msg === "GeolocationIsOff")
            {
                _msgBluetoothNotReady.text = qsTr("Geolocation is required by Android to scan Bluetooth LE devices. Please turn on geolocation module and click ok")
            }
            if(Msg === "GeolocationPermissionDenied")
            {
                _msgBluetoothNotReady.text = qsTr("Application needs geolocation permission to scan Bluetooth LE devices. Please give app permissions and click ok")
            }
            if(Msg === "UnknownBleError")
            {
                _msgBluetoothNotReady.text = qsTr("Unknown bluetooth error occured. Check if bluetooth and geolocation modules are enabled and click ok.")
            }

            _msgBluetoothNotReady.open();
            mBusy.visible = false;
            console.log("signal interface unavaliable");
        }

        function onSgDeviceUnavaliable(interfaceType, Msg)
        {
            if(Msg === "DeviceUnavaliable")
            {
                _msgBluetoothNotReady.text = qsTr("Device is unavaliable")
            }
            if(Msg === "UnknownBleError")
            {
                _msgBluetoothNotReady.text = qsTr("Unknown bluetooth error occured. Check if bluetooth and geolocation modules are enabled and click ok.")
            }

            _msgBluetoothNotReady.open();
            mBusy.visible = false;
            console.log("signal device unavaliable");
        }

        function onSgInterfaceError(errorDescription)
        {
            _main.connected = false;
            _main.openConnectPage();
            _msgBluetoothNotReady.text = qsTr("Device disconnected\n") + errorDescription
            _msgBluetoothNotReady.open();

            InterfaceManager.startScanning(DeviceConnectionType.BLE);
            mBusy.visible = false;
        }

        function onSgInterfaceDisconnected()
        {
            _main.connected = false;
        }
    }

    // CustomMessageDialog
    // {
    //     id: _msgExit

    //     buttons: Dialog.Yes | Dialog.No

    //     text: qsTr("Do you really want to quit?");

    //     headerText: qsTr("Quit?")
    //     onAccepted: {
    //         Qt.quit();
    //     }
    // }

    onClosing: function(close)
    {
        close.accepted = false;
        // _msgExit.open();
    }
}
