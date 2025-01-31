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
    property double radius: width/150

    ConnectionLayout{
        id: connectionLayout

        Layout.preferredWidth: parent.width/2
        Layout.margins: 2
        Layout.fillWidth: true
        Layout.fillHeight: true
    }

    Rectangle{
        color: Style.mainEnabledColor

        Layout.preferredWidth: parent.width/2
        Layout.margins: 2
        Layout.fillWidth: true
        Layout.fillHeight: true

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

                sourceImage: "qrc:/Images/usb-icon.svg"

                width: parent.width
                height: parent.height/20
            }

            Indicator{
                isOk: InterfaceManager.isBleAvaliable
                indicatorText: "Bluetooth LE"

                sourceImage: "qrc:/Images/bluetooth-icon.svg"

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
                id: control

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

            Switch{
                id: swAutoconnect
                height: parent.height/20
                text: qsTr("Autoconnect")

                checked: UiSettings.autoConnectEnabled

                contentItem: MText {
                         text: swAutoconnect.text
                         verticalAlignment: Text.AlignVCenter
                         leftPadding: swAutoconnect.indicator.width + swAutoconnect.spacing
                     }

                onClicked: {
                    UiSettings.saveSetting("autoconnect_enable", swAutoconnect.checked);
                }
            }

            Switch{
                id: swCheckUpdates
                height: parent.height/20
                text: qsTr("Check updates")

                checked: UiSettings.checkUpdatesEnabled

                contentItem: MText {
                         text: swCheckUpdates.text
                         verticalAlignment: Text.AlignVCenter
                         leftPadding: swCheckUpdates.indicator.width + swCheckUpdates.spacing
                     }

                onClicked: {
                    UiSettings.saveSetting("check_updates_enable", swCheckUpdates.checked);
                }
            }

            Row{
                height: parent.height/10
                Item{
                    width: txt.contentWidth+txt.leftPadding*2
                    height: parent.height
                    MText{
                        id: txt
                        anchors.fill: parent
                        leftPadding: 5
                        text: qsTr("Application language:")
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                ComboBox{
                    id: languageCombo

                    anchors.verticalCenter: parent.verticalCenter

                    property var codesMap: {"en": 0, "ru": 1, "it": 2, "de": 3}
                    model: ["English", "Русский"] //, "Italiano", "Deutsch"]

                    currentIndex: codesMap[UiSettings.appLanguageCode]

                    onActivated: function activated(index){
                        var strLanguageCode;
                        switch(index){
                            case 0: strLanguageCode = "en"; break;
                            case 1: strLanguageCode = "ru"; break;
                            case 2: strLanguageCode = "it"; break;
                            case 3: strLanguageCode = "de"; break;
                        }
                        UiSettings.setLanguage(strLanguageCode);
                    }
                }
            }
        }
    }
}
