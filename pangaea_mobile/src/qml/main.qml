import QtQuick

import QtQuick.Controls.Material
//import QtQuick.Controls 2.12

import Modules 1.0
import Elements 1.0
import Pages 1.0

import StyleSettings 1.0

ApplicationWindow
{
    id: _main

    visible: true
    //height: 600
    //width:  300

//    visibility: Window.FullScreen

    color: "#EBECEC"

    Material.theme: Material.Dark
    Material.accent: Style.currentTheme.materialAccent

    Material.elevation: 4

    property string markEdit: edit?" * ":" "
    property bool edit: true
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
        height: parent.height*0.98

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
        currentIndex: _swipeView.currentIndex

        TabButton {
            height: parent.height
            anchors.top: parent.top
            text: qsTr("Connect")
            onClicked: {
                openConnectPage();
            }
         }
         TabButton {
             height: parent.height
             anchors.top: parent.top
             text: qsTr("Controls")
             onClicked: {
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
            openConnectPage();
            _uiCore.sgSetUIParameter("ready_to_disconnect", true);
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
            _swipeView.currentIndex=2;
            _uiCore.setFirmware(firmwareLocalPath);
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
            _uiCore.doOnlineFirmwareUpdate();
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
            _uiCore.rescanDevices()
        }
    }

    Component.onCompleted:
    {
        _uiCore.setupApplication();
        _uiCore.rescanDevices();
    }

    Connections{
        target: _servicePage

        function onOpenSettingsWindow()
        {
            _swipeView.currentIndex=3
        }

        function onOpenConnectPage()
        {
            openConnectPage();
        }
    }

    Connections{
        target: _settingsPage

        function onCloseSettingsWindow()
        {
            _swipeView.currentIndex=2
        }
    }

    Connections
    {
        target: _uiCore

        function onSgConnectReady()
        {
            _swipeView.setCurrentIndex(1);
            _uiCore.readAll();
        }

        function onSgSetUIParameter(nameParam, inValue)
        {
            if( nameParam === "presetEdit" )
            {
                _main.edit = inValue;
            }
        }

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

            if(nameParam === "exchange_error")
            {
                _msgExchangeError.open()
            }

            if(nameParam === "preset_not_saved")
            {
                _msgCommon.headerText = qsTr("Warning");
                _msgCommon.text = qsTr("You must save preset before export");
                _msgCommon.open();
            }

            if(nameParam === "preset_import_unsuccecfull")
            {
                _msgCommon.headerText = qsTr("Warning");
                _msgCommon.text = qsTr("Preset import error!");
                _msgCommon.open();
            }

            if(nameParam === "preset_exported")
            {
                _msgCommon.headerText = qsTr("Preset export finished");
                _msgCommon.text = qsTr("Preset exported to:\n" + value);
                _msgCommon.open();
            }
        }

        function onSgLocalBluetoothNotReady(Msg)
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
            if(Msg === "DeviceUnavaliable")
            {
                _msgBluetoothNotReady.text = qsTr("Device is unavaliable")
            }
            if(Msg === "UnknownBleError")
            {
                _msgBluetoothNotReady.text = qsTr("Unknown bluetooth error occured. Check if bluetooth and geolocation modules are enabled and click ok.")
            }

            _msgBluetoothNotReady.open();
            console.log("Open _msgBluetoothNotReady dialog");
        }
    }

    onClosing:
    {
        _uiCore.sw4Enable();
        console.log("Close");
    }
}
