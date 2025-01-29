import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

import CppObjects

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
                    if(_darkGreen.checked) Style.currentTheme = Style.themeGreen
                    if(_darkBlue.checked) Style.currentTheme = Style.themeBlue
                    if(_darkOrange.checked) Style.currentTheme = Style.themeOrange

                    UiCore.saveSetting("color_theme" ,_btnGroupTheme.checkedButton.themeName);
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
                    property string themeName: "dark_orange"

                    colorCheck: Style.themeOrange.colorModulOn
                    text: qsTr("Dark orange")
               }

                MRadioButton {
                    id: _darkGreen
                    property string themeName: "dark_green"

                    colorCheck: Style.themeGreen.colorTextEnabled
                    text: qsTr("Dark green")
                }

                MRadioButton {
                    id: _darkBlue
                    property string themeName: "dark_blue"

                    colorCheck: Style.themeBlue.colorTextEnabled
                    text: qsTr("Dark blue")
                }
            }
        }

        Rectangle{
            width: parent.width*0.98
            height: parent.height*0.4
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
                    if(_autoselect.checked) UiCore.setLanguage("autoselect");
                    if(_english.checked) UiCore.setLanguage("en");
                    if(_russian.checked) UiCore.setLanguage("ru");
                    if(_italian.checked) UiCore.setLanguage("it");
                    if(_deutch.checked)  UiCore.setLanguage("de");
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

                    height: parent.height/6
               }
                MRadioButton {
                    id: _russian
                    text: "Русский"

                    height: parent.height/6
                }
                MRadioButton {
                    id: _italian
                    text: "Italiano" //Italiana

                    height: parent.height/6
                }
                MRadioButton {
                    id: _deutch
                    text: "Deutsch" //Deutsche

                    height: parent.height/6
               }

            }

        }

        Rectangle{
            id: _boxCheckUpdates

            width: parent.width*0.98
            height: parent.height*0.12
            border.color: Style.currentTheme.colorBorderOn
            radius: Style.baseRadius

            gradient: Gradient{
                GradientStop{position: 0.0; color: Style.colorModul}
                GradientStop{position: 1.0; color: Style.currentTheme.colorModulOff}
            }

            CheckBox{
                id: _checkBoxUpdates

                x: _boxCheckUpdates.width/2 - _contentText.contentWidth - _checkBoxUpdates.indicator.width/2

                anchors.verticalCenter: parent.verticalCenter

                contentItem: MText{
                    id: _contentText

                    color: Style.colorText
                    text: qsTr("Auto check updates")
                    anchors.left: _checkBoxUpdates.indicator.right
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: _checkBoxUpdates.indicator.width/5
                }
                onCheckStateChanged: {
                    UiCore.saveSetting("check_updates_enable", _checkBoxUpdates.checked);
                }
            }
        }

        MButton{
            id: _btnBack

            width: parent.width*0.5
            height: parent.height*0.1
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

    Connections
    {
        target: UiCore

        function onSgSetUIParameter(nameParam, inValue)
        {
            if(nameParam === "check_updates_enable")
            {
                _checkBoxUpdates.checked = inValue;
            }
        }

        function onSgSetUIText(nameParam, inText)
        {
            if(nameParam === "color_theme")
            {
                if(inText === "dark_orange") _darkOrange.checked = true
                if(inText === "dark_green") _darkGreen.checked = true
                if(inText === "dark_blue") _darkBlue.checked = true
            }


            // TODO UiSettings???? Сделать также как в Desktop
            if(nameParam === "application_language")
            {
                if(inText === "autoselect") _autoselect.checked = true;
                if(inText === "en") _english.checked = true;
                if(inText === "ru") _russian.checked = true;
                if(inText === "it") _italian.checked = true;
                if(inText === "de") _deutch.checked = true;
            }
        }
    }
}
