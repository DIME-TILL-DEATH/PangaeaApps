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
        color: Style.currentTheme.mainEnabledColor

        Layout.preferredWidth: parent.width/2
        Layout.margins: 2
        Layout.fillWidth: true
        Layout.fillHeight: true

        height: parent.height

        radius: root.radius
        border.width: 2

        GridLayout {

            anchors.centerIn: parent

            width: parent.width*0.85
            height: parent.height*0.95

            columns: 2

            Item{
                Layout.columnSpan: 2
                Layout.fillWidth: true
                Layout.fillHeight: true

                MText{
                    anchors.centerIn: parent
                    text: qsTr("Avaliable interfaces:")
                }
            }

            Indicator{
                isOk: InterfaceManager.isBleAvaliable
                indicatorText: "Bluetooth LE"

                sourceImage: "qrc:/Images/bluetooth-icon.svg"

                Layout.preferredWidth: parent.width/2
                Layout.preferredHeight: parent.height/25
            }

            Indicator{
                isOk: true
                indicatorText: "USB"

                sourceImage: "qrc:/Images/usb-icon.svg"

                Layout.preferredWidth: parent.width/2
                Layout.preferredHeight: parent.height/25
            }

            Item{
                Layout.columnSpan: 2
                Layout.fillWidth: true
                Layout.fillHeight: true

                MText{
                    anchors.centerIn: parent
                    text: qsTr("Services:")
                }
            }

            MButton{
                id: control

                text: qsTr("Run IR converter")

                Layout.columnSpan: 2
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: parent.height/10

                onClicked: UiCore.runIrConvertor();
            }

            MButton{
                text: qsTr("AMT electronics web-site")


                Layout.columnSpan: 2
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: parent.height/10

                onClicked: Qt.openUrlExternally("http://www.amtelectronics.com/");
            }

            MButton{
                text: qsTr("Pangaea web page")


                Layout.columnSpan: 2
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: parent.height/10

                onClicked: Qt.openUrlExternally("https://media.amt-sales.com/cat/cab-emulation/pangaea-series/")
            }

            Item{
                Layout.columnSpan: 2
                Layout.fillWidth: true
                Layout.fillHeight: true
                MText{
                    anchors.centerIn: parent
                    text: qsTr("Settings:")
                }
            }

            MComboHorizontal{
                id: languageCombo

                text: qsTr("Language:")

                Layout.preferredWidth: parent.width/2
                Layout.preferredHeight: parent.height/10

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

            MSwitchHorizontal{
                id: swAutoconnect

                text: qsTr("Autoconnect")

                Layout.preferredWidth: parent.width/2
                Layout.preferredHeight: parent.height/15

                checked: UiSettings.autoConnectEnabled

                onClicked: {
                    UiSettings.saveSetting("autoconnect_enable", swAutoconnect.checked);
                }
            }

            MComboHorizontal{
                id: themeCombo

                text: qsTr("Theme:")

                Layout.preferredWidth: parent.width/2
                Layout.preferredHeight: parent.height/10

                model: ["Classic blue", "Dark orange", "Dark blue", "Dark green"]

                currentIndex: UiSettings.colorTheme

                onActivated: function activated(index){
                    UiSettings.colorTheme = index

                    switch(index){
                        case 0: Style.currentTheme = Style.themeClassicBlue; break;
                        case 1: Style.currentTheme = Style.themeDarkOrange; break;
                        case 2: Style.currentTheme = Style.themeDarkBlue; break;
                        case 3: Style.currentTheme = Style.themeDarkGreen; break;
                    }
                }
            }

            MSwitchHorizontal{
                id: swCheckUpdates

                text: qsTr("Check updates")

                Layout.preferredWidth: parent.width/2
                Layout.preferredHeight: parent.height/15

                checked: UiSettings.checkUpdatesEnabled


                onClicked: {
                    UiSettings.saveSetting("check_updates_enable", swCheckUpdates.checked);
                }
            }
        }
    }
}
