import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15

import StyleSettings 1.0

import Elements 1.0
import Modules 1.0

import CppObjects

Item
{
    id: _root

    width:  parent.width
    height: parent.height

    property int mode: 0

    property bool copyFirst: false
    property bool presetEdited: DeviceProperties.presetModified

    property bool compareState: false

    property int borderSpacings : Style.mainSpacing

    signal exportPreset()
    signal importPreset()

    signal openPresetsList()

    Row
    {
        id: _row

        width:  parent.width
        height: parent.height

        spacing: borderSpacings

        BankSpin
        {
            width:  parent.width*1/6 - _row.spacing
            height: parent.height

            visible: !DeviceProperties.isLa3Mode

            onOpenPresetsList: _root.openPresetsList();
        }

        GridLayout
        {
            width:  parent.width*4/6
            height: parent.height

            columnSpacing: borderSpacings
            rowSpacing: borderSpacings

            rows: 2
            columns: 3

            ColumnLayout
            {
                Layout.preferredWidth: parent.width*2/5 - borderSpacings*2 //45*10*11/10
                Layout.preferredHeight: parent.height*11/15
                Layout.fillWidth: true
                Layout.fillHeight: true

                spacing: borderSpacings

                MButton
                {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    //: Output mode. Phones.
                    textButton: qsTr("Phones")
                    active: (mode==0)
                    onMbPressed:
                    {
                        mode = 0;
                        UiCore.setDeviceParameter(DeviceParameter.OUTPUT_MODE, mode);
                    }
                }

                MButton
                {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    //: Output mode. Balanced out.
                    textButton: qsTr("Balance")
                    active: (mode==2)
                    onMbPressed:
                    {
                        mode = 2;
                        UiCore.setDeviceParameter(DeviceParameter.OUTPUT_MODE, mode);
                    }
                }

                MButton
                {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    //: Output mode. Line out.
                    textButton: qsTr("Line")
                    active: (mode==1)
                    onMbPressed:
                    {
                        mode = 1;
                        UiCore.setDeviceParameter(DeviceParameter.OUTPUT_MODE, mode);
                    }
                }
            }

            ColumnLayout
            {
                Layout.preferredWidth: parent.width*3/10
                Layout.preferredHeight: parent.height*11/15
                spacing: borderSpacings

                MButton
                {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    //: Button text. Export preset
                    textButton: qsTr("Export")
                    enabled: !_root.compareState

                    onMbPressed: {
                        if(_root.presetEdited)
                            UiCore.sgSetUIText("preset_not_saved", "");
                        else
                            exportPreset();
                    }
                }

                MButton
                {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    // Почему-то именно кнопка save при смене своего состояния
                    // через свойство "enabled" отключала обновление SwipeView до любого следующего действия
                    // поэтому добавлено своё свойство "enabled" отключающее только нужные свойства
                    enabledKostyl: _root.presetEdited & !_root.compareState

                    //: Button text. Save preset
                    textButton: qsTr("Save")

                    onMbPressed:
                    {
                        DeviceProperties.saveChanges();
                    }
                }
                MButton
                {
                    id: _btnCompare

                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    enabled: _root.presetEdited
                    highlighted: _root.compareState

                    //: Button text. Compare preset
                    textButton: qsTr("Comp")
                    onMbPressed: AppProperties.comparePreset();/*UiCore.setParameter("compare", 0);*/
                }
            }

            ColumnLayout
            {
                Layout.preferredWidth: parent.width*3/10
                Layout.preferredHeight: parent.height*11/15
                spacing: borderSpacings

                MButton
                {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    //: Button text. Import preset
                    textButton: qsTr("Import")
                    enabled: !_root.compareState

                    onMbPressed:
                    {
                        importPreset();
                    }
                }

                MButton
                {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    //: Button text. Copy preset.
                    textButton: qsTr("Copy")
                    enabled: !_root.compareState

                    onMbPressed:
                    {
                        //UiCore.setParameter("copy", 0);
                        AppPropertis.copyPreset();
                        copyFirst=true;
                    }
                }
                MButton
                {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    //: Button text. Paste preset
                    textButton: qsTr("Paste")
                    enabled: copyFirst & !_root.compareState

                    onMbPressed:
                    {
                        // UiCore.setParameter("paste", 0)
                        AppPropertis.pastePreset();
                    }
                }
            }

            Vl
            {
                Layout.preferredWidth: parent.width
                Layout.preferredHeight:  parent.height*3/15
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.columnSpan: 3
            }

        }

        LA3ModeSpin
        {
            width:  parent.width*1/6 - _row.spacing
            height: parent.height

            visible: DeviceProperties.isLa3Mode
        }

        PresetSpin
        {
            width:  parent.width*1/6 -_row.spacing
            height: parent.height

            onOpenPresetsList: _root.openPresetsList();
        }
    }

    Connections{
        target: UiCore

        function onSgSetUIParameter(nameParam, inValue)
        {
            if(nameParam === "compare_state")
            {
                _root.compareState = inValue;
            }
        }

        function onSgSetUiDeviceParameter(paramType, value)
        {
            if(paramType === DeviceParameter.OUTPUT_MODE)
                mode = value;
        }
    }

}
