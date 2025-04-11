import QtQuick 2.15
import QtQuick.Controls.Fusion
import QtQuick.Dialogs
import QtCore

import QtQuick.Window 2.15

import ModulesFX 1.0
import StyleSettings 1.0
import Layouts 1.0

import CppObjects
import PangaeaBackend

Column
{
    id: _mainUi
    anchors.fill: parent
    focus: true
    spacing: 2


    HeadCPModern
    {
        id: _headCP100FX

        width:  parent.width
        height: parent.height/1000*200 - _mainUi.spacing * 3

        // onOpenIrManagerWindow: {
        //     _irManagerWindow.show();
        // }
    }

    Row{
        width:  parent.width
        height: parent.height/1000*300

        spacing: parent.width/80
        // Rectangle{
        //     height: parent.height/1000*350
        //     width: height

        //     color: "transparent"
        //     border.width: 2
        //     border.color: "green"
        // }
        Repeater{
            model: 14

            height: parent.height/1000*350
            width: height

            Rectangle{
                height: parent.height * 0.9
                width: height/2

                color: "transparent"
                border.width: 2
                border.color: "green"
            }
        }
    }

    Loader{
        width:  parent.width
        height: parent.height/1000*500

        source: "../ModulesFX/PR.qml";
    }
}
