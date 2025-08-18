import QtQuick

import StyleSettings

MText
{
    id: main

    signal clicked();
    signal wheel(int angleDelta);

    property bool check: false
    property bool leftText: false

    property bool enabled: true

    text:  "TEXT"
    color: check ? Style.currentTheme.highlightColor
                 : enabled? Style.currentTheme.textEnabled : Style.currentTheme.textDisabled
    horizontalAlignment: leftText?Text.AlignHLeft:Text.AlignHCenter

    font.pixelSize: parent.height/1.5
    opacity: main.enabled ? 1:0.5
    MouseArea
    {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: main.clicked()
        onWheel:   main.wheel(wheel.angleDelta.y);
    }
}
