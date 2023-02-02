import QtQuick 2.0
import QtQuick 2.7
import QtQuick.Controls 1.5
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.1


Item
{
    id: main
    width: parent.width
    height: parent.height/5*1
        property string dialFreq: "20 Hz 220"
    property int toolMult: (nomBand==1)?1:   (nomBand==2)?1:   (nomBand==3)?2:   (nomBand==4)?10:   50
    property int toolAdd:  (nomBand==1)?120: (nomBand==2)?360: (nomBand==3)?800: (nomBand==4)?2000: 6000
    property int disp: toolAdd + toolMult*(eqslider.value)
    MText
    {
        anchors.centerIn: parent
        text: disp
        font.bold: true
//        font.pixelSize: 16
        color: "blue"
    }
    MouseArea
    {
        anchors.fill: parent
        onClicked:
        {
            dialogAndroid.open()
        }
    }
    Dialog {
        id: dialogAndroid
property int startValue
        //
        //
        width: 300
        height: 200
        contentItem: Rectangle {
            width: 300
            height: 200
            color: "#5E5971"


            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: dividerHorizontal.top
                color: "#5E5971"

                Item {
                    height: parent.height/2
                    width: parent.width
                    NewSlider
                    {
                        id: eqslider
                        width: parent.width
                        height: parent.height
                        minimumValue: -100
                        maximumValue: 100
                    }
                    anchors.bottom: parent.bottom
                }
                Item {
                    height: parent.height/2
                    width: parent.width/5
                    MText
                    {
                        anchors.centerIn: parent
                        text: disp
                        font.family: "Arial Black"
                        font.bold: true
//                        font.pixelSize: 30
                    }
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
            Rectangle {
                id: dividerHorizontal
                color: "#d7d7d7"
                height: 2
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: row.top
            }

            Row {
                id: row
                height: 80
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right

                Button {
                    id: dialogButtonCancel
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: parent.width / 2 - 1

                    style: ButtonStyle {
                        background: Rectangle {
                            color: control.pressed ? "#d7d7d7" : "#5E5971"
                            border.width: 0
                        }

                        label: MText {
                            text: qsTr("Cancel")
                            color: "black"
//                            font.pixelSize: 20
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }
                    onClicked:
                    {
                        eqslider.value = dialogAndroid.startValue;
                        dialogAndroid.close()
                    }
                }
                Rectangle {
                    id: dividerVertical
                    width: 2
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    color: "#d7d7d7"
                }

                Button {
                    id: dialogButtonOk
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: parent.width / 2 - 1
                    style: ButtonStyle {
                        background: Rectangle {
                            color: control.pressed ? "#d7d7d7" : "#5E5971"
                            border.width: 0
                        }

                        label: MText {
                            text: qsTr("SAVE")
                            color: "black"
//                            font.pixelSize: 20
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }
                    onClicked: dialogAndroid.close()
                }
            }
        }
        onVisibleChanged:
        {
            if(dialogAndroid.visible )
            {
                startValue = eqslider.value;
            }
        }
    }
}
