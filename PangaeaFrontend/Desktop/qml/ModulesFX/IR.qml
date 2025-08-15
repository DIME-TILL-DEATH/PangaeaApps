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
                _chooseCabCombo,currentIndex = index
            }
        }

        Column{
            visible: UiCore.currentDevice.systemSettings.cabNumber === 2

            width: main.dialWidth
            height: main.dialHeight

            opacity: module.moduleEnabled ? 1.0 : 0.5

            Item{
                id: _Text


                height: parent.height/3
                width: parent.width

                Text{
                    id: textValue
                    anchors.fill: parent

                    text: "Cab num"
                    font.pixelSize: 5 * Style.dip
                    font.bold: true

                    color: Style.currentTheme.textEnabled

                    horizontalAlignment: TextInput.AlignHCenter
                    verticalAlignment: TextInput.AlignVCenter
                }
            }

            ComboBox
            {
                id: _chooseCabCombo

                height: parent.height / 3
                width: parent.width * 0.75

                model: ["1", "2"]

                anchors.horizontalCenter: parent.horizontalCenter

                currentIndex: 0

                onActivated: {
                    _fileBrowser.currentCabNum = _chooseCabCombo.currentIndex
                }
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

        // ParameterDial{
        //     width: main.dialWidth
        //     height: main.dialHeight

        //     controlValue: (_chooseCabCombo.currentIndex === 0) ? module.firstCabDelay : module.secondCabDelay
        // }

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
    }
}
