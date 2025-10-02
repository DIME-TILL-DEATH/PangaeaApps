import QtQuick 2.15
import QtQuick.Controls 2.15

import Elements 1.0
import Layouts 1.0

import StyleSettings 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property DualCabSim module: _moduleLoader.selectedModuleInstance

    FileBrowserWindow{
        id: _fileBrowser
    }


    contentItem: Row
    {
        id: _contentItem

        property alias currentIndex: _chooseCabCombo.currentIndex

        anchors.fill: parent

        onVisibleChanged: {
            if(UiCore.currentDevice.systemSettings.cabNumber !== 2)
            {
                _chooseCabCombo.currentIndex = 0;
            }
        }

        Connections{
            target: _fileBrowser

            function onCabNumChanged(index){
                _chooseCabCombo.currentIndex = index
            }
        }

        MComboVertical{
            id: _chooseCabCombo

            text: qsTr("Cab num")

            visible: UiCore.currentDevice.systemSettings.cabNumber === 2

            width: main.dialWidth
            height: main.dialHeight

            opacity: module.moduleEnabled ? 1.0 : 0.5

            model: ["1", "2"]

            currentIndex: 0

            onActivated: {
                _fileBrowser.currentCabNum = _chooseCabCombo.currentIndex
            }
        }


        Rectangle{
            width: 1
            height: parent.height

            visible: UiCore.currentDevice.systemSettings.cabNumber === 2
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: (_chooseCabCombo.currentIndex === 0) ? module.firstCabVolume : module.secondCabVolume
        }


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            visible: UiCore.currentDevice.systemSettings.cabNumber === 2

            controlValue: (_chooseCabCombo.currentIndex === 0) ? module.firstCabPan : module.secondCabPan
        }

        Rectangle{
            width: 1
            height: parent.height
        }

        Item{
            width: main.dialWidth * 2
            height: parent.height

            MButton{
                id: _showBrowser

                text: "Browser"

                width: parent.width * 0.75
                height: dialHeight/2

                anchors.centerIn: parent

                onClicked:{
                    _fileBrowser.show();
                }
            }
        }

        Rectangle{
            width: 1
            height: parent.height
        }

        Item{
            width: parent.width - main.dialWidth * 6
            height: parent.height
            MText{
                anchors.fill: parent
                anchors.leftMargin: 8

                verticalAlignment: Text.AlignVCenter

                property string currentIrName: _chooseCabCombo.currentIndex === 0 ? UiCore.currentDevice.ir1Name : UiCore.currentDevice.ir2Name

                text: qsTr("Current IR: ") + (currentIrName === qsTr("Empty") ? "" : currentIrName)
            }
        }
    }
}
