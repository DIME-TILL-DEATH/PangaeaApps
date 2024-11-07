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

    property bool moduleVisible: false

    // property string devName: "CP-16"

    property int countElements: 25
    property int masterControlsHeight: height*5/countElements

    Rectangle
    {
        anchors.fill: parent
        color: Style.colorFon
    }

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

                    case ModuleType.EQ:
                    {
                        _delegateLoader.source = "../Modules/EQPreview.qml";
                        _delegateLoader.height = _main.height*3/countElements - _moduleColumn.spacing;

                        _eqExtLoader.source = "../Modules/EQExt.qml";
                        _delegateLoader.item.extVisible.connect(showFullEq);
                        _eqExtLoader.item.hide.connect(hideFullEq);
                        _eqExtLoader.item.eqModule = moduleInstance;
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
        z: _main.z+1

    }

    // Cm
    // {
    //     id: cm
    //     width:  listViewModules.width
    //     height: _main.height*2/countElements - _moduleColumn.spacing
    //     visible: moduleVisible
    // }

    // Pr
    // {
    //     id: pr
    //     width:  listViewModules.width
    //     height: _main.height*4/countElements - _moduleColumn.spacing
    //     visible: moduleVisible
    // }

    // Hp
    // {
    //     id: hp
    //     width:  listViewModules.width
    //     height: _main.height*1/countElements - _moduleColumn.spacing
    //     visible: moduleVisible
    // }


    // Lp
    // {
    //     id: lp
    //     width:  listViewModules.width
    //     height: _main.height*1/countElements - _moduleColumn.spacing
    //     visible: moduleVisible
    // }

    // Er
    // {
    //     id: er
    //     width:  listViewModules.width
    //     height: _main.height*2/countElements - _moduleColumn.spacing
    //     visible: moduleVisible
    // }




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
            // DeviceProperties.saveChanges();
            // DeviceProperties.changePreset(newBank, newPreset, true);
        }
        onDiscarded:
        {
            // UiCore.restoreParameter("impulse")
            // DeviceProperties.changePreset(newBank, newPreset, true);
            // visible = false;
        }
        onRejected:
        {
            // // TODO: change to DeviceParameter
            // UiCore.restoreParameter("preset")
            // UiCore.restoreParameter("bank")
        }
    }

    Connections
    {
        target: UiCore

        function onCurrentDeviceChanged()
        {
                _masterControlsLoader.source = "../ControlGroups/MasterControls_CP.qml";
                listViewModules.model = UiCore.currentDevice.modulesListModel;
        }

        function onSgQmlRequestChangePreset(bank, preset)
        {
            if(UiCore.currentDevice.bank !== bank || UiCore.currentDevice.preset !== preset)
            {
                if(UiCore.currentDevice.deviceParamsModified)
                {
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
