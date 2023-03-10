import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

import Elements
import StyleSettings
import CppObjects 1.0

RowLayout{
    id: root

    anchors.fill: parent
    spacing: 2

    property bool isConnected: false
    property double radius: width/50

    ConnectionLayout{

    }

    Rectangle{
        color: Style.mainDisabledColor
        Layout.preferredWidth: parent.width/2
        Layout.preferredHeight: parent.height
        Layout.fillWidth: true
        Layout.margins: 2

        height: parent.height

        radius: root.radius
        border.width: 2
        Column {

            anchors.centerIn: parent

            width: parent.width*0.7
            height: parent.height*0.95

            spacing: height/30
            Item{
                width: parent.width
                height: parent.height/20
                MText{
                    anchors.centerIn: parent
                    text: qsTr("Avaliable interfaces:")
                }
            }

            Indicator{
                isOk: true
                indicatorText: "USB"

                sourceImage: "qrc:/qml/Images/usb-icon.svg"

                width: parent.width
                height: parent.height/20
            }

            Indicator{
                isOk: InterfaceManager.isBleAvaliable
                indicatorText: "Bluetooth LE"

                sourceImage: "qrc:/qml/Images/bluetooth-icon.svg"

                width: parent.width
                height: parent.height/20
            }

            Item{
                width: parent.width
                height: parent.height/20
                MText{
                    anchors.centerIn: parent
                    text: qsTr("Services:")
                }
            }

            Button{
                text: qsTr("Run IR converter")
                width: parent.width
                height: parent.height/15

                onClicked: UiCore.runIrConvertor();
            }

            Button{
                text: qsTr("AMT electronics web-site")
                width: parent.width
                height: parent.height/15

                onClicked: Qt.openUrlExternally("http://www.amtelectronics.com/");
            }

            Button{
                text: qsTr("Pangaea web page")
                width: parent.width
                height: parent.height/15

                onClicked: Qt.openUrlExternally("https://media.amt-sales.com/cat/cab-emulation/pangaea-series/")
            }

            Item{
                width: parent.width
                height: parent.height/20
                MText{
                    anchors.centerIn: parent
                    text: qsTr("Settings:")
                }
            }

//            Switch{
//                id: control1
//                height: parent.height/20
//                text: qsTr("Autoconnect")

//                contentItem: MText {
//                         text: control1.text
//                         verticalAlignment: Text.AlignVCenter
//                         leftPadding: control1.indicator.width + control1.spacing
//                     }
//            }

//            Switch{
//                id: control2
//                height: parent.height/20
//                text: qsTr("Check updates")

//                contentItem: MText {
//                         text: control2.text
//                         verticalAlignment: Text.AlignVCenter
//                         leftPadding: control2.indicator.width + control2.spacing
//                     }
//            }

            Row{
                Item{
                    width: txt.contentWidth+txt.leftPadding*2
                    height: parent.height
                    MText{
                        id: txt
                        leftPadding: 5
                        text: qsTr("Application language:")
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                ComboBox{
                    id: languageCombo
                    model: ["English", "??????????????", "Italiano", "Deutsch"]

                    onActivated: function activated(index){
                        var strLanguageCode;
                            switch(index){
                                case 0: strLanguageCode = "en"; break;
                                case 1: strLanguageCode = "ru"; break;
                                case 2: strLanguageCode = "it"; break;
                                case 3: strLanguageCode = "de"; break;
                            }
                            UiCore.setLanguage(strLanguageCode);
                    }
                }
            }
        }
    }

    Connections
    {
        target: UiCore
        function onSgSetUIText(nameParam, inString){
            if(nameParam === "application_language")
            {
                UiCore.setLanguage(inString);

                if(inString === "en") {languageCombo.currentIndex = 0; return;}
                if(inString === "ru") {languageCombo.currentIndex = 1; return;}
                if(inString === "it") {languageCombo.currentIndex = 2; return;}
                if(inString === "de") {languageCombo.currentIndex = 3; return;}

                languageCombo.currentIndex = 0;
            }
        }
    }
}
