import QtQuick 2.15
import QtQuick.Controls 2.15

import CppObjects 1.0

Item{
    id: _root

    property int currentIndex: -1

    property bool currentImpulseEnabled
    property string currentImpulseName

    Rectangle{
        anchors.centerIn: parent

        width: Math.min(parent.width*0.5, parent.height*0.5)
        height: width

        radius: width/2

        color: isImpulseEmpty ? "transparent" : "Salmon"

        opacity: ((currentIndex === presetMapIndex) ? currentImpulseEnabled : isImpulseEnabled) ? 1 : 0.5
        border.width: Math.max(2, width/20)
        border.color: currentIndex === presetMapIndex ? "Salmon" : "#EBECEC"

        MouseArea
        {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor

            onClicked: {
                UiCore.currentDevice.changePreset(bankNumber, presetNumber);
            }
            onEntered: tp.visible = (tp.text.length>0)
            onExited:  tp.visible = false
        }

        ToolTip
        {
            id: tp
            text: currentIndex === presetMapIndex ? currentImpulseName : impulseName
            visible: false
            timeout: 0
        }
    }
}
