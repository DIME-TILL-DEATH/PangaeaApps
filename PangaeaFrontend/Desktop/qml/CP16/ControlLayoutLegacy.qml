import QtQuick 2.15
import QtQuick.Controls.Fusion
import QtQuick.Dialogs
import QtCore

import Qt.labs.platform 1.1 as Labs

import QtQuick.Window 2.15

import ModulesClassic 1.0
import StyleSettings 1.0
import Layouts 1.0

import PangaeaFrontend
import PangaeaBackend

Column
{
    id: mainUi
    anchors.fill: parent
    focus: true
    spacing: 2

    function getModuleComponent(moduleType) {
        switch(moduleType) {
            case ModuleType.NG: return ngComponent;
            case ModuleType.CM: return cmComponent;
            case ModuleType.PR: return prComponent;
            case ModuleType.PA: return paComponent;
            case ModuleType.PS: return psComponent;
            case ModuleType.IR: return irComponent;
            case ModuleType.HP: return hpComponent;
            case ModuleType.EQ1: return eqLegacyComponent;
            case ModuleType.LP: return lpComponent;
            case ModuleType.ER_MONO:
            case ModuleType.ER_STEREO: return erComponent;
            default: return null;
        }
    }

    Component { id: ngComponent; Ng {} }
    Component { id: cmComponent; Cm {} }
    Component { id: prComponent; Pr {} }
    Component { id: paComponent; Pa {} }
    Component { id: psComponent; Ps {} }
    Component { id: irComponent; Ir {} }
    Component { id: hpComponent; Hp {} }
    Component { id: eqLegacyComponent; EqLegacy {} }
    Component { id: lpComponent; Lp {} }
    Component { id: erComponent; Er {} }

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
                    var widthMultiplier = 1;
                    
                    if(moduleType === ModuleType.EQ1) {
                        widthMultiplier = 5;
                    }
                    if(moduleType === ModuleType.IR) {
                        _mainRow.emitIrModule(moduleInstance);
                    }
                    
                    _delegateLoader.widthMult = widthMultiplier;
                    _delegateLoader.sourceComponent = mainUi.getModuleComponent(moduleType);
                    
                    if(_delegateLoader.item) {
                        _delegateLoader.item.module = moduleInstance;
                    }
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
            if(InterfaceManager.connectedInterface.connectionType !== DeviceConnectionType.USB)
            {
                UiCore.uploadIr(irFileDialog.currentFile);
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
            if(InterfaceManager.connectedInterface.connectionType === DeviceConnectionType.USB)
            {
                UiCore.uploadIr(irFileDialog.currentFile);
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
                UiCore.convertAndUploadIr();
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
