import QtQuick 2.12
//import QtGraphicalEffects 1.15
import Qt5Compat.GraphicalEffects

import StyleSettings 1.0
import Elements 1.0

Rectangle
{
    color: Style.colorItemHighlight

    radius: Style.baseRadius

    property int iconWidth: 50
    property int iconHeight: 50
    property bool iconVisible: false

    border.width:1
    border.color: Style.currentTheme.colorTextEnabled
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
            source: "qrc:/images/(V).svg"

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
