import QtQuick 2.15
import QtCore

import StyleSettings 1.0

import CustomOverlays 1.0
import ControlGroups 1.0
import Modules 1.0
import Elements 1.0

import CppObjects
import CppEnums

Item{
    id: _mapContent

    property int countElements: 25
    property int masterControlsHeight: height*5/countElements

    property alias modulesModel: listViewModules.model

    IrManagementWindow{
        id: _irManagement

        visible: false
    }

    Column
    {
        id: _moduleColumn

        width: parent.width*0.98
        height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter

        spacing: 2

        enabled: !_eqExtLoader.visible


        Rectangle {
            id: _presetNameContainer

            width: parent.width
            height: _main.height/countElements - _moduleColumn.spacing
            color: Style.colorModul

            clip: true

            radius: Style.baseRadius
            border.width: 1
            border.color: Style.currentTheme.colorBorderOn

            Item {
                id: _contentItem

                anchors.fill: parent
                anchors.rightMargin: parent.width/50
                anchors.leftMargin: parent.width/50
                anchors.bottomMargin: 3
                anchors.topMargin: 3

                MText{
                    width: parent.width * 0.9
                    elide: Text.ElideRight
                    text: qsTr("Preset name: ") + UiCore.currentDevice.currentPresetName
                    color: Style.colorText
                }
            }

            MouseArea{
                anchors.fill: parent

                onClicked: _presetNameDialog.open();
            }

            EditTextDialog
            {
                id: _presetNameDialog

                contentText: UiCore.currentDevice.currentPresetName

                headerText: qsTr("Set preset name")
                text: qsTr("Set name for current preset:")

                onAccepted:
                {
                    UiCore.currentDevice.currentPresetName = contentText;
                }
                onRejected:
                {
                    contentText = UiCore.currentDevice.currentPresetName;
                }
            }
        }

        ListView
        {
            id: listViewModules
            width: parent.width
            // height: (parent.height/_mapContent.countElements) * 20 - _moduleColumn.spacing * 4
            height: _mapContent.height - _presetNameContainer.height
                    - _reverbSectionConatiner.height - _clipIndContainer.height - _moduleColumn.spacing * 4
            spacing: 2

            // interactive: false
            orientation: ListView.Vertical

            clip: true

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
                        _delegateLoader.source = "/Modules/qml/Modules/NG.qml";
                        _delegateLoader.height = _main.height*2/countElements - _moduleColumn.spacing;
                        break;
                    }

                    case ModuleType.CM:
                    {
                        _delegateLoader.source = "/Modules/qml/Modules/CM.qml";
                        _delegateLoader.height = _main.height*2/countElements - _moduleColumn.spacing;
                        break;
                    }

                    case ModuleType.PR:
                    {
                        _delegateLoader.source = "/Modules/qml/Modules/PR.qml";
                        _delegateLoader.height = _main.height*4/countElements - _moduleColumn.spacing;
                        break;
                    }

                    case ModuleType.PA:
                    {
                        _delegateLoader.source = "/Modules/qml/Modules/PA.qml";
                        _delegateLoader.height = _main.height*4/countElements - _moduleColumn.spacing;
                        break;
                    }

                    case ModuleType.TR:
                    {
                        _delegateLoader.source = "/Modules/qml/Modules/TR.qml";
                        _delegateLoader.height = _main.height*2/countElements - _moduleColumn.spacing;
                        break;
                    }

                    case ModuleType.CH:
                    {
                        _delegateLoader.source = "/Modules/qml/Modules/CH.qml";
                        _delegateLoader.height = _main.height*4/countElements - _moduleColumn.spacing;
                        break;
                    }

                    case ModuleType.PH:
                    {
                        _delegateLoader.source = "/Modules/qml/Modules/PH.qml";
                        _delegateLoader.height = _main.height*7/countElements - _moduleColumn.spacing;
                        break;
                    }

                    case ModuleType.IR:
                    {
                        _delegateLoader.source = "/Modules/qml/Modules/IR.qml";
                        _delegateLoader.height = _main.height*1/countElements - _moduleColumn.spacing;
                        _delegateLoader.item.openIrManagementWindow.connect(_mapContent.showIrManagementWindow);
                        break;
                    }

                    case ModuleType.EQ1:
                    {
                        _delegateLoader.source = "/Modules/qml/Modules/EQPreviewModern.qml";
                        _delegateLoader.height = _main.height*3/countElements - _moduleColumn.spacing;
                        _delegateLoader.item.extVisible.connect(_mapContent.showFullEq1);
                        // _eqExtLoader.item.hide.connect(_mapContent.hideFullEq);

                        // _eqExtLoader.eq1Instance = moduleInstace
                        // _eqExtLoader.source = "/Modules/qml/Modules/EQExtModern.qml";
                        _eqExtLoader.eq1Instance = moduleInstance;
                        break;
                    }

                    case ModuleType.EQ2:
                    {
                        _delegateLoader.source = "/Modules/qml/Modules/EQPreviewModern.qml";
                        _delegateLoader.height = _main.height*3/countElements - _moduleColumn.spacing;
                        _delegateLoader.item.extVisible.connect(_mapContent.showFullEq2);
                        // _eqExtLoader.item.hide.connect(_mapContent.hideFullEq);

                        _eqExtLoader.eq2Instance = moduleInstance;
                        break;
                    }
                    }

                    _delegateLoader.item.module = moduleInstance;
                }
            }
        }

        Column{
            id: _reverbSectionConatiner

            width: parent.width
            // height: _dlRvContainer.enabled ?  _main.height*8/countElements : _main.height*1/countElements
            height: _dlRvContainer.enabled ?  _hdrSpatial.height + spacing + _dlRvContainer.height
                                           : _main.height*1/countElements

            spacing: 2

            Rectangle {
                id: _hdrSpatial

                color: Style.colorModul

                clip: true

                radius: Style.baseRadius
                border.width: 1
                border.color: Style.currentTheme.colorBorderOn

                width: parent.width
                height: _main.height*1/countElements

                MText{
                    anchors.centerIn: parent
                    text: (_dlRvContainer.enabled ? "(-)" : "(+)") + "Delay/Reverb"
                    color: Style.colorText
                }

                MouseArea{
                    anchors.fill: parent

                    onClicked: {
                        _dlRvContainer.enabled = !_dlRvContainer.enabled
                    }

                    Settings{

                    }
                }
            }

            Column{
                id: _dlRvContainer

                spacing: 2

                enabled: false

                width: parent.width
                height: _dl.height + _er.height + spacing


                DL{
                    id: _dl
                    module: UiCore.currentDevice.DL
                    width: parent.width
                    height: UiCore.currentDevice.DL.used ? _main.height*5/countElements - _moduleColumn.spacing : 0

                    visible: UiCore.currentDevice.DL.used
                }

                ER{
                    id: _er
                    module: UiCore.currentDevice.ER
                    width: parent.width
                    height: UiCore.currentDevice.ER.used ? _main.height*2/countElements - _moduleColumn.spacing : 0
                    visible: UiCore.currentDevice.ER.used
                }

                Settings{
                    property alias spatialSettingsView: _dlRvContainer.enabled
                }
            }

            Behavior on height{
                NumberAnimation { duration: 200 }
            }
        }

        ClipIndicator {
            id: _clipIndContainer

            width: parent.width
            height: _main.height/countElements  - _moduleColumn.spacing
        }
    }

    function showIrManagementWindow()
    {
        _irManagement.visible = true;
    }

    function centerViewAt(index)
    {
        listViewModules.positionViewAtIndex(index, ListView.Center)
    }

    function showFullEq1(){
        _eqExtLoader.eqModule = _eqExtLoader.eq1Instance;
        _eqExtLoader.active = true;
        // _eqExtLoader.item.update();
        _eqExtLoader.visible = true;
    }

    function showFullEq2(){
        _eqExtLoader.eqModule = _eqExtLoader.eq2Instance;
        _eqExtLoader.active = true;
        // _eqExtLoader.item.update();
        _eqExtLoader.visible = true;
    }

    function hideFullEq(){
        _eqExtLoader.visible = false;
        _eqExtLoader.active =  false;
    }

    Loader{
        id: _eqExtLoader

        property EqParametric eq1Instance
        property EqParametric eq2Instance

        property EqParametric eqModule

        source: "/Modules/qml/Modules/EQExtModern.qml";

        anchors.centerIn: parent

        active: false
        visible: false

        height: parent.height ///1.1
        width:  parent.width*0.95
        z: parent.z+1

        onLoaded:{

        }

        Connections{
            target: _eqExtLoader.item

            function onHide()
            {
                _mapContent.hideFullEq();
            }
        }
    }
}
