cmake_minimum_required(VERSION 3.21)

project(BackendTests LANGUAGES CXX)

set(PANGAEA_BACKEND_DIR "${CMAKE_SOURCE_DIR}/PangaeaBackend/")

enable_testing()

# Add all test subdirectories

add_subdirectory(cores)
add_subdirectory(device)
# add_subdirectory(modules)
# add_subdirectory(models)
# add_subdirectory(utils)

# Summary message
message(STATUS "Backend unit tests configured")
message(STATUS "To run all tests: ctest")
message(STATUS "To run specific test: ctest -R TestName")
message(STATUS "To run with verbose output: ctest --verbose")
