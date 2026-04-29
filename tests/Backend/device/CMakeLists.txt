cmake_minimum_required(VERSION 3.21)

project(TestDeviceClasses LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test Core)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Test for DeviceParameter
add_executable(TestDeviceParameter
    tst_deviceparameter.cpp
    # ${PANGAEA_BACKEND_DIR}/device/deviceparameter.cpp
    # ${PANGAEA_BACKEND_DIR}/device/deviceparameter.h
)

target_include_directories(TestDeviceParameter PUBLIC
     ${PANGAEA_BACKEND_DIR}/
     ${PANGAEA_BACKEND_DIR}/cores
     ${PANGAEA_BACKEND_DIR}/device
     ${PANGAEA_BACKEND_DIR}/interfaces/
     ${PANGAEA_BACKEND_DIR}/utils
     ${PANGAEA_BACKEND_DIR}/qmlEnums
     ${PANGAEA_BACKEND_DIR}/models/
     ${PANGAEA_BACKEND_DIR}/uihelpers/
     ${PANGAEA_BACKEND_DIR}/utils/


)

target_link_libraries(TestDeviceParameter PRIVATE
     Qt6::Test
     Qt6::Core

     PangaeaBackend
     converter-lib
)

add_test(NAME TestDeviceParameter COMMAND TestDeviceParameter)

# Test for DeviceAnswer
# add_executable(TestDeviceAnswer
#     tst_deviceanswer.cpp
#     ${PANGAEA_BACKEND_DIR}/device/deviceanswer.cpp
#     ${PANGAEA_BACKEND_DIR}/device/deviceanswer.h
#     ${PANGAEA_BACKEND_DIR}/device/deviceparameter.cpp
#     ${PANGAEA_BACKEND_DIR}/device/deviceparameter.h
# )

# target_include_directories(TestDeviceAnswer PUBLIC
#      ${PANGAEA_BACKEND_DIR}/
#      ${PANGAEA_BACKEND_DIR}/cores
#      ${PANGAEA_BACKEND_DIR}/device
#      ${PANGAEA_BACKEND_DIR}/interfaces/
#      ${PANGAEA_BACKEND_DIR}/utils
#      ${PANGAEA_BACKEND_DIR}/qmlEnums
#      ${PANGAEA_BACKEND_DIR}/models/
#      ${PANGAEA_BACKEND_DIR}/uihelpers/
# )

# target_link_libraries(TestDeviceAnswer
#     PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Core
# )

# add_test(NAME TestDeviceAnswer COMMAND TestDeviceAnswer)

# Test for Firmware -bad test
# add_executable(TestFirmware
#     tst_firmware.cpp
#     ${PANGAEA_BACKEND_DIR}/device/firmware.cpp
#     ${PANGAEA_BACKEND_DIR}/device/firmware.h
# )

# target_include_directories(TestFirmware PUBLIC
#      ${PANGAEA_BACKEND_DIR}/
#      ${PANGAEA_BACKEND_DIR}/cores
#      ${PANGAEA_BACKEND_DIR}/device
#      ${PANGAEA_BACKEND_DIR}/interfaces/
#      ${PANGAEA_BACKEND_DIR}/utils
#      ${PANGAEA_BACKEND_DIR}/qmlEnums
#      ${PANGAEA_BACKEND_DIR}/models/
#      ${PANGAEA_BACKEND_DIR}/uihelpers/
# )

# target_link_libraries(TestFirmware
#     PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Core
# )

# add_test(NAME TestFirmware COMMAND TestFirmware)

# Test for PresetAbstract
# add_executable(TestPresetAbstract
#     tst_presetabstract.cpp
#     ${PANGAEA_BACKEND_DIR}/device/presetabstract.cpp
#     ${PANGAEA_BACKEND_DIR}/device/presetabstract.h
#     ${PANGAEA_BACKEND_DIR}/device/deviceparameter.cpp
#     ${PANGAEA_BACKEND_DIR}/device/deviceparameter.h
# )

# target_include_directories(TestPresetAbstract PUBLIC
#      ${PANGAEA_BACKEND_DIR}/
#      ${PANGAEA_BACKEND_DIR}/cores
#      ${PANGAEA_BACKEND_DIR}/device
#      ${PANGAEA_BACKEND_DIR}/interfaces/
#      ${PANGAEA_BACKEND_DIR}/utils
#      ${PANGAEA_BACKEND_DIR}/qmlEnums
#      ${PANGAEA_BACKEND_DIR}/models/
#      ${PANGAEA_BACKEND_DIR}/uihelpers/
# )

# target_link_libraries(TestPresetAbstract
#     PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Core
# )

# add_test(NAME TestPresetAbstract COMMAND TestPresetAbstract)

# Test for IRFile
# add_executable(TestIRFile
#     tst_irfile.cpp
#     ${PANGAEA_BACKEND_DIR}/device/irfile.cpp
#     ${PANGAEA_BACKEND_DIR}/device/irfile.h
# )

# target_include_directories(TestIRFile PUBLIC
#      ${PANGAEA_BACKEND_DIR}/
#      ${PANGAEA_BACKEND_DIR}/cores
#      ${PANGAEA_BACKEND_DIR}/device
#      ${PANGAEA_BACKEND_DIR}/interfaces/
#      ${PANGAEA_BACKEND_DIR}/utils
#      ${PANGAEA_BACKEND_DIR}/qmlEnums
#      ${PANGAEA_BACKEND_DIR}/models/
#      ${PANGAEA_BACKEND_DIR}/uihelpers/
# )

# target_link_libraries(TestIRFile
#     PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Core
# )

# add_test(NAME TestIRFile COMMAND TestIRFile)
