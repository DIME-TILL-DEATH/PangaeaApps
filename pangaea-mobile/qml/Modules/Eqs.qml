import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

import CppObjects

Item
{
    id: main

    signal extVisible

    property string name: "EQ"
    property int paramType: DeviceParameter.EQ_ON

    property bool on: true
    property bool isPrePostVisible: true

    property int eq1: -20
    property int eq2: 70
    property int eq3: -80
    property int eq4: 60
    property int eq5: 95

    property int q1: 0
    property int q2: 0
    property int q3: 0
    property int q4: 0
    property int q5: 0

    property bool isPreEQ: true

    property double band_width_mult: 1.5/10

    BaseModule{
        id: _baseModule

        moduleName: qsTr("EQ")
        moduleDescription: qsTr("Equalizer")

        contentItem: Row
        {
            id: _content
            spacing: 0

            Item{
                height: parent.height
                width: parent.width

                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        extVisible();
                    }
                }

                Row
                {
                    anchors.fill: parent

                    spacing: 2

                    EQBar{
                        eqSetting: eq1
                        qSetting: q1
                        isOn: main.on
                    }

                    EQBar{
                        eqSetting: eq2
                        qSetting: q2
                        isOn: main.on
                    }

                    EQBar{
                        eqSetting: eq3
                        qSetting: q3
                        isOn: main.on
                    }

                    EQBar{
                        eqSetting: eq4
                        qSetting: q4
                        isOn: main.on
                    }

                    EQBar{
                        eqSetting: eq5
                        qSetting: q5
                        isOn: main.on
                    }

                    Item{
                        id: _prePostEQ

                        height: parent.height
                        width: parent.width*2/10

                        MouseArea{
                           anchors.fill: parent
                           enabled: isPrePostVisible

                            onClicked: {
                                isPreEQ = !isPreEQ
                                UiCore.setDeviceParameter(DeviceParameter.EQ_PRE, isPreEQ);
                            }
                        }

                        Column
                        {
                            visible: isPrePostVisible
                            anchors.fill: parent
                            MText
                            {
                                height: parent.height/2
                                width: parent.width

                                //anchors.fill: parent
                                horizontalAlignment: Qt.AlignHCenter
                                verticalAlignment: Qt.AlignVCenter
                                text: qsTr("PRE")
//                                color: isPreEQ ? (on ? Style.currentTheme.colorTextEnabled : Style.currentTheme.colorTextDisabledLight)
//                                               : (on ? Style.colorText : Style.currentTheme.colorTextDisabled)
                                color: isPreEQ ? (on ? Style.currentTheme.colorLedOn : Style.currentTheme.colorTextEnabled)
                                               : (on ? Style.currentTheme.colorTextDisabled : Style.currentTheme.colorModulOff)
                                font.bold: true
                            }

                            MText
                            {
                                height: parent.height/2
                                width: parent.width

                                horizontalAlignment: Qt.AlignHCenter
                                verticalAlignment: Qt.AlignVCenter
                                text: qsTr("POST")
                                color: isPreEQ ? (on ? Style.currentTheme.colorTextDisabled : Style.currentTheme.colorModulOff)
                                               : (on ? Style.currentTheme.colorLedOn : Style.currentTheme.colorTextEnabled)

                                font.bold: true
                            }
                        }
                    }
                }
            }

        }


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
            }

            if(paramType === DeviceParameter.EQ_PRE)
            {
                isPreEQ=value
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
