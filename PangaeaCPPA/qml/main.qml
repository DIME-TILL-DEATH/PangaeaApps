import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs
import Qt.labs.settings 1.0

import Qt.labs.platform 1.1 as Labs

import QtQuick.Window 2.15

import StyleSettings 1.0
import Layouts 1.0

import CppObjects
import CppEnums

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

    color: Style.backgroundColor

    property string markEdit: edit?" * ":" "
    property string devName: ""
    property string devVersion: ""

    property string interfaceDescription: ""
    property string markConnect: connected ? qsTr("Connected to ") + interfaceDescription : qsTr("Disconnected")

    property bool editable: false
    property bool edit: false
    property bool connected: false
    property bool wait: true

    title: connected ? "AMT PangaeaCPPA " +  " v." + Qt.application.version + " "
                + markConnect + devName + " (firmware v." + devVersion +") " + markEdit
                : "AMT PangaeaCPPA " + " v." + Qt.application.version + " " + markConnect

    Settings
    {
        category: "Current_folder"
        property alias curFolder: irFileDialog.currentFolder
    }

    header: MainMenu{
        visible: main.connected
        enabled: !mBusy.visible
    }

    StartLayout
    {
        id: startUi

        visible: !mainUi.visible
    }

    Column
    {
        id: mainUi
        anchors.fill: parent
        focus: true
        spacing: 2

        visible: false

        Head
        {
            id: head

            width:  parent.width
            height: parent.height/1000*150

            onSetImpuls: {
                irFileDialog.open();
            }
            editable: main.editable & (!main.wait)
            edit:     main.edit
        }

        ModulesList
        {
            id: modules

            width:  parent.width
            height: parent.height/1000*850

            enabled: editable & (!main.wait)
        }
    }

    FileDialog
    {
        id: irFileDialog

        title: qsTr("Select IR")
        nameFilters: [ "Wav files (*.wav)" ]

        onAccepted:
        {
            var cleanPath = irFileDialog.currentFile.toString();
            cleanPath = (Qt.platform.os==="windows")?decodeURIComponent(cleanPath.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"")):decodeURIComponent(cleanPath.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,""));
            UiCore.setImpuls(cleanPath);
        }
    }

    MessageDialog
    {
        id: msgPresetChangeSave

        property int saveParam: 0

        text: qsTr("Do you want to save changes?")
        title: qsTr("Save preset")

        buttons: MessageDialog.Save | MessageDialog.No | MessageDialog.Cancel
        onButtonClicked: function(button, role){
            switch(button)
            {
                case MessageDialog.Save:
                {
                    UiCore.setParameter("save_change", saveParam);
                    UiCore.setParameter("do_preset_change", saveParam);
                    break;
                }
                case MessageDialog.No:
                {
                    UiCore.restoreParameter("impulse")
                    UiCore.setParameter("do_preset_change", saveParam);
                    break;
                }
                case MessageDialog.Cancel:
                {
                    saveParam = 0
                    UiCore.restoreParameter("preset")
                    UiCore.restoreParameter("bank")
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

    MessageDialog
    {
        id: msgError

        title: qsTr("Error")
        text: qsTr("Device is disconnected")

        modality: Qt.ApplicationModal
        onButtonClicked: {
            mBusy.visible = false;
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

        function onSgPresetChangeStage(inChangePreset)
        {
            msgPresetChangeSave.saveParam = inChangePreset;
            msgPresetChangeSave.visible = true;
        }

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

            if(nameParam === "version_error")
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
        }

        function onSgSetUIParameter(nameParam, value)
        {
            if(nameParam === "preset_edited")
            {
                edit = value;
            }

            if(nameParam === "wait")
            {
                wait = value;
                if(msgPresetChangeSave.saveParam==(-2))
                {
                    if(!value && (!msgPresetChangeSave.visible) && (msgPresetChangeSave.saveParam==(-2)))
                        Qt.quit();
                }
            }
            if(nameParam === "editable")
                main.editable=value

            if(nameParam === "type_dev")
            {
                switch (value)
                {
                default: devName = "";  break;
                case DeviceType.CP100: devName = "CP-100";  break;
                case DeviceType.CP16: devName = "CP-16M";  break;
                case DeviceType.CP16PA: devName = "CP-16PA"; break;
                case DeviceType.CP100PA: devName = "CP-100PA"; break;
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

    function clearHeader()
    {
        main.edit = false;
        main.devName = ""
        main.devVersion = ""
        main.interfaceDescription = ""
    }

    Connections{
        target: InterfaceManager

        function onSgDeviceUnavaliable(senderType, reason)
        {
            msgError.text = qsTr("Device is unavaliable")
            msgError.open();
 //           mBusy.visible = true;
//            mBusy.visible = false;
        }

        function onSgExchangeError()
        {
            msgError.text = qsTr("Command exchange error")
            msgError.open();
//            mBusy.visible = false;
        }

        function onSgInterfaceConnected(interfaceDescription)
        {
            connected = true;
            interfaceDescription = interfaceDescription.address
            mainUi.visible = true;
            msgError.close();
        }

        function onSgInterfaceError(errorDescription)
        {
            clearHeader();
            connected = false;
            main.editable = false;
            mainUi.visible = false;
            msgError.text = qsTr("Device disconnected\n" + errorDescription)
            msgError.open();

            InterfaceManager.startScanning();
        }        

        function onSgInterfaceDisconnected()
        {
            clearHeader();
            connected = false;
            main.editable = false;
            mainUi.visible = false;

            InterfaceManager.startScanning();
        }
    }

    Component.onCompleted: {
        UiSettings.setupApplication();
        InterfaceManager.startScanning();

        if(main.width!==0)
        {
            main.width = UiSettings.windowWidth
            main.height = UiSettings.windowHeight
        }

        main.x = Screen.width/2 - main.width/2;
        main.y = Screen.height/2 - main.height/2
    }

    onClosing: function(close)
    {
        UiSettings.saveSetting("window_width", main.width);
        UiSettings.saveSetting("window_height", main.height);

        UiCore.sw4Enable();

        if(main.edit && main.connected)
        {
            msgPresetChangeSave.saveParam = (-2);
            msgPresetChangeSave.visible = true;
            close.accepted = false;
        }
    }
}