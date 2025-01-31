import QtQuick

import Elements 1.0
import CppObjects 1.0

Item
{
    id: main

    BaseModule{
        id: _baseModule

        isHeaderVisible: false

        module: EqResponse

        contentItem: Column
        {
            anchors.fill: parent

            SwitchEqMap
            {
                id: switchEnMap

                height: parent.height/1000*72
                width:  parent.width

                z: _baseModule.z+1
            }

            Item
            {
                height: parent.height/1000*928
                width:  parent.width

                Map
                {
                    visible: switchEnMap.map
                    anchors.fill: parent

                    MouseArea{
                        id: mAsteaalModuleOnOf
                        anchors.fill: parent
                        z: parent.z-1
                    }
                }
            }
        }
    }
}
