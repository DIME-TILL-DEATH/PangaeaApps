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

    function showFullEq(){
        console.log("Request to show BIG eq")
        eqsExt.visible = true;

    }

    Column
    {
        id: _moduleColumn

        width: parent.width*0.98
        height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter

        spacing: 2

        enabled: !eqsExt.visible

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
                    console.log("Module type:", moduleType)
                    switch(moduleType)
                    {
                    case ModuleType.PA:
                    {
                        console.log(moduleInstance)
                        _delegateLoader.source = "../Modules/Pa.qml";
                        _delegateLoader.height = _main.height*4/countElements - _moduleColumn.spacing;
                        _delegateLoader.item.module = moduleInstance;


                        break;
                    }

                    case ModuleType.EQ:
                    {
                        _delegateLoader.source = "../Modules/EqPreview.qml"
                        _delegateLoader.height = _main.height*3/countElements - _moduleColumn.spacing

                        _delegateLoader.item.extVisible.connect(showFullEq)
                        break;
                    }
                    }
                }
            }
        }
    }

    ObjectModel
    {
        id: modulesList
    }

    // Ng
    // {
    //     id: ng
    //     width:  listViewModules.width
    //     height: _main.height*2/countElements - _moduleColumn.spacing
    //     visible: moduleVisible
    // }
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


    // Ps
    // {
    //     id: ps

    //     width:  listViewModules.width
    //     height: _main.height*1/countElements - _moduleColumn.spacing

    //     visible: moduleVisible & (!DeviceProperties.isPaFirmware)
    // }
    // Ir
    // {
    //     id: ir
    //     width:  listViewModules.width
    //     height: _main.height*1/countElements - _moduleColumn.spacing
    //     visible: moduleVisible
    // }
    // Hp
    // {
    //     id: hp
    //     width:  listViewModules.width
    //     height: _main.height*1/countElements - _moduleColumn.spacing
    //     visible: moduleVisible
    // }

    // EqPreview
    // {
    //     id: eq

    //     width:  listViewModules.width
    //     height: _main.height*3/countElements - _moduleColumn.spacing

    //     property int prePositionIndex: 2
    //     property int postPositionIndex: 6
    //     isPreEQ: (ObjectModel.index === prePositionIndex)
    //     isPrePostVisible: DeviceProperties.isPaFirmware

    //     onExtVisible:
    //     {
    //         eqsExt.visible = true;
    //     }
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

    function arrangePrePost(value_isEqPre)
    {
        // if(DeviceProperties.isPaFirmware)
        // {
        //     if(value_isEqPre)
        //     {
        //         if(!eq.isPreEQ) modulesList.move(eq.postPositionIndex, eq.prePositionIndex, 1);
        //     }
        //     else
        //     {
        //         if(eq.isPreEQ) modulesList.move(eq.prePositionIndex, eq.postPositionIndex, 1);
        //     }
        // }
        // listViewModules.forceLayout();
    }

    function placeAllModuls()
    {
        // modulesList.clear();

        // modulesList.append(ng);
        // modulesList.append(cm);
        // modulesList.append(pr);
        // if(DeviceProperties.isPaFirmware) modulesList.append(pa);
        // modulesList.append(ir);
        // modulesList.append(hp);
        // modulesList.append(eq)
        // modulesList.append(lp);
        // if(!DeviceProperties.isPaFirmware) modulesList.append(ps);
        // modulesList.append(er);

        // moduleVisible = true;
        // listViewModules.forceLayout();
    }

    // EqsExt
    // {
    //     id: eqsExt
    //     visible: false

    //     height: parent.height/1.5
    //     width:  parent.width*0.95
    //     z: _main.z+1
    //     onHide:
    //     {
    //         visible = false;
    //     }
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
            DeviceProperties.saveChanges();
            DeviceProperties.changePreset(newBank, newPreset, true);
        }
        onDiscarded:
        {
            UiCore.restoreParameter("impulse")
            DeviceProperties.changePreset(newBank, newPreset, true);
            visible = false;
        }
        onRejected:
        {
            // TODO: change to DeviceParameter
            UiCore.restoreParameter("preset")
            UiCore.restoreParameter("bank")
        }
    }

    Connections
    {
        target: DeviceProperties

        function onDeviceTypeChanged()
        {
            _masterControlsLoader.source = "../ControlGroups/MasterControls_CP.qml"
            listViewModules.model = ModulesListModel
        }

        function onPresetNotSaved(bank, preset)
        {
            msgPresetChangeSave.newBank = bank;
            msgPresetChangeSave.newPreset = preset;
            msgPresetChangeSave.visible = true;
        }
    }

    Connections
    {
        target: UiCore

        function onSgSetUiDeviceParameter(paramType, value)
        {
            switch(paramType)
            {
            case DeviceParameter.EQ_PRE:
            {
                arrangePrePost(value);
                break;
            }
            }
        }

        // not set ui
        function onSgSetDeviceParameter(paramType, value)
        {
            if(paramType === DeviceParameter.EQ_PRE)
            {
                arrangePrePost(value);
            }
        }
    }

    Connections{
        target: InterfaceManager

        function onSgInterfaceError(errorDescription)
        {
            modulesList.clear();
            listViewModules.forceLayout();
        }

        function onSgInterfaceDisconnected()
        {
            modulesList.clear();
            listViewModules.forceLayout();
        }
    }
}
