import QtQuick
import QtQuick.Controls

import QtQml.Models 2.2

import Modules 1.0
import StyleSettings 1.0

import CppObjects
import CppEnums

Row
{
    id: _mainRow

    spacing: 2

    property bool isPaFirmware: true
    property int modulesCount: 15
    property bool moduleVisible: false
    property int widthWithoutSpase: width - spacing * 11

    signal emitIrModule(CabSim moduleInstance);

    In
    {
        id: inp

        height: _mainRow.height
        width:  _mainRow.widthWithoutSpase/modulesCount/2
        // visible: moduleVisible
    }

    ListView
    {
        id: listViewModules

        width: _mainRow.width - inp.width - outp.width - vl.width - spacing * 3
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

            width: listViewModules.width

            Component.onCompleted: function(){
                switch(moduleType)
                {
                case ModuleType.NG:
                {
                    _delegateLoader.source = "/qml/Modules/Ng.qml";
                    _delegateLoader.width = _mainRow.widthWithoutSpase/modulesCount
                    _delegateLoader.height = _mainRow.height
                    break;
                }

                case ModuleType.CM:
                {
                    _delegateLoader.source = "/qml/Modules/Cm.qml";
                    _delegateLoader.width = _mainRow.widthWithoutSpase/modulesCount
                    _delegateLoader.height = _mainRow.height
                    break;
                }

                case ModuleType.PR:
                {
                    _delegateLoader.source = "/qml/Modules/Pr.qml";
                    _delegateLoader.width = _mainRow.widthWithoutSpase/modulesCount
                    _delegateLoader.height = _mainRow.height
                    break;
                }

                case ModuleType.PA:
                {
                    _delegateLoader.source = "/qml/Modules/Pa.qml";
                    _delegateLoader.width = _mainRow.widthWithoutSpase/modulesCount
                    _delegateLoader.height = _mainRow.height
                    break;
                }

                case ModuleType.PS:
                {
                    _delegateLoader.source = "/qml/Modules/Ps.qml";
                    _delegateLoader.width = _mainRow.widthWithoutSpase/modulesCount
                    _delegateLoader.height = _mainRow.height
                    break;
                }

                case ModuleType.IR:
                {
                    _delegateLoader.source = "/qml/Modules/Ir.qml";
                    _delegateLoader.width = _mainRow.widthWithoutSpase/modulesCount
                    _delegateLoader.height = _mainRow.height

                    emitIrModule(moduleInstance);
                    break;
                }

                case ModuleType.HP:
                {
                    _delegateLoader.source = "/qml/Modules/Hp.qml";
                    _delegateLoader.width = _mainRow.widthWithoutSpase/modulesCount
                    _delegateLoader.height = _mainRow.height
                    break;
                }
                case ModuleType.EQ1:
                {
                    _delegateLoader.source = "/qml/Modules/EqParametric.qml";
                    _delegateLoader.width = _mainRow.widthWithoutSpase/modulesCount * 5
                    _delegateLoader.height = _mainRow.height
                    break;
                }
                case ModuleType.LP:
                {
                    _delegateLoader.source = "/qml/Modules/Lp.qml";
                    _delegateLoader.width = _mainRow.widthWithoutSpase/modulesCount
                    _delegateLoader.height = _mainRow.height
                    break;
                }

                case ModuleType.ER_MONO:
                case ModuleType.ER_STEREO:
                {
                    _delegateLoader.source = "/qml/Modules/Er.qml";
                    _delegateLoader.width = _mainRow.widthWithoutSpase/modulesCount
                    _delegateLoader.height = _mainRow.height
                    break;
                }
                }

                _delegateLoader.item.module = moduleInstance;
            }
        }
    }

    Vl
    {
        id: vl
        height: _mainRow.height
        width:  _mainRow.widthWithoutSpase/modulesCount
    }

    Out
    {
        id: outp

        height: _mainRow.height
        width:  _mainRow.widthWithoutSpase/modulesCount/2
    }


    // Ps
    // {
    //     id: ps

    //     height: listViewModules.height
    //     width:  listViewModules.widthWithoutSpase/modulesCount
    //     visible: moduleVisible & (!isPaFirmware)

    //     //onChPresence: pa.setPresence(value)
    // }

    // Lp
    // {
    //     id: lp
    //     height: listViewModules.height
    //     width:  listViewModules.widthWithoutSpase/modulesCount
    //     visible: moduleVisible
    // }

    // EqsMap
    // {
    //     id: eqsMap

    //     height: listViewModules.height
    //     width:  listViewModules.widthWithoutSpase/modulesCount*5
    //     visible: moduleVisible

    //     property int prePositionIndex: 3
    //     property int postPositionIndex: 7

    //     property bool isPrePosition: (ObjectModel.index === prePositionIndex)

    // }

    // Hp
    // {
    //     id: hp
    //     height: listViewModules.height
    //     width:  listViewModules.widthWithoutSpase/modulesCount
    //     visible: moduleVisible
    // }

    // Ir
    // {
    //     id: ir
    //     height: listViewModules.height
    //     width:  listViewModules.widthWithoutSpase/modulesCount
    //     visible: moduleVisible
    // }

    // Pa
    // {
    //     id: pa
    //     height: listViewModules.height
    //     width: listViewModules.widthWithoutSpase/modulesCount
    //     visible: moduleVisible & isPaFirmware

    //     //onChPresence: ps.setPresence(value)
    // }

    // Pr
    // {
    //     id: pr
    //     height: listViewModules.height
    //     width:  listViewModules.widthWithoutSpase/modulesCount
    //     visible: moduleVisible
    // }

    Connections
    {
        target: UiCore

        function onCurrentDeviceChanged()
        {
            switch(UiCore.currentDevice.deviceType)
            {
                // case DeviceType.LA3:
                // {
                //     _masterControlsLoader.source = "/ControlGroups/qml/ControlGroups/MasterControls_LA.qml";
                //     _masterControlsLoader.item.openPresetsList.connect(_presetsList.open);
                //     setMapContent();
                //     break;
                // }
                // default:
                // {
                //     _masterControlsLoader.source = "/ControlGroups/qml/ControlGroups/MasterControls_CP.qml";
                //     _masterControlsLoader.item.openPresetsList.connect(_presetsList.open);
                //     setMapContent();
                //     break;
                // }
            }
        }
    }
    // Connections{
    //     target: UiCore.currentDevice.modulesListModel

    //     function onModelReset(){
    //         listViewModules.item.modulesModel = undefined // без этого пропускает первое обновление
    //         listViewModules.item.modulesModel = UiCore.currentDevice.modulesListModel
    //     }
    // }

}
