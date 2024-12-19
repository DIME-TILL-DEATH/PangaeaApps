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

        MasterControls_CP{
            id: _masterControlsLoader

            width:  parent.width
            height: masterControlsHeight
        }

        Connections{
            target: _masterControlsLoader //.item

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

            model: UiCore.currentDevice.modulesListModel;

            interactive: false
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

                    case ModuleType.PS:
                    {
                        _delegateLoader.source = "/Modules/qml/Modules/PS.qml";
                        _delegateLoader.height = _main.height*1/countElements - _moduleColumn.spacing;
                        break;
                    }

                    case ModuleType.IR:
                    {
                        _delegateLoader.source = "/Modules/qml/Modules/IR.qml";
                        _delegateLoader.height = _main.height*1/countElements - _moduleColumn.spacing;
                        break;
                    }

                    case ModuleType.HP:
                    {
                        _delegateLoader.source = "/Modules/qml/Modules/HP.qml";
                        _delegateLoader.height = _main.height*1/countElements - _moduleColumn.spacing;
                        break;
                    }
                    case ModuleType.EQ:
                    {
                        _delegateLoader.source = "/Modules/qml/Modules/EQPreviewLegacy.qml";
                        _delegateLoader.height = _main.height*3/countElements - _moduleColumn.spacing;

                        _eqExtLoader.source = "/Modules/qml/Modules/EQExtLegacy.qml";
                        _delegateLoader.item.extVisible.connect(_mapContent.showFullEq);
                        _eqExtLoader.item.hide.connect(_mapContent.hideFullEq);
                        _eqExtLoader.item.eqModule = moduleInstance;
                        break;
                    }
                    case ModuleType.LP:
                    {
                        _delegateLoader.source = "/Modules/qml/Modules/LP.qml";
                        _delegateLoader.height = _main.height*1/countElements - _moduleColumn.spacing;
                        break;
                    }

                    case ModuleType.ER:
                    {
                        _delegateLoader.source = "/Modules/qml/Modules/ER.qml";
                        _delegateLoader.height = _main.height*2/countElements - _moduleColumn.spacing;
                        break;
                    }
                    }

                    _delegateLoader.item.module = moduleInstance;
                }
            }
        }
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

        height: parent.height/1.2
        width:  parent.width*0.95
        z: parent.z+1
    }
}
