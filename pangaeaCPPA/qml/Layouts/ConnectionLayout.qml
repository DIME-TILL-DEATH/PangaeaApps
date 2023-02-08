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

  //  color: Style.mainDisabledColor

    width: parent.width/2
    height: parent.height

    spacing: height/50

    property double radius: width/20
 //   border.width: 2

    Rectangle{
        id: _scanContainerRect

        width: parent.width
        height: parent.height*9/50//-parent.spacing*3

        color: "blue"
        border.width: 1
      //  border.color:  Style.currentTheme.colorBorderOn
        radius: root.radius

        SequentialAnimation on color{
            running: !isConnected
            loops: Animation.Infinite
            alwaysRunToEnd: true
            ColorAnimation {from: Style.mainEnabledColor; to: "gray"; duration: 1000}
            ColorAnimation {from: "gray"; to: Style.mainEnabledColor; duration: 1000}
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
        height: parent.height*40/50

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

            focus: true

            delegate: Item{
                width: listView.width
                height: listView.height/10
                MText{
                    width: parent.width-40
                    leftPadding: parent.width/20
                    anchors.verticalCenter: parent.verticalCenter

                    text: modelData.name + "\n" + modelData.address
                }

                MouseArea
                {
                    anchors.fill: parent
                    z: 1
                    propagateComposedEvents: true

                    onClicked:
                    {
                        listView.autoSelectedItem = index
                    }
                    onDoubleClicked:
                    {
                        listView.autoSelectedItem = index
                        UiCore.connectToDevice(modelData)
                    }
                }
            }

            highlight: Rectangle
            {
                color: Style.highlightColor
                opacity: 0.75

                radius: listView.width/75

                property int iconWidth: 50
                property int iconHeight: 50
                property bool iconVisible: false

               // border.width:1
                //border.color: Style.backgroundColor
                Item{
                    id: _vConatiner

                    visible: iconVisible

                    anchors.verticalCenter: parent.verticalCenter
                    x: parent.width - width*2

                    width: iconWidth
                    height: iconHeight
                    Image
                    {
                        id: _vImg
                        source: "qrc:/qml/Images/(V).svg"

                        anchors.fill: parent

                        fillMode: Image.PreserveAspectFit
                        transformOrigin: Item.Center
                        layer {
                            enabled: true
                            effect: ColorOverlay {
                                color: "white"
                            }
                        }
                    }
                }
            }
        }
    }

    Connections{
        target: UiCore

        //function onSgSetUIDataList(nameParam, list)
        function onSgDeviceListUpdated(list)
        {
            listView.model = list

            console.log(list)
            console.log(DeviceDescription.USBAuto)
        }
    }
}
