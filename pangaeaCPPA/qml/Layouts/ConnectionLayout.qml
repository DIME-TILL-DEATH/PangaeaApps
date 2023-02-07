import QtQuick
import QtQuick.Controls

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

    property double radius: width/20
 //   border.width: 2

    Rectangle{
        id: _scanContainerRect

        width: parent.width
        height: parent.height*1/12//-parent.spacing*3

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
                color: Style.backgroundColor
                text: isConnected ? qsTr("CONNECTED") : qsTr("Searching for devices...")
                // text: isConnected ? "CONNECTED" : "SCANNING"
            }
            MText
            {
                id: _auxText
                anchors.horizontalCenter: parent.horizontalCenter
                color: Style.backgroundColor
                font.pixelSize: _mainText.font.pixelSize*0.8
                text: isConnected ? qsTr("(TAP to disconnect)") : ""
                visible: isConnected
            }
        }

//        MouseArea{
//            anchors.fill: parent
//            onClicked: {
//                if(isConnected)
//                {
//                    messageAcceptDisconnect.open()
//                }
//            }
//        }
    }

    Rectangle{
        color: Style.mainDisabledColor

        width: parent.width
        height: parent.height*11/12

        radius: root.radius
        border.width: 2

        ListView{
            id: listView

            anchors.fill: parent

            delegate: Rectangle{
                width: listView.width*0.75
                height: width/5
                MText{
                    text: modelData.name + modelData.address
                }
            }
        }
    }

    Connections{
        target: UiCore

        function onSgSetUIDataList(nameParam, list)
        {
            listView.model = list

            console.log(list)
        }
    }
}
