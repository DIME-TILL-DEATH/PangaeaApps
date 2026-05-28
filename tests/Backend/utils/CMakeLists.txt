cmake_minimum_required(VERSION 3.21)

project(TestUtilities LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test Core)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Test for Parser
add_executable(TestParser
    tst_parser.cpp
    ${PANGAEA_BACKEND_DIR}/utils/parser.cpp
    ${PANGAEA_BACKEND_DIR}/utils/parser.h
    ${PANGAEA_BACKEND_DIR}/utils/maskedparser.cpp
    ${PANGAEA_BACKEND_DIR}/utils/maskedparser.h
)

target_link_libraries(TestParser
    PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Core
)

add_test(NAME TestParser COMMAND TestParser)

# Test for Logger
add_executable(TestLogger
    tst_logger.cpp
    ${PANGAEA_BACKEND_DIR}/utils/logger.cpp
    ${PANGAEA_BACKEND_DIR}/utils/logger.h
)

target_link_libraries(TestLogger
    PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Core
)

add_test(NAME TestLogger COMMAND TestLogger)
