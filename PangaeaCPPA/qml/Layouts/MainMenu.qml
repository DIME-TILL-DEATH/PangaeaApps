import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs //1.3

import QtQuick.Window 2.15

import StyleSettings 1.0
import Qt.labs.platform 1.1 as Labs
import Qt.labs.settings 1.0

import CppObjects 1.0

MenuBar{
    id: mainMenu

    property bool presetEdited: true

    Menu{
        title: qsTr("File")
        MenuItem{
            text: qsTr("Import preset")

            onTriggered: importPresetDialog.open();
        }
        MenuItem{
            text: qsTr("Export preset")

            onTriggered:
            {
                if(mainMenu.presetEdited)
                    UiCore.sgSetUIText("preset_not_saved", "");
                else
                    exportPresetDialog.open();
            }
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
        title: qsTr("Interface")

       Menu{
           id: languageMenu
           title: qsTr("Language")

            ActionGroup{
                id: languageGroup
            }

            Action{
                id: menuEn

                text: "English"
                checkable: true
                checked: UiSettings.appLanguageCode === "en"
                ActionGroup.group: languageGroup

                onTriggered: UiSettings.setLanguage("en");
            }
            Action{
                id: menuRu

                text: "Русский"
                checkable: true
                checked: UiSettings.appLanguageCode === "ru"
                ActionGroup.group: languageGroup

                onTriggered: UiSettings.setLanguage("ru");
            }
//            Action{
//                id: menuIt

//                text: "Italiano"
//                checkable: true
//                checked: UiSettings.appLanguageCode === "it"
//                ActionGroup.group: languageGroup

//                onTriggered: UiSettings.setLanguage("it");
//            }
//            Action{
//                id: menuDe

//                text: "Deutsch"
//                checkable: true
//                checked: UiSettings.appLanguageCode === "de"
//                ActionGroup.group: languageGroup

//                onTriggered: UiSettings.setLanguage("de");
//            }
        }

        Menu{
            title: qsTr("Modules direction")
            MenuItem{
                text: qsTr("Left to right")
                checkable: true
                checked: !UiSettings.isModulesRightAligned
                onTriggered: UiSettings.saveSetting("modules_right_aligned", false);
            }
            MenuItem{
                text: qsTr("Right to left")
                checkable: true
                checked: UiSettings.isModulesRightAligned
                onTriggered: UiSettings.saveSetting("modules_right_aligned", true);
            }
        }

        MenuItem{
            id: menuAutoconnect

            text: qsTr("Autoconnect")
            checkable: true
            checked: UiSettings.autoConnectEnabled

            onTriggered: UiSettings.saveSetting("autoconnect_enable", checked);
        }
        MenuItem{
            id: menuCheckUpdates

            text: qsTr("Check updates")
            checkable: true
            checked: UiSettings.checkUpdatesEnabled

            onTriggered: UiSettings.saveSetting("check_updates_enable", checked);
        }
    }

    Menu{
        title: qsTr("Tools")
        MenuItem{
            text: qsTr("IR convertor")

            onTriggered: UiCore.runIrConvertor();
        }

        MenuItem{
            id: menuUpdateFirmware

            text: qsTr("Update firmware")
            enabled: false

            onTriggered: pickFimwareFileDialog.open();
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
         property alias importFolder: importPresetDialog.currentFolder
         property alias exportFolder: exportPresetDialog.currentFolder
    }

    FileDialog{
        id: importPresetDialog

        title: qsTr("Import preset file")

        currentFolder: Labs.StandardPaths.writableLocation(Labs.StandardPaths.DocumentsLocation) + "/AMT/pangaeaCPPA/"

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

    FileDialog{
        id: pickFimwareFileDialog

        title: qsTr("Pick firmware file")

        property string cleanPath

        currentFolder: Labs.StandardPaths.writableLocation(Labs.StandardPaths.DocumentsLocation) + "/AMT/pangaeaCPPA/"

        onAccepted: {
            cleanPath = currentFile.toString();
            cleanPath = (Qt.platform.os==="windows")?decodeURIComponent(cleanPath.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"")):decodeURIComponent(cleanPath.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,""));
            aproveFileDialog.open();
        }
    }

    MessageDialog{
        id: aproveFileDialog

        title: qsTr("Load firmware file")
        text: qsTr("Are you sure want to upload firmware file") + "\n" + pickFimwareFileDialog.cleanPath;

        buttons: MessageDialog.Yes | MessageDialog.No

        onButtonClicked: function (button, role) {
            switch(button){
            case MessageDialog.Yes:
                UiCore.updateFirmware(pickFimwareFileDialog.cleanPath);
                break;
            }
        }
    }

    MessageDialog{
        id: notFwFileDialog

        title: qsTr("Error")
        text: qsTr("Not a fiwmare file!")
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
                    case 0:
                        menuDeviceManual.strManualBaseName = "";
                        menuUpdateFirmware.enabled = false;
                        break;
                    case 1:
                        menuDeviceManual.strManualBaseName = "pangaea-CP-100-user-manual";
                        menuUpdateFirmware.enabled = false;
                        break;
                    case 2:
                        menuDeviceManual.strManualBaseName = "pangaea-VC-16-user-manual";
                        menuUpdateFirmware.enabled = true;
                        break;
                    case 3:
                        menuDeviceManual.strManualBaseName = "pangaea-VC-16-user-manual";
                        menuUpdateFirmware.enabled = true;
                        break;
                    case 4:
                        menuDeviceManual.strManualBaseName = "pangaea-CP-100-user-manual";
                        menuUpdateFirmware.enabled = false;
                        break;
                }
            }

            if(nameParam === "preset_edited")
            {
                mainMenu.presetEdited = value;
            }
        }

        function onSgSetUIText(nameParam, auxText)
        {
            if(nameParam === "not_fw_file_error")
            {
                notFwFileDialog.open();
            }
        }
    }
}
