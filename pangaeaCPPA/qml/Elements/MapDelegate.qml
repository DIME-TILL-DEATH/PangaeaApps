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

        color: ((currentIndex === presetMapIndex) ?
                (currentImpulseName === qsTr("empty")) : (impulseName === qsTr("empty"))) ? "transparent" : "Salmon"
        opacity: ((currentIndex === presetMapIndex) ? currentImpulseEnabled : isImpulseEnabled) ? 1 : 0.5
        border.width: Math.max(2, width/20)
        border.color: currentIndex === presetMapIndex ? "Salmon" : "#EBECEC"

        MouseArea
        {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: UiCore.setParameter("set_preset_change", presetDeviceIndex)
//            onClicked:   UiCore.setParameter("set_preset_change", nomRow*controlMultiplier+index)
           // onClicked: UiCore.setParameter("set_preset_change", 16*nomRow + index) // 4^4 packing (2^4=16 or <<4)
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

    Connections
    {
        target: UiCore
        function onSgSetUIText(nameParam, value)
        {
            if (nameParam==="impulse_name")
                _root.currentImpulseName=value;
        }

        function onSgSetUIParameter(nameParam, value)
        {
            if(nameParam==="cabinet_enable")
                _root.currentImpulseEnabled = value;
        }

        function onSgSetParameter(nameParam, value)
        {
            if(nameParam==="cabinet_enable")
                _root.currentImpulseEnabled = value;
        }
    }
}
