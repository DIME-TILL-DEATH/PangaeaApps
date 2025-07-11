import QtQuick 2.15
import QtQuick.Controls.Fusion
import QtQuick.Dialogs
import QtCore

import Qt.labs.platform 1.1 as Labs

import QtQuick.Window 2.15

import ModulesClassic 1.0
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

    HeadLegacy
    {
        id: head

        width:  parent.width
        height: parent.height/1000*150

        onSetImpuls: {
            irFileDialog.open();
        }
    }

    Row
    {
        id: _mainRow

        width: parent.width
        height: parent.height/1000*850

        spacing: 2

        property bool isPaFirmware: true
        property int modulesCount: 15
        property bool moduleVisible: false
        property int widthWithoutSpaсe: width - spacing * 11

        function emitIrModule(moduleInstance){
            head.irModule = moduleInstance;
        }

        In{
            id: inp

            height: _mainRow.height
            width:  _mainRow.widthWithoutSpaсe/ _mainRow.modulesCount/2
            // visible: moduleVisible
        }

        ListView{
            id: listViewModules

            width: contentWidth
            height: _mainRow.height

            spacing: _mainRow.spacing

            interactive: false
            orientation: ListView.Horizontal

            layoutDirection:  UiSettings.isModulesRightAligned ? Qt.RightToLeft : Qt.LeftToRight

            model: UiCore.currentDevice.modulesListModel;

            add: Transition{
                NumberAnimation { properties: "x"; duration: 500 }
            }

            move: Transition {
                 NumberAnimation { properties: "x"; duration: 250 }
            }

            displaced: Transition {
                 NumberAnimation { properties: "x"; duration: 250 }
             }

            delegate: Loader{
                id: _delegateLoader

                property int widthMult: 1
                width: _mainRow.widthWithoutSpaсe/_mainRow.modulesCount * widthMult
                height: _mainRow.height

                Component.onCompleted: function(){
                    switch(moduleType)
                    {
                    case ModuleType.NG: _delegateLoader.source = "../ModulesClassic/Ng.qml"; break;
                    case ModuleType.CM: _delegateLoader.source = "../ModulesClassic/Cm.qml"; break;
                    case ModuleType.PR: _delegateLoader.source = "../ModulesClassic/Pr.qml"; break;
                    case ModuleType.PA: _delegateLoader.source = "../ModulesClassic/Pa.qml"; break;
                    case ModuleType.PS: _delegateLoader.source = "../ModulesClassic/Ps.qml"; break;
                    case ModuleType.IR: {
                        _delegateLoader.source = "../ModulesClassic/Ir.qml";
                        _mainRow.emitIrModule(moduleInstance);
                        break;
                    }
                    case ModuleType.HP: _delegateLoader.source = "../ModulesClassic/Hp.qml"; break;
                    case ModuleType.EQ1: {
                        _delegateLoader.source = "../ModulesClassic/EqLegacy.qml";
                        _delegateLoader.widthMult = 5;
                        break;
                    }
                    case ModuleType.LP: _delegateLoader.source = "../ModulesClassic/Lp.qml"; break;
                    case ModuleType.ER_MONO:
                    case ModuleType.ER_STEREO: _delegateLoader.source = "../ModulesClassic/Er.qml"; break;
                    }

                    _delegateLoader.item.module = moduleInstance;
                }
            }
        }

        Vl{
            id: vl
            height: _mainRow.height
            width:  _mainRow.widthWithoutSpaсe/_mainRow.modulesCount
        }

        Out{
            id: outp

            height: _mainRow.height
            width:  _mainRow.widthWithoutSpaсe/_mainRow.modulesCount/2
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

            if(InterfaceManager.connectedInterface.connectionType !== DeviceConnectionType.USB)
            {
                // TODO: переделать cleanPath на QUrl
                UiCore.uploadIr(cleanPath);
            }
        }

        onRejected:
        {
            if(InterfaceManager.connectedInterface.connectionType === DeviceConnectionType.USB)
            {
                UiCore.currentDevice.escImpulse()
            }
        }

        onSelectedFileChanged:
        {

            var cleanPath = irFileDialog.currentFile.toString();
            cleanPath = (Qt.platform.os==="windows")?decodeURIComponent(cleanPath.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"")):decodeURIComponent(cleanPath.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,""));

            if(InterfaceManager.connectedInterface.connectionType === DeviceConnectionType.USB){
                // TODO: переделать cleanPath на QUrl
                UiCore.uploadIr(cleanPath);
            }
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
