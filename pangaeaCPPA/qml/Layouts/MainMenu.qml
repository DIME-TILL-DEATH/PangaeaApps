import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs //1.3

import QtQuick.Window 2.15

import StyleSettings 1.0
import Qt.labs.platform 1.1 as Labs
import Qt.labs.settings 1.0

import CppObjects 1.0

MenuBar{
    Menu{
        title: qsTr("File")
        MenuItem{
            text: qsTr("Import preset")

            onTriggered: importPresetDialog.open();
        }
        MenuItem{
            text: qsTr("Export preset")

            onTriggered: exportPresetDialog.open();
        }
        MenuSeparator{}
        MenuItem{
            text: qsTr("Disconnect from device")

            onTriggered: disconnectDialog.open();
        }
        MenuSeparator{}
        MenuItem{
            text: qsTr("Quit")
            onTriggered: {Window.window.close()}
        }
    }

    Menu{
        title: qsTr("View")

        Menu{
            title: qsTr("Modules direction")
            MenuItem{
                text: qsTr("Left to right")
                checkable: true
                checked: !Style.modulesRightAligned
                onTriggered: Style.modulesRightAligned = false;
            }
            MenuItem{
                text: qsTr("Right to left")
                checkable: true
                checked: Style.modulesRightAligned
                onTriggered: Style.modulesRightAligned = true;
            }
        }
    }

    Menu{
        title: qsTr("Tools")
        MenuItem{
            text: qsTr("IR convertor")

            onTriggered: UiCore.runIrConvertor();
        }
    }

    Menu{
        title: qsTr("Help")

        MenuItem{
            text: qsTr("AMT web site")

            onTriggered: Qt.openUrlExternally("http://www.amtelectronics.com/");
        }

        MenuItem{
            text: qsTr("Pangaea web page")

            onTriggered: Qt.openUrlExternally("https://media.amt-sales.com/cat/cab-emulation/pangaea-series/")
        }

        MenuItem{
            id: menuDeviceManual

            property string strManualBaseName: ""

            text: qsTr("Device manual")

            onTriggered: {
                UiCore.openManualExternally(strManualBaseName)
            }
        }
        MenuSeparator{}

        MenuItem{
            text: qsTr("About...")
            onTriggered: aboutDialog.open()
        }
    }

    Settings
    {
        category: "Current_folder"
       // property alias importFolder: importPresetDialog.folder
      //  property alias exportFolder: exportPresetDialog.folder
         property alias importFolder: importPresetDialog.currentFolder
         property alias exportFolder: exportPresetDialog.currentFolder
    }

    FileDialog{
        id: importPresetDialog

        title: qsTr("Import preset file")

//        folder: Labs.StandardPaths.writableLocation(Labs.StandardPaths.DocumentsLocation) + "/AMT/pangaeaCPPA/"
        currentFolder: Labs.StandardPaths.writableLocation(Labs.StandardPaths.DocumentsLocation) + "/AMT/pangaeaCPPA/"

        //selectMultiple: false
        //fileMode: FileDialog.OpenFile

        onAccepted: {
            var cleanPath = currentFile.toString();//fileUrl.toString();
            cleanPath = (Qt.platform.os==="windows")?decodeURIComponent(cleanPath.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"")):decodeURIComponent(cleanPath.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,""));
            UiCore.importPreset(cleanPath);
        }
    }

    FileDialog{
        id: exportPresetDialog

        title: qsTr("Export preset file")
        nameFilters: [ "Pangaea preset files (*.pst)" ]

        currentFolder: Labs.StandardPaths.writableLocation(Labs.StandardPaths.DocumentsLocation) + "/AMT/pangaeaCPPA/"

        fileMode: FileDialog.SaveFile

        onAccepted: {
            var cleanPath = currentFile.toString();//fileUrl.toString();
            cleanPath = (Qt.platform.os==="windows")?decodeURIComponent(cleanPath.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"")):decodeURIComponent(cleanPath.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,""));
            UiCore.exportPreset(cleanPath);
        }
    }

    MessageDialog{
        id: disconnectDialog

        title: qsTr("Disconnect?")
        text: qsTr("Are you sure want to disconnect?")

        buttons: MessageDialog.Yes | MessageDialog.No

        onButtonClicked: function (button, role) {
            switch(button){
            case MessageDialog.Yes:
                InterfaceManager.disconnectFromDevice();
                break;
            }
        }
    }

    MessageDialog{
        id: aboutDialog

        title: qsTr("About...")
        text: qsTr("AMT Pangaea CP-16/CP-100")
        informativeText: qsTr("Desktop application") + "\n" +
              qsTr("Version: ") + Qt.application.version + "\n"
              + qsTr("(c) 2023")

    }

    Connections{
        target: UiCore

        function onSgSetUIParameter(nameParam, value)
        {
            if(nameParam === "type_dev")
            {
                switch (value)
                {
                    case 0: menuDeviceManual.strManualBaseName = "";  break;
                    case 1: menuDeviceManual.strManualBaseName = "pangaea-CP-100-user-manual";  break;
                    case 2: menuDeviceManual.strManualBaseName = "pangaea-VC-16-user-manual";  break;
                    case 3: menuDeviceManual.strManualBaseName = "pangaea-VC-16-user-manual"; break;
                    case 4: menuDeviceManual.strManualBaseName = "pangaea-CP-100-user-manual"; break;
                }

                console.log("Settling device path:", value, menuDeviceManual.strManualBaseName)
            }
        }
    }
}
