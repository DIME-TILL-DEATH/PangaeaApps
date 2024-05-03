import QtQuick 2.15
import QtQuick.Controls 2.15

import Elements 1.0
import StyleSettings 1.0

import CppObjects 1.0

Rectangle {
    id: root
    anchors.fill: parent

    property string moduleName: "MN"
    property int paramType

    property bool on: false
    property bool isHeaderVisible: true

    property Component contentItem

    property alias contentProperties: _loader.item

    signal sgModuleOnOf()

    clip: true
    color: on ? Style.mainEnabledColor : Style.mainDisabledColor

    Material
    {
        id: material
        on: root.on
    }

    Column{
        anchors.fill: parent

        Item{
            id: _header

            width:  parent.width
            height: parent.height/1000*70

            visible: isHeaderVisible

            MText
            {
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                font.pixelSize: parent.height/1.7
                color: root.on ? "salmon" : Style.backgroundColor
                text: moduleName

                z: _contentContainer.z+1
            }
        }

        Item{
            id: _contentContainer

            width: parent.width
            height: parent.height-_header.height

            Loader{
                id: _loader

                width: _contentContainer.width
                height: _contentContainer.height

                sourceComponent: contentItem

                onLoaded: {
                    item.parent = root;
                }
            }
        }
    }


    MouseArea
    {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape:  Qt.PointingHandCursor
        onClicked:
        {
            material.start(mouseX, mouseY)
            sgModuleOnOf()
        }
    }


    Rectangle{
        id: _maskCompare

        anchors.fill: root

        opacity: 0.25
        visible: !root.enabled
        z: root.parent.z + 1

        SequentialAnimation on color{
            running: true
            loops: Animation.Infinite
            ColorAnimation {from: "white"; to: "lightcoral"; duration: 1000}
            ColorAnimation {from: "lightcoral"; to: "white"; duration: 1000}
        }
    }

    Connections{
        target: UiCore

        function onSgSetUIParameter(parameterName, inValue)
        {
            if(parameterName === "compare_state")
            {
                root.enabled = !inValue
            }
        }
    }
}
