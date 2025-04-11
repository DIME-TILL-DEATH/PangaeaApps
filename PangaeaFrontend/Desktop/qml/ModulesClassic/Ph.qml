import QtQuick 2.15

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property Phaser module

    contentItem: Row{
        anchors.fill: parent
        Column{
            id: _column

            width: parent.width/2
            height: parent.height

            Item
            {
                width:  parent.width
                height: parent.height/1000*150
            }

            ParameterDial{
                controlValue: module.rate
            }

            ParameterDial{
                controlValue: module.width
            }

            ParameterDial{
                controlValue: module.center
            }

            ParameterDial{
                controlValue: module.feedback
            }

            ParameterDial{
                controlValue: module.mix
            }

            Item{
                width:  parent.width
                height: parent.height/1000*25
            }
        }

        Column{
            width: parent.width/2
            height: parent.height

            Item
            {
                width:  parent.width
                height: parent.height/1000*150
            }

            Item
            {
                width:  parent.width
                height: parent.height/1000*165
            }

            Item
            {
                width:  parent.width
                height: parent.height/1000*165
            }

            Item
            {
                id: _stagesSwitch

                width:  parent.width
                height: parent.height/1000*165

                property ControlValue controlValue: module.stages

                enabled: module.moduleEnabled

                Column
                {
                    anchors.fill:  parent

                    Switch3
                    {
                        id:    mSwitch
                        width: parent.width
                        height: parent.height/100*50
                        opacity: _stagesSwitch.enabled ? 1 : 0.5

                        value: _stagesSwitch.controlValue.displayValue

                        onChValue: function changeValue(value){
                            _stagesSwitch.controlValue.displayValue = value
                        }
                    }

                    Column
                    {
                        width: parent.width*0.9
                        height: parent.height/100*50
                        anchors.horizontalCenter: parent.horizontalCenter
                        CheckText
                        {
                            width: parent.width
                            height: parent.height/3
                            text: "Stages 3"
                            check: mSwitch.value==0
                            font.pixelSize: height/1.4
                            horizontalAlignment: Text.AlignLeft

                            onClicked: _stagesSwitch.controlValue.displayValue = 0
                        }

                        CheckText
                        {
                            width: parent.width
                            height: parent.height/3
                            text: "Stages 5"
                            check: mSwitch.value==1
                            font.pixelSize: height/1.4

                            onClicked: _stagesSwitch.controlValue.displayValue = 1
                        }

                        CheckText
                        {
                            width: parent.width
                            height: parent.height/3
                            text: "Stages 7"
                            check: mSwitch.value==2
                            font.pixelSize: height/1.4
                            horizontalAlignment: Text.AlignRight

                            onClicked: _stagesSwitch.controlValue.displayValue = 2
                        }
                    }
                }
            }
        }
    }
}
