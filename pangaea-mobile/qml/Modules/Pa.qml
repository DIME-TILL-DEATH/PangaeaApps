import QtQuick 2.15
import QtQuick.Controls 2.15

import StyleSettings 1.0
import Elements 1.0

import CppObjects

Item
{
    id: main

    property bool on: true
    property int paramType: DeviceParameter.AMP_ON

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

                name: "Volume"
                paramType: DeviceParameter.AMP_VOLUME

                moduleOn: on
            }

            CustomizerSlider
            {
                height: parent.height/4
                width: parent.width

                name: "Presence"
                paramType: DeviceParameter.PRESENCE_VOLUME

                moduleOn: on
            }
            CustomizerSlider
            {
                height: parent.height/4
                width: parent.width

                name: "Slave"
                paramType: DeviceParameter.AMP_SLAVE

                moduleOn: on
            }

            MComboBox
            {
                id: _comboBox

                on: main.on

                height: parent.height/4
                width: parent.width

                model: ["01.PP 6L6","02.PP EL34","03.SE 6L6","04.SE EL34","05.AMT TC-3","06.CALIF","07.BRIT M","08.BRIT L","09.DEFAULT","10.CALIF MOD","11.CALIF VINT","12.PVH 01","13.PVH 02","14.PVH 03","15.PVH 04"]

                currentIndex: main.ampType

                onActivated:
                {
                    UiCore.setDeviceParameter(DeviceParameter.AMP_TYPE, currentIndex);
                }
            }
        }
    }

    // когда включаем/выклаючаем PA, также включить/выключить presence
    onOnChanged: {
        if(main.visible) // только если модуль есть в устройстве
            UiCore.setParameter("pa-ps_linked_on", main.on);
    }

    Connections
    {
        target: UiCore

        function onSgSetUiDeviceParameter(paramType, value)
        {
            if(paramType === main.paramType)
            {
                main.on=value
                console.log("Settling by sgSetDeviceParameter", paramType);

                if(main.visible) // только если модуль есть в устройстве
                    UiCore.setParameter("pa-ps_linked_on", main.on);
            }

            if(paramType === DeviceParameter.AMP_TYPE)
            {
                main.ampType = value
                console.log("Settling by sgSetDeviceParameter", paramType);
            }
        }
    }

    Connections{
        target: _baseModule

        function onSgModuleOnOf()
        {
            main.on = (!main.on);
            UiCore.setDeviceParameter(main.paramType, main.on)
        }
    }
}
