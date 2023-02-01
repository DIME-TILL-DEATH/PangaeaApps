import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15
//import Qt.labs.platform 1.1 as Labs
//import QtQuick.Dialogs 1.3

import StyleSettings 1.0

import Elements 1.0
import Modules 1.0

Item
{
    id: _root

    width:  parent.width
    height: parent.height

    property int mode: 0

    property bool copyFirst: false
    property bool presetEdited: false

    property bool compareState: false

    property alias bank : _bank
    property alias preset : _preset

    property int borderSpacings : Style.mainSpacing

    signal exportPreset()
    signal importPreset()

    Row
    {
        id: _row

        width:  parent.width
        height: parent.height

        spacing: borderSpacings

        PresetBankSpin
        {
            id: _bank

            width:  parent.width*1/6 - _row.spacing
            height: parent.height

            text: qsTr("BANK")
            nameValue: "bank"
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
                        _uiCore.setParameter("output_mode", mode);
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
                        _uiCore.setParameter("output_mode", mode);
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
                        _uiCore.setParameter("output_mode", mode);
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
                            _uiCore.sgSetUIText("preset_not_saved", "");
                        else
                            exportPreset();
                            //exportPresetDialog.open();
                            //exportPresetFileDialog.open();
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
                        _uiCore.setParameter("save_change", (-1));
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
                    onMbPressed: _uiCore.setParameter("compare", 0);
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
                        //importPresetFileDialog.open();
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
                        _uiCore.setParameter("copy", 0);
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
                        _uiCore.setParameter("paste", 0)
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

        PresetBankSpin
        {
            id: _preset

            width:  parent.width*1/6 -_row.spacing
            height: parent.height

            text: qsTr("PRESET")
            nameValue: "preset"
        }
    }

////    FileDialog{
////        id: exportPresetFileDialog

////        modality: Qt.WindowModal

////        selectExisting:  false
////        selectMultiple: false

////        folder: Labs.StandardPaths.writableLocation(Labs.StandardPaths.GenericDataLocation)

////        defaultSuffix: "pst"
////        nameFilters: ["Firmware files (*.pst)"]
////        onAccepted:
////        {
////            _uiCore.exportPreset(exportPresetFileDialog.fileUrl);
////        }
////    }

    Connections{
        target: _uiCore

        function onSgSetUIParameter(nameParam, inValue)
        {
            if(nameParam === "output_mode")
                mode = inValue;

            if(nameParam === "preset_edited")
            {
                _root.presetEdited = inValue;
            }

            if(nameParam === "compare_state")
            {
                _root.compareState = inValue;
            }
        }
    }

}
