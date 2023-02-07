import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs
import Qt.labs.settings 1.0

import QtQuick.Window 2.15

import StyleSettings 1.0
import Layouts 1.0

ApplicationWindow
{
    id: main
    visible: true

    width:  Screen.width * 0.75
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

    title: qsTr("AMT Pangaea " + devName +" (v." + devVersion +") " + " v." + Qt.application.version + " "
                + markConnect + qsTr(" Bank ") + head.bank + qsTr(" Preset ") + head.preset + markEdit)

    Settings
    {
        category: "Current_folder"
        property alias curFolder: irFileDialog.currentFolder
    }

    header: MainMenu{
        visible: main.connected
        enabled: !wait
    }

    ConnectionLayout
    {
        id: connectionUi

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

            onSetImpuls: irFileDialog.open();
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
            _uiCore.setImpuls(cleanPath);
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
                    _uiCore.setParameter("save_change", saveParam);
                    _uiCore.setParameter("do_preset_change", saveParam);
                    break;
                }
                case MessageDialog.No:
                {
                    _uiCore.restoreParameter("impulse")
                    _uiCore.setParameter("do_preset_change", saveParam);
                    break;
                }
                case MessageDialog.Cancel:
                {
                    saveParam = 0
                    _uiCore.restoreParameter("preset")
                    _uiCore.restoreParameter("bank")
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
                _uiCore.convertAndUploadImpulse(cleanPath);
                break;
            }
        }
    }

    MessageDialog
    {
        id: msgError

        title: qsTr("Error")
        text: qsTr("Device is disconnected")
    }

    MessageDialog
    {
        id: _msgVersionError

        title: qsTr("Warning")
        text: qsTr("Version error!")
    }

    MBusy
    {
        id: mBusy
        z:1
    }

    Connections
    {
        target: _uiCore

        function onSgPresetChangeStage(inChangePreset)
        {
            msgPresetChangeSave.saveParam = inChangePreset;
            msgPresetChangeSave.visible = true;
        }

        function onSgSetUIText(nameParam, inString)
        {
            if(nameParam === "port_opened")
            {
                connected = true;
                interfaceDescription = inString
                mainUi.visible = true;
                msgError.close();
            }

            if(nameParam === "port_closed")
            {
                connected = false;
                main.editable = false;
                mainUi.visible = false;
                msgError.text = qsTr("Device disconnected")
                msgError.open();
            }

            if(nameParam === "not_supported_ir")
            {
                msgIncorretIR.text = qsTr("Pangaea doesn't support this wav format:\n") +
                                     inString + "\n" +
                                     qsTr("Do you want to convert it before upload?")
                msgIncorretIR.open();
            }

            if(nameParam === "preset_import_unsuccecfull")
            {
                msgError.text = qsTr("Not a Pangaea preset file!")
                msgError.open();
            }

            if(nameParam === "exchange_error")
            {
                msgError.text = qsTr("Command exchange error")
                msgError.open();
            }

            if(nameParam === "devVersion")
            {
                devVersion = inString;
            }

            if(nameParam === "version_error")
            {
                 var versionArray = inString.split(',');
                _msgVersionError.informativeText = qsTr("Firmware version of your device is ") +
                        versionArray[0] +
                        qsTr("\nMinimum requsted version is ") + versionArray[1]
                _msgVersionError.visible = true;
            }
        }

        function onSgSetUIParameter(nameParam, value)
        {
            if(nameParam === "window_width")
            {
                main.width = value;
                main.x = Screen.width/2 - main.width/2
            }

            if(nameParam === "window_height")
            {
                main.height = value;
                main.y = Screen.height/2 - main.height/2
            }

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
                case 0: devName = "";  break;
                case 1: devName = "CP-100";  break;
                case 2: devName = "CP-16M";  break;
                case 3: devName = "CP-16PA"; break;
                case 4: devName = "CP-100PA"; break;
                }
            }
        }
    }

    Component.onCompleted: {
        _uiCore.setupApplication();
    }

    onClosing: function(close)
    {
        _uiCore.saveSetting("window_width", main.width);
        _uiCore.saveSetting("window_height", main.height);
        _uiCore.saveSetting("modules_right_aligned", Style.modulesRightAligned);


        if(main.edit && main.connected)
        {
            msgPresetChangeSave.saveParam = (-2);
            msgPresetChangeSave.visible = true;
            close.accepted = false;
        }
    }
}

