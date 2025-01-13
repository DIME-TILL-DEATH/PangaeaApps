import QtQuick 2.15
import QtQuick.Controls 2.15
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
            height: (parent.height/_mapContent.countElements) * 20 - _moduleColumn.spacing * 4
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

                    case ModuleType.IR:
                    {
                        _delegateLoader.source = "/Modules/qml/Modules/IR.qml";
                        _delegateLoader.height = _main.height*1/countElements - _moduleColumn.spacing;
                        _delegateLoader.item.openIrManagementWindow.connect(_mapContent.showIrManagementWindow);
                        break;
                    }

                    case ModuleType.EQ2:
                    case ModuleType.EQ1:
                    {
                        _delegateLoader.source = "/Modules/qml/Modules/EQPreviewModern.qml";
                        _delegateLoader.height = _main.height*3/countElements - _moduleColumn.spacing;
                        _delegateLoader.item.extVisible.connect(_mapContent.showFullEq);

                        // // TODO fixed item
                        _eqExtLoader.source = "/Modules/qml/Modules/EQExtModern.qml";
                        _delegateLoader.item.extVisible.connect(_mapContent.showFullEq);
                        _eqExtLoader.item.hide.connect(_mapContent.hideFullEq);
                        _eqExtLoader.item.eqModule = moduleInstance;
                        break;
                    }
                    }

                    _delegateLoader.item.module = moduleInstance;
                }
            }
        }

        ER{
            id: _er
            module: UiCore.currentDevice.ER
            width: parent.width
            height: _main.height*2/countElements - _moduleColumn.spacing;
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

        height: parent.height ///1.1
        width:  parent.width*0.95
        z: parent.z+1

        onLoaded:{

        }
    }
}
