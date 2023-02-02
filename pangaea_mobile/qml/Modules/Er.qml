import QtQuick
import QtQuick.Controls

import StyleSettings 1.0
import Elements 1.0

Item
{
    id: main

    property bool on: true
    property string nameValue: "early_on"

    property int currentType

    BaseModule{
        id: _baseModule

        moduleName: qsTr("ER")
        moduleDescription: qsTr("Early reflections")

        width: parent.width
        height: parent.height

        contentItem: Column
        {
            height: parent.height
            width: parent.width
            CustomizerSlider
            {
                id: _slider

                height: parent.height/2
                width: parent.width
                nameValue: "Early reflection"
                nameParam: "early_volume"
                bottomLineEnabled: false
                moduleOn: on        

            }

            MComboBox
            {
                id: _combo

                height: parent.height/2
                width: parent.width

                on: main.on

                currentIndex: currentType

                model: ["SHORT","MEDIUM","LONG"]

                onActivated:
                {
                    _uiCore.setParameter("early_type", currentIndex);
                }
            }
        }
    }

    Connections
    {
        target: _uiCore
        function onSgSetUIParameter(nameParam, nameValue)
        {
            if((nameParam === main.nameValue))
            {
                main.on=nameValue

            }

            if(nameParam === "early_type")
            {
                main.currentType = nameValue
            }
        }
    }
    Connections{
        target: _baseModule
        function onSgModuleOnOf()
        {
            main.on = (!main.on);
        }
    }


}
