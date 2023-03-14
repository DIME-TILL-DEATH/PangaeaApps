import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

import Elements
import StyleSettings

Rectangle
{
    color: Style.headColor
    opacity: 0.75

    radius: listView.width/75

    property int iconWidth: 50
    property int iconHeight: 50
    property bool iconVisible: false
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

