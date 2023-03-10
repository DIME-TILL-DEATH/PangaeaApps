import QtQuick 2.15//2.12
import QtQuick.Controls 2.15//2.5
//import Qt.labs.platform 1.1 as Labs
//import QtQuick.Dialogs 1.3

import StyleSettings 1.0

import ControlGroups 1.0
import Modules 1.0
import Elements 1.0

Item
{
    id: _main

    // Multiply 16 = shift left for 4 bits
    property int  presetNom: _masterControls.bank.value*16+_masterControls.preset.value

    property string devName: "CP-16"

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

        MasterControls{
            id: _masterControls

            width:  parent.width
            height: masterControlsHeight
        }

        Ng
        {
            width:  parent.width
            height: _main.height*2/countElements - _moduleColumn.spacing
        }
        Cm
        {
            width:  parent.width
            height: _main.height*2/countElements - _moduleColumn.spacing
        }

        Pr
        {
            width:  parent.width
            height: _main.height*4/countElements - _moduleColumn.spacing
        }

        Pa
        {
            id: _paModule

            width: parent.width
            height: _main.height*4/countElements - _moduleColumn.spacing
            visible: true
        }

        //TODO presence в PA это одно и то же. Обрабатывать одним модулем
        Ps
        {
            id: _psModule

            width:  parent.width
            height: _main.height*1/countElements - _moduleColumn.spacing

            visible: false
        }
        Ir
        {
            id: ir
            width:  parent.width
            height: _main.height*1/countElements - _moduleColumn.spacing
        }
        Hp
        {
            width:  parent.width
            height: _main.height*1/countElements - _moduleColumn.spacing
        }

        Eqs
        {
            id: _eqsModule
            eq1: eqsExt.eq1
            eq2: eqsExt.eq2
            eq3: eqsExt.eq3
            eq4: eqsExt.eq4
            eq5: eqsExt.eq5
            q1: eqsExt.q1
            q2: eqsExt.q2
            q3: eqsExt.q3
            q4: eqsExt.q4
            q5: eqsExt.q5
            name: "EQ"
            width:  parent.width
            height: _main.height*3/countElements - _moduleColumn.spacing
            onExtVisible:
            {
                eqsExt.visible = true;
            }
        }
        Lp
        {
            width:  parent.width
            height: _main.height*1/countElements - _moduleColumn.spacing
        }

        Er
        {
            width:  parent.width
            height: _main.height*2/countElements - _moduleColumn.spacing
        }
    }

    EqsExt
    {
        id: eqsExt
        visible: false
        anchors.centerIn: parent
        height: parent.height/2
        width:  parent.width/5*4
        onHide:
        {
            visible = false;
        }
    }

    CustomMessageDialog
    {
        id: msgPresetChangeSave

        // TODO saveParam to enum
        property int saveParam: 0

        headerText: qsTr("Save preset")
        text: qsTr("Do you want to save your changes?")

        buttons: Dialog.Yes | Dialog.Discard | Dialog.Cancel

        closeOnDisconnect: true

        onAccepted:
        {
            _uiCore.setParameter("save_change", saveParam);
            _uiCore.setParameter("do_preset_change", saveParam);
        }
        onDiscarded:
        {
            _uiCore.restoreParameter("impulse")
            _uiCore.setParameter("do_preset_change", saveParam);
            visible = false;
        }
        onRejected:
        {
            saveParam = 0
            _uiCore.restoreParameter("preset")
            _uiCore.restoreParameter("bank")
        }
    }

    Connections
    {
        target: _uiCore

        function onSgPresetChangeStage(inChangePreset)
        {
            msgPresetChangeSave.saveParam = inChangePreset;
            msgPresetChangeSave.open();
        }

        function onSgSetUIParameter(nameParam, inValue)
        {
            if(nameParam === ("type_dev"))
            {
                switch (inValue)
                {
                case 0: devName = "";  break;
                case 1:
                    devName = "CP-100";
                    _paModule.visible=false
                    _psModule.visible=true
                    _eqsModule.isPrePostVisible=false
                    break;
                case 2:
                    devName = "CP-16M";
                    _paModule.visible=false
                    _psModule.visible=true
                    _eqsModule.isPrePostVisible=false
                    break;
                case 3: devName = "CP-16PA";
                    _paModule.visible=true
                    _psModule.visible=false
                    _eqsModule.isPrePostVisible=true
                    break;
                case 4: devName = "CP-100PA";
                    _paModule.visible=true
                    _psModule.visible=false
                    _eqsModule.isPrePostVisible=true
                    break;
                }
            }

            if(nameParam === "open_preset_list")
            {
                console.log("Open presets list");
                _presetsList.open();
            }

        }
    }

    Connections{
        target: _masterControls

        function onImportPreset(){
            _uiCore.importPreset("");
        }

        function onExportPreset(){
            _uiCore.exportPreset("");
        }
    }
}
