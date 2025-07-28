import QtQuick
import QtQuick.Controls

import Elements
import StyleSettings

import CppObjects
import PangaeaBackend

Window
{
    id: main

    width: Screen.width/5
    height: Screen.width/3

    minimumWidth: Screen.width/8
    minimumHeight: Screen.width/10

    title: qsTr("Preset map")

    property alias delegate: _presetListView.delegate
    property alias currentIndex: _presetListView.currentIndex

    onVisibleChanged: {
        if(visible){
            _presetListView.positionViewAtIndex(_presetListView.currentIndex, ListView.Center);
        }
    }

    Rectangle{
        color: Style.mainEnabledColor
        anchors.fill: parent

        ListView{
            id: _presetListView

            width: parent.width * 0.95
            height: parent.height

            currentIndex: UiCore.currentDevice.bank * UiCore.currentDevice.maxBankCount + UiCore.currentDevice.preset

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            // boundsBehavior: Flickable.StopAtBounds
            clip: true

            spacing: 4

            ScrollBar.vertical: ScrollBar {
                id: sbControl
                // size: 0.3
                position: 0.2
                active: true
                orientation: Qt.Vertical

                contentItem: Rectangle {
                    implicitWidth: 6
                    implicitHeight: 100
                    radius: width / 2
                    color: Style.textEnabled

                    opacity: sbControl.policy === ScrollBar.AlwaysOn || (sbControl.size < 1.0) ? 1 : 0
                }
            }


            Connections{
                target:  UiCore.currentDevice.presetListModel

                function onModelReset(){
                    _presetListView.model = UiCore.currentDevice.presetListModel
                }
            }

            Connections
            {
                target: UiCore

                function onCurrentDeviceChanged()
                {
                    switch(UiCore.currentDevice.deviceType)
                    {
                        case DeviceType.UNKNOWN_DEVICE:
                        {
                            _presetListView.model = undefined
                            break;
                        }
                    }
                }
            }
        }

    }
}
