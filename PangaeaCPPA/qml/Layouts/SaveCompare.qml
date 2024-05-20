import QtQuick

import Elements
import StyleSettings
import CppObjects 1.0

Item
{
    id: main

    property bool edit: true

    Rectangle
    {
        anchors.fill: parent
        color: Style.mainEnabledColor
        Column
        {
            anchors.fill: parent
            MButton
            {
                width:  parent.width
                height: parent.height/2
                text: "SAVE"
                enabled: main.edit & !AppProperties.compareState
                onClicked: DeviceProperties.saveChanges();
            }

            MButton
            {
                id: bComp
                width:  parent.width
                height: parent.height/2
                text: "COMP"

                enabled:  main.edit
                highlighted: AppProperties.compareState

                onClicked: AppProperties.comparePreset();
            }
        }
    }
}
