import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

import Elements
import StyleSettings
import CppObjects 1.0

Column {
    id: root

    property bool isConnected: false

    anchors.centerIn: parent

    width: parent.width/2
    height: parent.height

    spacing: height/50

    property double radius: width/50

    Rectangle{
        id: _scanContainerRect

        width: parent.width
        height: parent.height*7/50//-parent.spacing*3

        color: "blue"
        border.width: 1
      //  border.color:  Style.currentTheme.colorBorderOn
        radius: root.radius

        SequentialAnimation on color{
            running: !isConnected
            loops: Animation.Infinite
            alwaysRunToEnd: true
            ColorAnimation {from: Style.mainEnabledColor; to: Style.headColor; duration: 1000}
            ColorAnimation {from: Style.headColor; to: Style.mainEnabledColor; duration: 1000}
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
                source: "qrc:/qml/Images/Arrow Gear new.svg"

                anchors.fill: parent

                visible: !isConnected

                fillMode: Image.PreserveAspectFit
                transformOrigin: Item.Center
            }
            Image
            {
                id: _connectedImg
                source: "qrc:/qml/Images/Connected.svg"

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

                text: isConnected ? qsTr("CONNECTED") : qsTr("Searching for devices...")
                // text: isConnected ? "CONNECTED" : "SCANNING"
            }
            MText
            {
                id: _auxText
                anchors.horizontalCenter: parent.horizontalCenter

                font.pixelSize: _mainText.font.pixelSize*0.8
                text: isConnected ? qsTr("(TAP to disconnect)") : ""
                visible: isConnected
            }
        }
    }

    Rectangle{
        color: Style.mainDisabledColor

        width: parent.width
        height: parent.height*35/50

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

            model: InterfaceListModel

            focus: true
            clip: true

            delegate: ILDelegate{
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

//    Rectangle{
//        color: Style.mainDisabledColor

//        width: parent.width
//        height: parent.height*6/50

//        radius: root.radius
//        border.width: 2

//        ButtonGroup{
//            buttons: row.children
//        }

//        Row{
//            id: row
//            anchors.centerIn: parent

//            spacing: 5

//            width: parent.width*0.75
//            height: parent.height*0.9

//            RadioButton{
//                text: "USB"
//                onClicked: {
//                    InterfaceManager.setInterface(DeviceDescription.USBAuto);
//                }
//            }
//            RadioButton{
//                text: "Bluetooth"
//                onClicked: {
//                    InterfaceManager.setInterface(DeviceDescription.BLE);
//                }
//            }
//        }
//    }
}
