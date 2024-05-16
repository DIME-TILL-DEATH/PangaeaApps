import QtQuick
import QtQuick.Controls

import QtQml.Models 2.2

import Modules 1.0
import StyleSettings 1.0

import CppObjects
import CppEnums

Item
{
    id: main

    property bool isPaFirmware: true

    property int modulesCount: 15

    property bool moduleVisible: false

    ListView
    {
        id: listViewModules
        anchors.fill: parent
        spacing: 2
        property int widthWithoutSpase: width-spacing*10

        interactive: false
        orientation: ListView.Horizontal

        layoutDirection:  UiSettings.isModulesRightAligned ? Qt.RightToLeft : Qt.LeftToRight

        model: modulesList

        add: Transition{
            NumberAnimation { properties: "x"; duration: 500 }
        }

        move: Transition {
             NumberAnimation { properties: "x"; duration: 250 }
        }

        displaced: Transition {
             NumberAnimation { properties: "x"; duration: 250 }
         }
    }

    In
    {
        id: inp

        height: listViewModules.height
        width:  listViewModules.widthWithoutSpase/modulesCount/2
        visible: moduleVisible
    }

    Out
    {
        id: outp

        height: listViewModules.height
        width:  listViewModules.widthWithoutSpase/modulesCount/2
        visible: moduleVisible
    }

    Vl
    {
        id: vl
        height: listViewModules.height
        width:  listViewModules.widthWithoutSpase/modulesCount
        visible: moduleVisible
    }

    Er
    {
        id: er
        height: listViewModules.height
        width:  listViewModules.widthWithoutSpase/modulesCount
        visible: moduleVisible
    }

    Ps
    {
        id: ps

        height: listViewModules.height
        width:  listViewModules.widthWithoutSpase/modulesCount
        visible: moduleVisible & (!isPaFirmware)

        //onChPresence: pa.setPresence(value)
    }

    Lp
    {
        id: lp
        height: listViewModules.height
        width:  listViewModules.widthWithoutSpase/modulesCount
        visible: moduleVisible
    }

    EqsMap
    {
        id: eqsMap

        height: listViewModules.height
        width:  listViewModules.widthWithoutSpase/modulesCount*5
        visible: moduleVisible

        property int prePositionIndex: 3
        property int postPositionIndex: 7

        property bool isPrePosition: (ObjectModel.index === prePositionIndex)

    }

    Hp
    {
        id: hp
        height: listViewModules.height
        width:  listViewModules.widthWithoutSpase/modulesCount
        visible: moduleVisible
    }

    Ir
    {
        id: ir
        height: listViewModules.height
        width:  listViewModules.widthWithoutSpase/modulesCount
        visible: moduleVisible
    }

    Pa
    {
        id: pa
        height: listViewModules.height
        width: listViewModules.widthWithoutSpase/modulesCount
        visible: moduleVisible & isPaFirmware

        //onChPresence: ps.setPresence(value)
    }

    Pr
    {
        id: pr
        height: listViewModules.height
        width:  listViewModules.widthWithoutSpase/modulesCount
        visible: moduleVisible
    }

    Cm
    {
        id: cm
        height: listViewModules.height
        width:  listViewModules.widthWithoutSpase/modulesCount
        visible: moduleVisible
    }

    Ng
    {
        id: ng
        height: listViewModules.height
        width:  listViewModules.widthWithoutSpase/modulesCount
        visible: moduleVisible
    }

    ObjectModel
    {
        id: modulesList
    }


    function arrangePrePost(isEqPre)
    {
        if(isPaFirmware)
        {
            if(isEqPre)
            {
                if(!eqsMap.isPrePosition) modulesList.move(eqsMap.postPositionIndex, eqsMap.prePositionIndex, 1);
            }
            else
            {
                if(eqsMap.isPrePosition) modulesList.move(eqsMap.prePositionIndex, eqsMap.postPositionIndex, 1);
            }
        }
    }

    function placeAllModuls()
    {
        modulesList.clear();

        modulesList.append(inp);
        modulesList.append(ng);
        modulesList.append(cm);
        modulesList.append(pr);
        if(isPaFirmware) modulesList.append(pa);
        modulesList.append(ir);
        modulesList.append(hp);
        modulesList.append(eqsMap)
        modulesList.append(lp);
        if(!isPaFirmware) modulesList.append(ps);
        modulesList.append(er);
        modulesList.append(vl);
        modulesList.append(outp);

        moduleVisible = true
        listViewModules.forceLayout();
    }

    Connections
    {
        target: DeviceProperties

        function onDeviceTypeChanged()
        {
            isPaFirmware = ((DeviceProperties.deviceType===DeviceType.CP16PA)||(DeviceProperties.deviceType===DeviceType.CP100PA));
            placeAllModuls();
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
