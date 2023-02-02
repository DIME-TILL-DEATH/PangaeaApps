import QtQuick 2.15 //2.7
import QtQuick.Controls 2.15 //1.5
//import QtQuick.Controls.Styles 1.4

import StyleSettings 1.0
import Elements 1.0

Item
{
    id: main

    property bool on: true
    property string nameValue: "amp_on"

    property int ampType

    BaseModule{
        id: _baseModule

        width: parent.width
        height: parent.height

        moduleName: qsTr("PA")
        moduleDescription: qsTr("Power amp")

        contentItem: Column{

            property alias curInd: _comboBox.currentIndex

            height: parent.height
            width: parent.width
            CustomizerSlider
            {
                height: parent.height/4
                width: parent.width
                nameValue: "Volume"
                nameParam: "amp_volume"
                moduleOn: on
            }

            CustomizerSlider
            {
                height: parent.height/4
                width: parent.width
                nameValue: "Presence"
                nameParam: "presence_volume"
                moduleOn: on
            }
            CustomizerSlider
            {
                height: parent.height/4
                width: parent.width
                nameValue: "Slave"
                nameParam: "amp_slave"
                moduleOn: on
            }

            MComboBox
            {
                id: _comboBox

                on: main.on

                height: parent.height/4
                width: parent.width

                model: ["01.PP 6L6","02.PP EL34","03.SE 6L6","04.SE EL34","05.AMT TC-3","06.CALIF","07.BRIT M","08.BRIT L","09.DEFAULT","10.CALIF MOD","11.CALIF VINT","12.PVH 01","13.PVH 02","14.PVH 03","15.PVH 04"]

                currentIndex: ampType

                onActivated:
                {
                    _uiCore.setParameter("amp_type", currentIndex);
                }

                Connections
                {
                    target: _uiCore
                    function onSgSetUIParameter(nameParam, nameValue)
                    {
                        if(nameParam === "amp_type")
                        {
                            main.ampType = nameValue
                            _comboBox.currentIndex = nameValue
                        }
                    }
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
