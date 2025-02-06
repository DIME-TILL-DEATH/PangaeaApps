import QtQuick 2.15
import QtQuick.Controls.Fusion
import QtQuick.Dialogs
import QtCore

import Qt.labs.platform 1.1 as Labs

import QtQuick.Window 2.15

import Modules 1.0
import StyleSettings 1.0
import Layouts 1.0

import CppObjects
import PangaeaBackend

Column
{
    id: mainUi
    anchors.fill: parent
    focus: true
    spacing: 2

    ModulesConfigWindow{
        id: _modulesConfigWindow
    }

    IrManagerWindow{
        id: _irManagerWindow

        onUploadFileReq: {
            irFileDialog.open();
        }
    }

    HeadCPModern
    {
        id: head

        width:  parent.width
        height: parent.height/1000*150

        onOpenIrManagerWindow: {
            _irManagerWindow.show();
            console.log("Models:", UiCore.currentDevice.irsInLibrary, UiCore.currentDevice.irsInFolder)
        }

        onOpenModulesConfigWindow: {
            _modulesConfigWindow.show();
        }
    }

    ModulesListModern
    {
        id: _mainRow

        width: parent.width
        height: parent.height/1000*850

        onEmitIrModule: moduleInstance => {
            head.irModule = moduleInstance;
        }
    }


    FileDialog
    {
        id: irFileDialog

        title: qsTr("Select IR")
        nameFilters: [ "Wav files (*.wav)" ]

        onAccepted:
        {
            // TODO: переделать cleanPath на QUrl
            var cleanPath = irFileDialog.currentFile.toString();
            cleanPath = (Qt.platform.os==="windows")?decodeURIComponent(cleanPath.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"")):decodeURIComponent(cleanPath.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,""));

            UiCore.uploadIr(cleanPath, _irManagerWindow.dstIrPath);

        }

        Settings
        {
            category: "Current_folder"
            property alias curFolder: irFileDialog.currentFolder
        }
    }

    MessageDialog{
        id: msgIncorretIR

        title: qsTr("Incorrect wav format")

        buttons: MessageDialog.Yes | MessageDialog.No

        onButtonClicked: function (button, role) {
            switch(button){
            case MessageDialog.Yes:
                // TODO: переделать cleanPath на QUrl
                var cleanPath = irFileDialog.currentFile.toString();
                cleanPath = (Qt.platform.os==="windows")?decodeURIComponent(cleanPath.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"")):decodeURIComponent(cleanPath.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,""));
                UiCore.convertAndUploadIr(cleanPath);
                break;
            }
        }
    }


    Connections{
        target: UiCore.currentDevice

        function onSgDeviceError(type, description, params)
        {
            switch(type)
            {
                case DeviceErrorType.IrFormatNotSupported:
                {
                    msgIncorretIR.text = qsTr("Pangaea doesn't support this wav format:") + "\n" +
                                         description + "\n" +
                                         qsTr("Do you want to convert it before upload?")
                    msgIncorretIR.open();
                    break;
                }
            }
        }
    }
}
