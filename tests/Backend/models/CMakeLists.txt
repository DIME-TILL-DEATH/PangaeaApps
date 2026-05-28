cmake_minimum_required(VERSION 3.21)

project(TestModels LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test Core Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test Core Gui)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Test for PresetListModel
add_executable(TestPresetListModel
    tst_presetlistmodel.cpp
    ${PANGAEA_BACKEND_DIR}/models/presetlistmodel.cpp
    ${PANGAEA_BACKEND_DIR}/models/presetlistmodel.h
)

target_link_libraries(TestPresetListModel
    PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui
)

add_test(NAME TestPresetListModel COMMAND TestPresetListModel)

# Test for DevicesListModel
add_executable(TestDevicesListModel
    tst_deviceslistmodel.cpp
    ${PANGAEA_BACKEND_DIR}/models/deviceslistmodel.cpp
    ${PANGAEA_BACKEND_DIR}/models/deviceslistmodel.h
    ${PANGAEA_BACKEND_DIR}/interfaces/devicedescription.cpp
    ${PANGAEA_BACKEND_DIR}/interfaces/devicedescription.h
)

target_link_libraries(TestDevicesListModel
    PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui
)

add_test(NAME TestDevicesListModel COMMAND TestDevicesListModel)

# Test for ModulesListModel
add_executable(TestModulesListModel
    tst_moduleslistmodel.cpp
    ${PANGAEA_BACKEND_DIR}/models/moduleslistmodel.cpp
    ${PANGAEA_BACKEND_DIR}/models/moduleslistmodel.h
)

target_link_libraries(TestModulesListModel
    PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui
)

add_test(NAME TestModulesListModel COMMAND TestModulesListModel)

# Test for FileBrowserModel
add_executable(TestFileBrowserModel
    tst_filebrowsermodel.cpp
    ${PANGAEA_BACKEND_DIR}/models/filebrowsermodel.cpp
    ${PANGAEA_BACKEND_DIR}/models/filebrowsermodel.h
)

target_link_libraries(TestFileBrowserModel
    PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui
)

add_test(NAME TestFileBrowserModel COMMAND TestFileBrowserModel)
