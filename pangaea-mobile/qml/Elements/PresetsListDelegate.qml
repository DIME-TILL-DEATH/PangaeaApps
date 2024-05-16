import QtQuick 2.15//2.12
import QtQuick.Controls 2.15//2.5
import QtQuick.Layouts 1.15

import StyleSettings 1.0

import ControlGroups 1.0
import Modules 1.0
import Elements 1.0

import CppObjects

Rectangle{
    width: 100
    height: _grid.height*1.1

    property int maxMap: 4

    color: ListView.isCurrentItem ? Style.colorItemHighlight  : Style.colorFon

    radius: Style.baseRadius
    border.width: 1
    border.color: isImpulseEnabled ? Style.currentTheme.colorBorderOn
                                   : Style.currentTheme.colorBorderOff

    GridLayout{
        id: _grid

        width: parent.width

        columns: 2
        rows: 2

        Item{
            id: _presetNameContainer

            Layout.preferredWidth: parent.width
            Layout.preferredHeight: _presetNameText.textHeight*1.4
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            Layout.fillWidth: true

            MText{
                id: _presetNameText

                leftPadding: (_impulseOnIndicatorContainer.width-_impulseOnIndicator.width)/2

                text: presetName
                font.pixelSize: 8 * Style.dip
                color: Style.colorText

                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Item{
            id: _impulseOnIndicatorContainer

            Layout.preferredWidth: parent.width/10
            Layout.preferredHeight: _impulseNameContainer.height*1.05
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            Layout.fillWidth: true

            Rectangle{
                id: _impulseOnIndicator

                width: _impulseNameText.font.pixelSize*0.9
                height: width

                radius: width/2
                border.width: 1

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                color: isImpulseEnabled ? Style.currentTheme.colorLedOn
                                        : Style.currentTheme.colorLedOff
            }
        }

        Item{
            id: _impulseNameContainer

            Layout.preferredWidth: parent.width-_impulseOnIndicatorContainer.width
            Layout.preferredHeight: childrenRect.height
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            Layout.fillWidth: true

            MText{
                id: _impulseNameText

                width: parent.width

                text: impulseName

                elide: Text.ElideMiddle
                anchors.verticalCenter: parent.verticalCenter

                color: isImpulseEnabled ? Style.currentTheme.colorTextEnabled
                                        : Style.currentTheme.colorTextDisabled
            }
        }
    }

    MouseArea{
        anchors.fill: parent

        enabled: !parent.ListView.isCurrentItem

        onClicked: {
            DeviceProperties.changePreset(bankNumber, presetNumber)
        }
    }
}

