import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

import Elements
import StyleSettings
import CppObjects 1.0

import PangaeaBackend

Column{
    spacing: height/100

    Rectangle{
        id: _scanContainerRect

        width: parent.width
        height: parent.height*12/100

        //color: "blue"
        border.width: 2
        radius: root.radius

        SequentialAnimation on color{
            running: !isConnected
            loops: Animation.Infinite
            alwaysRunToEnd: true
            ColorAnimation {from: Style.highlightColor; to: Style.mainEnabledColor; duration: 1000}
            ColorAnimation {from: Style.mainEnabledColor; to: Style.highlightColor; duration: 1000}
        }

        Item{
            id: _imgContainer

            width:  _scanContainerRect.height*0.6
            height: _scanContainerRect.height*0.6
            anchors.right: _textColumn.left
            anchors.rightMargin: _scanContainerRect.height/10
            anchors.verticalCenter: parent.verticalCenter
            Image
            {
                id: _scanImg
                source: "qrc:/Images/Arrow Gear new.svg"

                anchors.fill: parent

                visible: !isConnected

                fillMode: Image.PreserveAspectFit
                transformOrigin: Item.Center
            }
            Image
            {
                id: _connectedImg
                source: "qrc:/Images/Connected.svg"

                anchors.fill: parent

                visible: isConnected

                fillMode: Image.PreserveAspectFit
                transformOrigin: Item.Center
            }

            SequentialAnimation on rotation{
                running: !isConnected
                loops: Animation.Infinite
                alwaysRunToEnd: true
                NumberAnimation {
                    from: 0
                    to: 360
                    duration: 1500
                }
            }
        }

        Column{
            id: _textColumn

            x: (_scanContainerRect.width - width + _imgContainer.width)/2


            width: (_mainText.textWidth > _auxText.textWidth) ?
                       _mainText.textWidth : _auxText.textWidth

            anchors.verticalCenter: parent.verticalCenter
            MText
            {
                id: _mainText
                anchors.horizontalCenter: parent.horizontalCenter

                text: qsTr("Searching for devices...")
            }
            MText
            {
                id: _auxText
                anchors.horizontalCenter: parent.horizontalCenter

                font.pixelSize: _mainText.font.pixelSize*0.8
                text: ""
                visible: isConnected
            }
        }
    }

    Rectangle{
        color: Style.mainEnabledColor

        width: parent.width
        height: parent.height*60/100

        radius: root.radius
        border.width: 2

        ListView{
            id: listView

            property int autoSelectedItem
            currentIndex: (autoSelectedItem<count) ? autoSelectedItem : "0"

            width: parent.width*0.9
            height: parent.height*0.95

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            enabled: !isConnected

            model: DevicesListModel

            focus: true
            clip: true

            delegate: ILDelegate{
                property variant data: model

                width: listView.width
                height: listView.height/6
            }

            highlight: ILHighlight{}

            // Transitions
            add: Transition {
                 NumberAnimation { properties: "y"; from: listView.height; duration: 250 }
            }

            displaced: Transition{
                NumberAnimation { properties: "y"; duration: 300 }
            }

            remove: Transition {
                 NumberAnimation { properties: "x"; to: listView.width; duration: 250 }
            }
        }
    }

    Rectangle{
        id: connectButton

        width: parent.width
        height: parent.height*12/100

        enabled: (listView.count > 0)

        border.width: 2
        radius: root.radius

        opacity: mA.pressed ? 0.5 : 1

        color: enabled ? Style.mainEnabledColor : "gray"

        MText{
            anchors.centerIn: parent
            text: qsTr("Click to connect")
        }

        MouseArea{
            id: mA

            anchors.fill: parent
            enabled: parent.enabled

            onClicked: {
                InterfaceManager.connectToDevice(listView.currentItem.data.deviceDescription,)
            }
        }
    }

    Rectangle{
        id: offlineInterfacesButton

        width: parent.width
        height: parent.height*12/100

        border.width: 2
        radius: root.radius

        opacity: mA.pressed ? 0.5 : 1

        color: enabled ? Style.mainEnabledColor : "gray"

        MText{
            anchors.centerIn: parent
            text: qsTr("Show virtual interfaces")
        }

        MouseArea{
            id: mA2

            anchors.fill: parent
            enabled: parent.enabled

            onClicked: {
                InterfaceManager.startScanning(DeviceConnectionType.Offline);
            }
        }
    }
}
