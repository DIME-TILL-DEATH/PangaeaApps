import QtQuick

import Elements
import StyleSettings
import CppObjects 1.0

Item
{
    id: main

    property bool copyFirst: false

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
                text: "COPY"
                enabled: !AppProperties.compareState
                onClicked: {
                    AppProperties.copyPreset(); //{UiCore.setParameter("copy", 0);
                    copyFirst=true;
                }
            }

            MButton
            {
                width:  parent.width
                height: parent.height/2
                text: "PASTE"
                scaleText: 0.88
                enabled: copyFirst & !AppProperties.compareState
                onClicked: AppProperties.pastePreset(); //UiCore.setParameter("paste", 0)
            }
        }
    }
}
