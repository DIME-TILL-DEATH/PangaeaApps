import QtQuick 2.15
import QtQuick.Controls 2.12

import Elements 1.0
import StyleSettings 1.0

ProgressBar
{
    id: progressBar

    width: parent.width*0.8
    height: parent.height*1/10

    anchors.horizontalCenter: parent.horizontalCenter

    property real progressBarValue
    property string progressBarText : (Math.round(progressBarValue*10000)/100)+"%"

    value: progressBarValue

    background: Rectangle {
             implicitWidth: 200
             implicitHeight: 6
             color: Style.colorFon
             radius: 3

             border.width: 1
         }

     contentItem: Item {
         implicitWidth: 200
         implicitHeight: 4

         Rectangle {
             width: progressBar.visualPosition * parent.width
             height: parent.height
             radius: 2
             color: Style.currentTheme.colorModulOn
             border.width: 1
        }

         Text
         {
             width: parent.width
             height: parent.height*1/10

             horizontalAlignment: Text.AlignHCenter
             verticalAlignment: Text.AlignVCenter

             font.pixelSize: parent.width*0.05

             anchors.horizontalCenter: parent.horizontalCenter
             anchors.verticalCenter: parent.verticalCenter

             text: progressBarText

             color: Style.colorText
         }
    }
}
