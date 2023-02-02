import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
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

    color: "#EBECEC"

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
        property alias curFolder: fileOpen.folder
    }

    header: MainMenu{
        visible: main.connected
        enabled: !wait
    }

    Column
    {
        anchors.fill: parent
        focus: true
        spacing: 2

        Head
        {
            id: head

            width:  parent.width
            height: parent.height/1000*150

            onSetImpuls: fileOpen.open();
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

//    Timer
//    {
//        id: timer

//        interval: 250
//        repeat: true
//        running: fileOpen.visible

//        property string lastSelectFile: ""

//        onTriggered:
//        {
//            if(lastSelectFile!=fileOpen.fileUrl)
//            {
//                var cleanPath;
//                lastSelectFile=fileOpen.fileUrl;
//                cleanPath = (Qt.platform.os=="windows")?decodeURIComponent(lastSelectFile.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"")):decodeURIComponent(lastSelectFile.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,""));
//                _uiCore.setImpuls(cleanPath);
//            }
//        }
//    }

    FileDialog
    {
        id: fileOpen

        title: qsTr("Select IR")
        nameFilters: [ "Wav files (*.wav)" ]

        onAccepted:
        {
            //TODO нужно ли?
            //modules.irEnable(true);
            var cleanPath = fileOpen.fileUrl.toString();
            //lastSelectFile=fileOpen.fileUrl;
            cleanPath = (Qt.platform.os=="windows")?decodeURIComponent(cleanPath.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"")):decodeURIComponent(cleanPath.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,""));
            _uiCore.setImpuls(cleanPath);
        }
        onRejected:
        {
            //_uiCore.escImpuls();
        }
    }

    MessageDialog
    {
        id: msgPresetChangeSave

        property int saveParam: 0

        icon: StandardIcon.Question
        text: qsTr("Do you want to save changes?")
        title: qsTr("Save preset")

        // TODO standardButtons resize bug in qml
        standardButtons: MessageDialog.Save | MessageDialog.No | MessageDialog.Cancel
        onAccepted:
        {
            _uiCore.setParameter("save_change", saveParam);
            _uiCore.setParameter("do_preset_change", saveParam);
        }
        onNo:
        {
            _uiCore.restoreParameter("impulse")
            _uiCore.setParameter("do_preset_change", saveParam);
        }
        onRejected:
        {
            saveParam = 0
            _uiCore.restoreParameter("preset")
            _uiCore.restoreParameter("bank")
        }
    }

    MessageDialog{
        id: msgIncorretIR

        icon: StandardIcon.Warning
        title: qsTr("Incorrect wav format")

        standardButtons: MessageDialog.Yes | MessageDialog.No

        onYes: {

            var cleanPath = fileOpen.fileUrl.toString();
            cleanPath = (Qt.platform.os=="windows")?decodeURIComponent(cleanPath.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"")):decodeURIComponent(cleanPath.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,""));

            console.log("accepted, path", cleanPath);
            _uiCore.convertAndUploadImpulse(cleanPath);
        }
    }

    MessageDialog
    {
        id: msgError

        icon: StandardIcon.Critical
        title: qsTr("Error")
        text: qsTr("Device is disconnected")
    }

    MessageDialog
    {
        id: _msgVersionError

        icon: StandardIcon.Warning
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
                msgError.close();
            }

            if(nameParam === "port_closed")
            {
                connected = false;
                main.editable = false;
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
            }

            if(nameParam === "window_height")
            {
                main.height = value;
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

    onClosing:
    {
        _uiCore.saveSetting("window_width", main.width);
        _uiCore.saveSetting("window_height", main.height);
        _uiCore.saveSetting("modules_right_aligned", Style.modulesRightAligned);

        _uiCore.sw4Enable();

        if(main.edit && main.connected)
        {
            msgPresetChangeSave.saveParam = (-2);
            msgPresetChangeSave.visible = true;
            close.accepted = false;
        }
    }
}

