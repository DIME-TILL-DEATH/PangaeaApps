import QtQuick 2.12
import QtQuick.Controls 2.12 //2.5

import StyleSettings 1.0

Rectangle
{
    id: main

    property string textButton: "1111"
    property bool active: false

    // Почему-то именно кнопка save при смене своего состояния
    // через свойство "enabled" отключала обновление SwipeView до любого следующего действия
    // поэтому добавлено своё свойство "enabled" отключающее только нужные свойства
    property bool enabledKostyl: true

    property bool highlighted: false
    property color highlightColor: "red"
    property color textColor: main.active ? Style.currentTheme.colorTextEnabled:
                                            ((main.enabled & main.enabledKostyl) ? Style.colorText : Style.colorBtnDisabled)

    signal mbPressed()


    implicitWidth:  parent.width/2 //implicitWidth
    implicitHeight: parent.height/2 //implicitHeight

    border.width: 1
    border.color: main.active ? Style.currentTheme.colorTextEnabled : Style.currentTheme.colorBorderOff

    gradient: Gradient{
                GradientStop{position: 0.0; color: highlighted ? highlightColor : Style.colorModul}
                GradientStop{position: 1.0; color: Style.currentTheme.colorModulOff}}


    clip: true
    radius: Style.baseRadius

    MText
    {
        z: 2
        // anchors.fill: parent
        width: parent.width
        height: parent.height

        text: qsTr(textButton)

        font.bold: true
        color: textColor

        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
    }
    Rectangle
    {
        id: colorRe
        height: 0
        width: 0
        color: Style.colorBtnEffect


        transform: Translate {
            x: -colorRe.width / 2
            y: -colorRe.height / 2
        }
    }
    MouseArea {
        id: myButtonMouse
        // anchors.fill: parent
        width: parent.width
        height: parent.height

        enabled: main.enabledKostyl

        onClicked:
        {
            circleAn.stop()
        }
        onPressed:
        {
            colorRe.x = mouseX;
            colorRe.y = mouseY;
            circleAn.start();
            mbPressed();
        }
        onReleased: circleAn.stop()
        onPositionChanged: circleAn.stop()
    }

    PropertyAnimation {
        id: circleAn
        target: colorRe
        properties: "width,height,radius"
        from: 0
        to: parent.height * 3
        duration: 400

        onStopped: {
            colorRe.width = 0
            colorRe.height = 0
        }
    }
}
