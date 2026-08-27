import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

import PangaeaFrontend
import PangaeaBackend

Item {

     id: _root
    signal closeSettingsWindow()

    Rectangle
    {
        anchors.fill: parent
        color: Style.colorFon
    }
    Column
    {
        anchors.fill: parent
        spacing: 4
        padding: 4

        Rectangle{
            width: parent.width*0.98
            height: parent.height*0.3
            border.color: Style.currentTheme.colorBorderOn
            radius: Style.baseRadius

            gradient: Gradient{
                GradientStop{position: 0.0; color: Style.colorModul}
                GradientStop{position: 1.0; color: Style.currentTheme.colorModulOff}
            }

            ButtonGroup {
                id: _btnGroupTheme

                buttons: columnThemes.children
                exclusive: true
                onCheckedButtonChanged: {
                    if(_darkOrange.checked)
                    {
                        UiSettings.colorTheme = UiSettings.DarkOrange
                        Style.currentTheme = Style.themeOrange;
                    }
                    if(_darkGreen.checked)
                    {
                        UiSettings.colorTheme = UiSettings.DarkGreen
                        Style.currentTheme = Style.themeGreen;
                    }
                    if(_darkBlue.checked)
                    {
                        UiSettings.colorTheme = UiSettings.DarkBlue;
                        Style.currentTheme = Style.themeBlue;
                    }
                }
            }

            Column {
                id: columnThemes
                anchors.fill: parent
                spacing: 4
                Item{
                    width: parent.width
                    height: parent.height*0.15
                    MText {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Color theme")
                        color: Style.colorText
                    }
                }

                MRadioButton {
                    id: _darkOrange
                    property var themeType: UiSettings.DarkOrange

                    checked: UiSettings.colorTheme === UiSettings.DarkOrange

                    colorCheck: Style.themeOrange.colorModulOn
                    text: qsTr("Dark orange")
               }

                MRadioButton {
                    id: _darkGreen
                    property var themeType: UiSettings.DarkGreen

                    checked: UiSettings.colorTheme === UiSettings.DarkGreen

                    colorCheck: Style.themeGreen.colorTextEnabled
                    text: qsTr("Dark green")
                }

                MRadioButton {
                    id: _darkBlue
                    property var themeType: UiSettings.DarkBlue

                    checked: UiSettings.colorTheme === UiSettings.DarkBlue

                    colorCheck: Style.themeBlue.colorTextEnabled
                    text: qsTr("Dark blue")
                }
            }
        }

        Rectangle{
            width: parent.width*0.98
            height: parent.height*0.3
            border.color: Style.currentTheme.colorBorderOn
            radius: Style.baseRadius

            gradient: Gradient{
                GradientStop{position: 0.0; color: Style.colorModul}
                GradientStop{position: 1.0; color: Style.currentTheme.colorModulOff}
            }

            ButtonGroup {
                buttons: columnLanguages.children
                exclusive: true
                onCheckedButtonChanged: {
                    if(_autoselect.checked) UiSettings.setLanguage("autoselect");
                    if(_english.checked) UiSettings.setLanguage("en");
                    if(_russian.checked) UiSettings.setLanguage("ru");
                    if(_italian.checked) UiSettings.setLanguage("it");
                    if(_deutch.checked)  UiSettings.setLanguage("de");
                }
            }


            Column {
                id: columnLanguages
                anchors.fill: parent
//                spacing: 4
                Item{
                    width: parent.width
                    height: parent.height/6
                    MText {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Language")
                        color: Style.colorText
                    }
                }

                MRadioButton {
                    id: _autoselect
                    text: qsTr("Autoselect")

                    height: parent.height/6
                }
                MRadioButton {
                    id: _english
                    text: "English"

                    checked: UiSettings.appLanguageCode === "en"

                    height: parent.height/6
               }
                MRadioButton {
                    id: _russian
                    text: "Русский"

                    checked: UiSettings.appLanguageCode === "ru"

                    height: parent.height/6
                }
                MRadioButton {
                    id: _italian
                    text: "Italiano" //Italiana

                    checked: UiSettings.appLanguageCode === "it"

                    height: parent.height/6
                }
                MRadioButton {
                    id: _deutch
                    text: "Deutsch" //Deutsche

                    checked: UiSettings.appLanguageCode === "de"

                    height: parent.height/6
               }

            }

        }

        Rectangle{
            id: _boxCheckUpdates

            width: parent.width*0.98
            height: parent.height*0.25
            border.color: Style.currentTheme.colorBorderOn
            radius: Style.baseRadius

            gradient: Gradient{
                GradientStop{position: 0.0; color: Style.colorModul}
                GradientStop{position: 1.0; color: Style.currentTheme.colorModulOff}
            }

            Column{
                anchors.fill: parent

                CheckBox{
                    id: _checkBoxConvert

                    checked: UiSettings.autoConvertWav
                    text: qsTr("Auto convert WAV")

                    contentItem: MText{
                        text: _checkBoxConvert.text

                        color: Style.colorText
                        anchors.left: _checkBoxConvert.indicator.right
                    //     verticalAlignment: Text.AlignVCenter
                        leftPadding: _checkBoxConvert.indicator.width/5
                    }
                    onCheckStateChanged: {
                        UiSettings.saveSetting("auto_convert_wav", _checkBoxConvert.checked);
                    }
                }

                CheckBox{
                    id: _checkBoxTrim

                    checked: UiSettings.autoTrimWav

                    text: qsTr("Auto trim WAV")
                    contentItem: MText{
                        text: _checkBoxTrim.text

                        color: Style.colorText
                        anchors.left: _checkBoxTrim.indicator.right
                        // verticalAlignment: Text.AlignVCenter
                        leftPadding: _checkBoxTrim.indicator.width/5
                    }
                    onCheckStateChanged: {
                        UiSettings.saveSetting("auto_trim_wav", _checkBoxTrim.checked);
                    }
                }

                CheckBox{
                    id: _checkBoxUpdates

                    checked: UiSettings.checkUpdatesEnabled

                    text: qsTr("Auto check updates")
                    contentItem: MText{
                        text: _checkBoxUpdates.text

                        color: Style.colorText
                        anchors.left: _checkBoxUpdates.indicator.right
                        // verticalAlignment: Text.AlignVCenter
                        leftPadding: _checkBoxUpdates.indicator.width/5
                    }
                    onCheckStateChanged: {
                        UiSettings.saveSetting("check_updates_enable", _checkBoxUpdates.checked);
                    }
                }
            }
        }

        MButton{
            id: _btnBack

            width: parent.width*0.5
            height: parent.height*0.075
            anchors.horizontalCenter: parent.horizontalCenter

            //: Back to the previous page
            textButton: qsTr("Back")

            border.color: _btnBack.active ? Style.currentTheme.colorBorderOff : Style.currentTheme.colorBorderOn

            onMbPressed:
            {
                _root.closeSettingsWindow()
            }
        }
    }
}
