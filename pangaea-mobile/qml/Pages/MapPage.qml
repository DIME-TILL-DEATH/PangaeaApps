import QtQuick 2.15
import QtQuick.Controls 2.15
import StyleSettings 1.0

import ControlGroups 1.0
import Modules 1.0
import Elements 1.0

import CppObjects
import CppEnums

Item
{
    id: _main

    property int countElements: 25
    property int masterControlsHeight: height*5/countElements

    Rectangle
    {
        anchors.fill: parent
        color: Style.colorFon
    }

    // TODO MapCPLegacy.qml
    Item{
        id: _mapContent

        anchors.fill: parent

        PresetsWindow
        {
            id: _presetsList

            topHeaderSize: masterControlsHeight
        }

        Column
        {
            id: _moduleColumn

            width: parent.width*0.98
            height: parent.height
            anchors.horizontalCenter: parent.horizontalCenter

            spacing: 2

            enabled: !_eqExtLoader.visible

            Loader{
                id: _masterControlsLoader

                width:  parent.width
                height: masterControlsHeight
            }
            Connections{
                target: _masterControlsLoader.item

                function onOpenPresetsList() {
                    console.log("Open presets list");
                    _presetsList.open();
                }
            }


            ListView
            {
                id: listViewModules
                width: parent.width
                height: parent.height - masterControlsHeight - _moduleColumn.spacing
                spacing: 2

                interactive: false
                orientation: ListView.Vertical

                move: Transition {
                     NumberAnimation { properties: "y"; duration: 250 }
                }

                displaced: Transition {
                     NumberAnimation { properties: "y"; duration: 250 }
                 }

                delegate: Loader{
                    id: _delegateLoader

                    width: listViewModules.width

                    Component.onCompleted: function(){
                        switch(moduleType)
                        {
                        case ModuleType.NG:
                        {
                            _delegateLoader.source = "../Modules/NG.qml";
                            _delegateLoader.height = _main.height*2/countElements - _moduleColumn.spacing;
                            break;
                        }

                        case ModuleType.CM:
                        {
                            _delegateLoader.source = "../Modules/CM.qml";
                            _delegateLoader.height = _main.height*2/countElements - _moduleColumn.spacing;
                            break;
                        }

                        case ModuleType.PR:
                        {
                            _delegateLoader.source = "../Modules/PR.qml";
                            _delegateLoader.height = _main.height*4/countElements - _moduleColumn.spacing;
                            break;
                        }

                        case ModuleType.PA:
                        {
                            _delegateLoader.source = "../Modules/PA.qml";
                            _delegateLoader.height = _main.height*4/countElements - _moduleColumn.spacing;
                            break;
                        }

                        case ModuleType.IR:
                        {
                            _delegateLoader.source = "../Modules/IR.qml";
                            _delegateLoader.height = _main.height*1/countElements - _moduleColumn.spacing;
                            break;
                        }

                        case ModuleType.HP:
                        {
                            _delegateLoader.source = "../Modules/HP.qml";
                            _delegateLoader.height = _main.height*1/countElements - _moduleColumn.spacing;
                            break;
                        }
                        case ModuleType.EQ:
                        {
                            _delegateLoader.source = "../Modules/EQPreviewLegacy.qml";
                            _delegateLoader.height = _main.height*3/countElements - _moduleColumn.spacing;

                            _eqExtLoader.source = "../Modules/EQExtLegacy.qml";
                            _delegateLoader.item.extVisible.connect(_mapContent.showFullEq);
                            _eqExtLoader.item.hide.connect(_mapContent.hideFullEq);
                            _eqExtLoader.item.eqModule = moduleInstance;
                            break;
                        }
                        case ModuleType.LP:
                        {
                            _delegateLoader.source = "../Modules/LP.qml";
                            _delegateLoader.height = _main.height*1/countElements - _moduleColumn.spacing;
                            break;
                        }

                        case ModuleType.ER:
                        {
                            _delegateLoader.source = "../Modules/ER.qml";
                            _delegateLoader.height = _main.height*2/countElements - _moduleColumn.spacing;
                            break;
                        }
                        }

                        _delegateLoader.item.module = moduleInstance;
                    }
                }
            }
        }

        function showFullEq(){
            _eqExtLoader.visible = true;
        }

        function hideFullEq(){
            _eqExtLoader.visible = false;
        }

        Loader{
            id: _eqExtLoader

            anchors.centerIn: parent

            visible: false

            height: parent.height/1.5
            width:  parent.width*0.95
            z: parent.z+1
        }
    }

    CustomMessageDialog
    {
        id: msgPresetChangeSave

        property int newBank
        property int newPreset

        headerText: qsTr("Save preset")
        text: qsTr("Do you want to save your changes?")

        buttons: Dialog.Yes | Dialog.Discard | Dialog.Cancel

        closeOnDisconnect: true

        onAccepted:
        {
            UiCore.currentDevice.saveChanges();
            UiCore.currentDevice.changePreset(newBank, newPreset, true);
        }
        onDiscarded:
        {
            UiCore.currentDevice.changePreset(newBank, newPreset, true);
            visible = false;
        }
        onRejected:
        {
            // // TODO: more accurate way
            UiCore.currentDevice.restoreValue("bank-preset")
        }
    }

    Connections
    {
        target: UiCore

        function onCurrentDeviceChanged()
        {
            switch(UiCore.currentDevice.deviceClass)
            {
                case DeviceClass.ABSTRACT:
                {
                    listViewModules.model = UiCore.currentDevice.modulesListModel;
                    _masterControlsLoader.sourceComponent = undefined
                    _eqExtLoader.sourceComponent = undefined
                    break;
                }
                case DeviceClass.CP_LEGACY:
                {
                    _masterControlsLoader.source = "../ControlGroups/MasterControls_CP.qml";
                    listViewModules.model = UiCore.currentDevice.modulesListModel;
                    break;
                }
                case DeviceClass.CP_MODERN:
                {
                    _masterControlsLoader.source = "../ControlGroups/MasterControls_CP.qml";
                    listViewModules.model = UiCore.currentDevice.modulesListModel;
                    break;
                }
            }
        }

        function onSgQmlRequestChangePreset(bank, preset)
        {
            if(UiCore.currentDevice.bank !== bank || UiCore.currentDevice.preset !== preset)
            {
                if(UiCore.currentDevice.deviceParamsModified)
                {
                    msgPresetChangeSave.newBank = bank;
                    msgPresetChangeSave.newPreset = preset;
                    msgPresetChangeSave.open();
                }
                else
                {
                    UiCore.currentDevice.changePreset(bank, preset);
                }
            }
        }
    }
}
